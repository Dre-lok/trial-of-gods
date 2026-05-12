#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

// Delegates must live outside the class
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamaged, float, NewHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOTG2_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
    float MaxHealth = 100.f;

    UPROPERTY(BlueprintReadOnly, Category="Health")
    float CurrentHealth;

    UFUNCTION(BlueprintCallable, Category="Health")
    void ApplyDamage(float Amount);

    UFUNCTION(BlueprintCallable, Category="Health")
    bool IsDead() const;

    UPROPERTY(BlueprintAssignable, Category="Health")
    FOnDeath OnDeath;

    UPROPERTY(BlueprintAssignable, Category="Health")
    FOnDamaged OnDamaged;

protected:
    virtual void BeginPlay() override;
};