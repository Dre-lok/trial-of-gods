#include "HealthComponent.h"

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth;
}

void UHealthComponent::ApplyDamage(float Amount)
{
    if (IsDead()) return;

    CurrentHealth = FMath::Clamp(CurrentHealth - Amount, 0.f, MaxHealth);
    UE_LOG(LogTemp, Warning, TEXT("[HEALTH] %.1f / %.1f"), CurrentHealth, MaxHealth);

    OnDamaged.Broadcast(CurrentHealth);

    if (IsDead())
        OnDeath.Broadcast();
}

bool UHealthComponent::IsDead() const
{
    return CurrentHealth <= 0.f;
}