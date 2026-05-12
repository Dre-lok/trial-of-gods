#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
    Unarmed     UMETA(DisplayName="Unarmed"),
    Armed       UMETA(DisplayName="Armed"),
    Attacking   UMETA(DisplayName="Attacking"),
    Dodging     UMETA(DisplayName="Dodging"),
    Stunned     UMETA(DisplayName="Stunned"),
    Dead        UMETA(DisplayName="Dead")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOTG2_API UStatusComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
    float MaxStamina = 100.f;

    UPROPERTY(BlueprintReadWrite, Category="Stamina")
    float CurrentStamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
    float StaminaRegen = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina")
    float sRegenDelay = 1.f;

    UPROPERTY(BlueprintReadWrite, Category="Stamina")
    bool bIsActing = false;

    UPROPERTY(BlueprintReadWrite, Category="Combat")
    ECombatState CombatState = ECombatState::Unarmed;

    UFUNCTION(BlueprintCallable, Category="Stamina")
    void StaminaDrain(float Amount);

    UFUNCTION(BlueprintCallable, Category="Stamina")
    void StartStaminaRegen();

    UFUNCTION(BlueprintCallable, Category="Combat")
    bool CanAct() const;

    UFUNCTION(BlueprintCallable, Category="Combat")
    void SetCombatState(ECombatState NewState);

protected:
    virtual void BeginPlay() override;

private:
    FTimerHandle RegenTimerHandle;
    void RegenTick();
};