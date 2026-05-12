#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPoint.generated.h"

UCLASS()
class TOTG2_API UBTTask_FindPatrolPoint : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_FindPatrolPoint();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    // The blackboard key to write the patrol location into
    UPROPERTY(EditAnywhere, Category="Patrol")
    FBlackboardKeySelector PatrolLocationKey;
};