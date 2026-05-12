#include "AEnemyBase.h"
#include "Animation/AnimInstance.h"
#include "AEnemyAIController.h"
#include "HealthComponent.h"
#include "DrawDebugHelpers.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    AIControllerClass = AEnemyAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
    HealthBarWidget->SetupAttachment(GetRootComponent());
    HealthBarWidget->SetWidgetSpace(EWidgetSpace::World);
    HealthBarWidget->SetDrawSize(FVector2D(200.f, 20.f));
    HealthBarWidget->SetVisibility(false);
}

void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();

    if (HealthComponent)
    {
        HealthComponent->OnDeath.AddDynamic(this, &AEnemyBase::OnDeath);
        HealthComponent->OnDamaged.AddDynamic(this, &AEnemyBase::OnDamaged);
    }

    if (GetMesh() && GetMesh()->GetAnimInstance())
        GetMesh()->GetAnimInstance()->InitializeAnimation();
}

void AEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    DoAttackTrace();
}

// --- Damage ---

void AEnemyBase::OnDamaged(float NewHealth)
{
    if (bIsDead) return;
    
    UE_LOG(LogTemp, Warning, TEXT("[ENEMY] OnDamaged — health: %.1f"), NewHealth);
    HitsReceived++;

    // Show health bar
    HealthBarWidget->SetVisibility(true);

    // Update the health bar fill
    if (UUserWidget* Widget = Cast<UUserWidget>(HealthBarWidget->GetUserWidgetObject()))
    {
        if (UProgressBar* Bar = Cast<UProgressBar>(Widget->GetWidgetFromName(TEXT("HealthBar"))))
        {
            float Percent = HealthComponent->CurrentHealth / HealthComponent->MaxHealth;
            Bar->SetPercent(Percent);
        }
    }

    // Reset hide timer
    GetWorld()->GetTimerManager().SetTimer(
        HealthBarHideTimer, [this]()
        {
            HealthBarWidget->SetVisibility(false);
        }, 3.0f, false
    );

    // Hit react
    if (!HitReactMontage) return;
    UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
    if (AnimInst && !AnimInst->Montage_IsPlaying(HitReactMontage))
        AnimInst->Montage_Play(HitReactMontage);
}

void AEnemyBase::OnDeath()
{
    if (bIsDead) return;
    bIsDead = true;

    UE_LOG(LogTemp, Warning, TEXT("[ENEMY] OnDeath — %s"), *GetName());

    // 1. Stop and shut down the entire behavior tree
    if (AAIController* AIC = Cast<AAIController>(GetController()))
    {
        AIC->StopMovement();
        if (UBrainComponent* Brain = AIC->GetBrainComponent())
            Brain->StopLogic(TEXT("Dead"));
    }

    // 2. Kill movement entirely
    GetCharacterMovement()->DisableMovement();
    GetCharacterMovement()->StopMovementImmediately();

    // 3. Disable capsule so they don't block/slide
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 4. Play death montage and destroy after
    if (DeathMontage)
    {
        UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
        if (AnimInst)
        {
            float MontageLength = AnimInst->Montage_Play(DeathMontage);
            GetWorld()->GetTimerManager().SetTimer(
                DeathDestroyTimer, this, &AEnemyBase::HandleDeathDestroy,
                3.5f, false
            );
            return;
        }
    }

    Destroy();
}

void AEnemyBase::HandleDeathDestroy()
{
    Destroy();
}
// --- Attack Trace ---

void AEnemyBase::OnAttackTraceStart()
{
    bAttackTraceActive = true;
    HitActorsThisSwing.Empty();
    UE_LOG(LogTemp, Warning, TEXT("[ENEMY] AttackTraceStart"));
}

void AEnemyBase::OnAttackTraceStop()
{
    bAttackTraceActive = false;
    UE_LOG(LogTemp, Warning, TEXT("[ENEMY] AttackTraceStop"));
}

void AEnemyBase::DoAttackTrace()
{
    if (!bAttackTraceActive) return;
    

    USkeletalMeshComponent* SkelMesh = GetMesh();
    if (!SkelMesh) return;

    FVector Start = SkelMesh->GetSocketLocation(FName("weapon_r"));
    FVector End = Start + SkelMesh->GetSocketRotation(FName("weapon_r")).Vector() * AttackTraceLength;

    TArray<FHitResult> Hits;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(AttackTraceRadius);

    GetWorld()->SweepMultiByChannel(
        Hits, Start, End, FQuat::Identity, ECC_Pawn, Sphere
    );


    for (FHitResult& Hit : Hits)
    {
        AActor* HitActor = Hit.GetActor();
        if (!HitActor || HitActor == this) continue;
        if (HitActorsThisSwing.Contains(HitActor)) continue;

        HitActorsThisSwing.Add(HitActor);

        UHealthComponent* HealthComp = HitActor->FindComponentByClass<UHealthComponent>();
        if (HealthComp)
        {
            HealthComp->ApplyDamage(AttackDamage);
            
            UE_LOG(LogTemp, Warning, TEXT("[ENEMY] Hit: %s for %.1f"), *HitActor->GetName(), AttackDamage);
        }
    }
}