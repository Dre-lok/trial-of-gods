#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_ClearTarget.generated.h"

UCLASS()
class TOTG2_API UBTTask_ClearTarget : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_ClearTarget();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    UPROPERTY(EditAnywhere, Category="Blackboard")
    FBlackboardKeySelector TargetKey;
};