#include "DodgeComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "StatusComponent.h"

UDodgeComponent::UDodgeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDodgeComponent::BeginPlay()
{
    Super::BeginPlay();
    OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UDodgeComponent::TryDodge(FVector InputDirection)
{
    UStatusComponent* Status = OwnerCharacter->FindComponentByClass<UStatusComponent>();
    if (!Status || Status->CurrentStamina < StaminaCost) return;
    Status->StaminaDrain(StaminaCost);
    
    if (bIsDodging || !OwnerCharacter) return;

    FVector DodgeDir = InputDirection.IsNearlyZero()
        ? -OwnerCharacter->GetActorForwardVector()
        : InputDirection.GetSafeNormal();

    bIsDodging = true;

    // Kill any existing velocity first - gives that snappy feel
    OwnerCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;

    // Sharp initial burst
    OwnerCharacter->LaunchCharacter(DodgeDir * DodgeForce, true, false);

    // Brake aggressively mid-dodge so it stops crisply
    OwnerCharacter->GetCharacterMovement()->BrakingFrictionFactor = 8.f;
    OwnerCharacter->GetCharacterMovement()->BrakingDecelerationWalking = 5000.f;

    GetWorld()->GetTimerManager().SetTimer(
        DodgeTimerHandle,
        this,
        &UDodgeComponent::OnDodgeEnd,
        DodgeDuration,
        false
    );
}

void UDodgeComponent::OnDodgeEnd()
{
    bIsDodging = false;

    // Restore normal movement feel
    OwnerCharacter->GetCharacterMovement()->BrakingFrictionFactor = 2.f;
    OwnerCharacter->GetCharacterMovement()->BrakingDecelerationWalking = 2048.f;
}