#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.h"
#include "AtkComponent.generated.h"
 
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOTG2_API UAtkComponent : public UActorComponent
{
    GENERATED_BODY()
 
public:
    UAtkComponent();
 
    // --- Stamina Costs ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
    float LightStaminaCost = 20.f;
 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
    float HeavyStaminaCost = 40.f;
 
    // --- Combo Settings ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
    int32 MaxComboCount = 2;
 
    UPROPERTY(BlueprintReadOnly, Category="Attack")
    int32 CurrentCombo = 0;
 
    UPROPERTY(BlueprintReadOnly, Category="Attack")
    bool bCanAttack = true;
 
    // --- Trace State ---
    UPROPERTY(BlueprintReadOnly, Category="Attack|Trace")
    bool bTraceActive = false;
 
    // --- Trace Settings ---
    UPROPERTY(EditAnywhere, Category="Attack|Trace")
    float TraceRadius = 15.0f;
 
    UPROPERTY(EditAnywhere, Category="Attack|Trace")
    float TraceLength = 80.0f;
 
    UPROPERTY(EditAnywhere, Category="Attack|Trace")
    float LightAttackDamage = 20.0f;
 
    // --- Combat Interface ---
    UFUNCTION(BlueprintCallable, Category="Attack")
    bool TryLightAttack();
 
    UFUNCTION(BlueprintCallable, Category="Attack")
    void TryHeavyAttack();
 
    UFUNCTION(BlueprintCallable, Category="Attack")
    void OnAttackEnd();

    UFUNCTION(BlueprintCallable, Category="Attack")
    void InterruptAttack();
 
    UFUNCTION(BlueprintCallable, Category="Attack|Trace")
    void OnWeaponTraceStart();
 
    UFUNCTION(BlueprintCallable, Category="Attack|Trace")
    void OnWeaponTraceStop();

    UPROPERTY(EditAnywhere, Category="Attack|Feel")
    TSubclassOf<UCameraShakeBase> HitCameraShake;
 
protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
 
private:
    UPROPERTY()
    ACharacter* OwnerCharacter;
 
    UPROPERTY()
    UStatusComponent* StatusComp;
 
    FTimerHandle ComboResetTimer;
 
    bool bAttacking = false;
 
    UPROPERTY()
    TArray<AActor*> HitActorsThisSwing;
 
    void ResetCombo();
    void DoWeaponTrace();
};
