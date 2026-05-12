#include "BTTask_ClearTarget.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTTask_ClearTarget::UBTTask_ClearTarget()
{
    NodeName = TEXT("Clear Target");
}

EBTNodeResult::Type UBTTask_ClearTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return EBTNodeResult::Failed;

    Blackboard->ClearValue(TargetKey.SelectedKeyName);
    return EBTNodeResult::Succeeded;
}