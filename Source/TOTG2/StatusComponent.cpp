#include "StatusComponent.h"
#include "TimerManager.h"

void UStatusComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentStamina = MaxStamina;
}

void UStatusComponent::StaminaDrain(float Amount)
{
    CurrentStamina = FMath::Clamp(CurrentStamina - Amount, 0.f, MaxStamina);
    // Cancel any running regen, restart delay
    GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);
    StartStaminaRegen();
}

void UStatusComponent::StartStaminaRegen()
{
    GetWorld()->GetTimerManager().SetTimer(
        RegenTimerHandle,
        this,
        &UStatusComponent::RegenTick,
        sRegenDelay,
        true
    );
}

void UStatusComponent::RegenTick()
{
    CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRegen, 0.f, MaxStamina);
    if (CurrentStamina >= MaxStamina)
        GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);
}

bool UStatusComponent::CanAct() const
{
    switch (CombatState)
    {
        case ECombatState::Dodging:
        case ECombatState::Stunned:
        case ECombatState::Dead:
            return false;
        default:
            return true;
    }
}
void UStatusComponent::SetCombatState(ECombatState NewState)
{
    CombatState = NewState;
}