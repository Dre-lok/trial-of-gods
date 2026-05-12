#include "BTTask_StepBack.h"
#include "AEnemyBase.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_StepBack::UBTTask_StepBack()
{
    NodeName = "Step Back";
}

EBTNodeResult::Type UBTTask_StepBack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIC = OwnerComp.GetAIOwner();
    if (!AIC) return EBTNodeResult::Failed;

    AEnemyBase* Enemy = Cast<AEnemyBase>(AIC->GetPawn());
    if (!Enemy) return EBTNodeResult::Failed;

    AActor* Target = Cast<AActor>(
        OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("TargetActor"))
    );
    if (!Target) return EBTNodeResult::Failed;

    // Direction away from player
    FVector AwayDir = (Enemy->GetActorLocation() - Target->GetActorLocation()).GetSafeNormal();
    FVector StepTarget = Enemy->GetActorLocation() + AwayDir * Enemy->StepBackDistance;

    // Slow down during step back
    Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->WalkSpeed;

    AIC->MoveToLocation(StepTarget, 20.f, false);

    ActiveOwnerComp = &OwnerComp;
    Enemy->GetWorldTimerManager().SetTimer(
        StepBackTimer, this, &UBTTask_StepBack::FinishTask, 0.8f, false
    );

    return EBTNodeResult::InProgress;
}

void UBTTask_StepBack::FinishTask()
{
    if (ActiveOwnerComp)
    {
        FinishLatentTask(*ActiveOwnerComp, EBTNodeResult::Succeeded);
        ActiveOwnerComp = nullptr;
    }
}