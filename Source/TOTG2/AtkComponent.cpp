#include "AtkComponent.h"
#include "HealthComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

UAtkComponent::UAtkComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UAtkComponent::BeginPlay()
{
    Super::BeginPlay();
    OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
        StatusComp = OwnerCharacter->FindComponentByClass<UStatusComponent>();
}

void UAtkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    DoWeaponTrace();
}

bool UAtkComponent::TryLightAttack()
{
    if (!StatusComp || !StatusComp->CanAct()) return false;
    if (StatusComp->CurrentStamina < LightStaminaCost) return false;
    if (bAttacking) return false;

    bAttacking = true;
    bCanAttack = false;
    StatusComp->StaminaDrain(LightStaminaCost);
    StatusComp->SetCombatState(ECombatState::Attacking);
    GetWorld()->GetTimerManager().ClearTimer(ComboResetTimer);

    UE_LOG(LogTemp, Warning, TEXT("[ATK] gate passed — combo index: %d"), CurrentCombo);
    return true;
}

void UAtkComponent::TryHeavyAttack()
{
    if (!StatusComp || !StatusComp->CanAct()) return;
    if (StatusComp->CurrentStamina < HeavyStaminaCost) return;

    StatusComp->StaminaDrain(HeavyStaminaCost);
    StatusComp->SetCombatState(ECombatState::Armed);
    CurrentCombo = 0;
}

void UAtkComponent::OnAttackEnd()
{
    bAttacking = false;
    bCanAttack = true;

    CurrentCombo++;
    UE_LOG(LogTemp, Warning, TEXT("[ATK] OnAttackEnd — next index: %d"), CurrentCombo);

    if (CurrentCombo >= MaxComboCount)
    {
        ResetCombo();
        return;
    }

    StatusComp->SetCombatState(ECombatState::Armed);
    GetWorld()->GetTimerManager().SetTimer(
        ComboResetTimer, this, &UAtkComponent::ResetCombo, 2.0f, false
    );
}

void UAtkComponent::ResetCombo()
{
    UE_LOG(LogTemp, Warning, TEXT("[ATK] ResetCombo"));
    CurrentCombo = 0;
    bCanAttack = true;
    bAttacking = false;
    StatusComp->SetCombatState(ECombatState::Unarmed);
}

void UAtkComponent::OnWeaponTraceStart()
{
    bTraceActive = true;
    HitActorsThisSwing.Empty();
    UE_LOG(LogTemp, Warning, TEXT("[ATK] WeaponTraceStart"));
}

void UAtkComponent::OnWeaponTraceStop()
{
    bTraceActive = false;
    UE_LOG(LogTemp, Warning, TEXT("[ATK] WeaponTraceStop"));
}

void UAtkComponent::DoWeaponTrace()
{
    if (!bTraceActive) return;

    AActor* Owner = GetOwner();
    if (!Owner) return;

    USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
    if (!Mesh) return;

    FVector Start = Mesh->GetSocketLocation(FName("weapon_r"));
    FVector End = Start + Mesh->GetSocketRotation(FName("weapon_r")).Vector() * TraceLength;


    TArray<FHitResult> Hits;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceRadius);

    GetWorld()->SweepMultiByChannel(
        Hits, Start, End, FQuat::Identity,
        ECC_Pawn, Sphere
    );

    for (FHitResult& Hit : Hits)
    {
        AActor* HitActor = Hit.GetActor();
        if (!HitActor || HitActor == Owner) continue;
        if (HitActorsThisSwing.Contains(HitActor)) continue;

        HitActorsThisSwing.Add(HitActor);

        UHealthComponent* HealthComp = HitActor->FindComponentByClass<UHealthComponent>();
        if (HealthComp)
        {
            HealthComp->ApplyDamage(LightAttackDamage);
            
            if (HitCameraShake)
            {
                if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
                    PC->ClientStartCameraShake(HitCameraShake);
            }
        }
    }
} 


void UAtkComponent::InterruptAttack()
{
    if (!bAttacking) return;

    bAttacking = false;
    bCanAttack = true;
    GetWorld()->GetTimerManager().ClearTimer(ComboResetTimer);
    ResetCombo();

    UE_LOG(LogTemp, Warning, TEXT("[ATK] Attack interrupted by hit"));
}