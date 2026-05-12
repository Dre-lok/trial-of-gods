#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DodgeComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOTG2_API UDodgeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UDodgeComponent();

    // Tweak these in Blueprint/editor without recompiling
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dodge")
    float DodgeForce = 1400.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dodge")
    float DodgeDuration = 0.55f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dodge")
    float StaminaCost = 25.f;

    UPROPERTY(BlueprintReadOnly, Category="Dodge")
    bool bIsDodging = false;

    // Blueprint calls this on IA_Dodge input
    UFUNCTION(BlueprintCallable, Category="Dodge")
    void TryDodge(FVector InputDirection);

private:
    FTimerHandle DodgeTimerHandle;
    ACharacter* OwnerCharacter;

    void OnDodgeEnd();

protected:
    virtual void BeginPlay() override;
};