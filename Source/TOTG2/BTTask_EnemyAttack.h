#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EnemyAttack.generated.h"

UCLASS()
class TOTG2_API UBTTask_EnemyAttack : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_EnemyAttack();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

    UPROPERTY(EditAnywhere, Category="Attack")
    float AttackRange = 120.f;

    UPROPERTY(EditAnywhere, Category="Attack")
    int32 MinAttacks = 1;

    UPROPERTY(EditAnywhere, Category="Attack")
    int32 MaxAttacks = 3;

private:
    UPROPERTY()
    UBehaviorTreeComponent* ActiveOwnerComp = nullptr;

    FTimerHandle AttackFinishTimer;

    void FinishAttackTask();
};