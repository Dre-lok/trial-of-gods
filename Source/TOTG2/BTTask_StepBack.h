#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StepBack.generated.h"

UCLASS()
class TOTG2_API UBTTask_StepBack : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UBTTask_StepBack();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    FTimerHandle StepBackTimer;
    UBehaviorTreeComponent* ActiveOwnerComp = nullptr;

    UFUNCTION()
    void FinishTask();
};