#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CirclePlayer.generated.h"

UCLASS()
class TOTG2_API UBTTask_CirclePlayer : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UBTTask_CirclePlayer();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
    virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    float ElapsedTime = 0.f;
    float CircleDuration = 0.f;
    int32 CircleDirection = 1; // 1 = right, -1 = left
};
