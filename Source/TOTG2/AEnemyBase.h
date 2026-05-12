#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Components/WidgetComponent.h"
#include "AEnemyBase.generated.h"

UCLASS()
class TOTG2_API AEnemyBase : public ACharacter, public IGenericTeamAgentInterface
{
    GENERATED_BODY()

public:
    AEnemyBase();
    virtual void Tick(float DeltaTime) override;

    // --- Team ---
    virtual FGenericTeamId GetGenericTeamId() const override 
    { return FGenericTeamId(1); }

    // --- Components ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
    UHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, Category="UI")
    UWidgetComponent* HealthBarWidget;
    
    FTimerHandle HealthBarHideTimer;
    

    // --- Montages ---
    UPROPERTY(EditDefaultsOnly, Category="Combat|Animations")
    UAnimMontage* HitReactMontage;

    UPROPERTY(EditDefaultsOnly, Category="Combat|Animations")
    UAnimMontage* DeathMontage;

    UPROPERTY(EditDefaultsOnly, Category="Combat|Animations")
    UAnimMontage* AttackMontage;

    // --- Combat Settings ---
    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float AttackDamage = 10.f;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float AttackTraceRadius = 20.f;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float AttackTraceLength = 80.f;

    // --- Hit Counter ---
    UPROPERTY(BlueprintReadOnly, Category="Combat")
    int32 HitsReceived = 0;

    UPROPERTY(BlueprintReadOnly, Category="Combat")
    bool bIsDead = false;

    // --- Trace Interface (called from anim notifies) ---
    UFUNCTION(BlueprintCallable, Category="Combat")
    void OnAttackTraceStart();

    UFUNCTION(BlueprintCallable, Category="Combat")
    void OnAttackTraceStop();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    float StepBackDistance = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    float CircleRadius = 300.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    float CircleDuration = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    float ChaseSpeed = 400.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    float WalkSpeed = 200.f;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnDeath();

    UFUNCTION()
    void OnDamaged(float NewHealth);

private:
    bool bAttackTraceActive = false;

    UPROPERTY()
    TArray<AActor*> HitActorsThisSwing;

    FTimerHandle DeathDestroyTimer;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float DeathLingerTime = 3.0f;

    void FinalDestroy();

    void DoAttackTrace();
    void HandleDeathDestroy();
};