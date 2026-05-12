#include "BTTask_FindPatrolPoint.h"
#include "PatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolPoint::UBTTask_FindPatrolPoint()
{
    NodeName = TEXT("Find Patrol Point");
}

EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    APawn* AIPawn = AIController->GetPawn();
    if (!AIPawn) return EBTNodeResult::Failed;

    // Get all patrol points in the level
    TArray<AActor*> PatrolPoints;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPoint::StaticClass(), PatrolPoints);

    if (PatrolPoints.IsEmpty()) return EBTNodeResult::Failed;

    // Find the closest patrol point we're NOT already at
    AActor* BestPoint = nullptr;
    float BestDistance = FLT_MAX;
    FVector CurrentLocation = AIPawn->GetActorLocation();

    // Get current patrol target to avoid repeating it
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    FVector CurrentPatrolLoc = Blackboard->GetValueAsVector(PatrolLocationKey.SelectedKeyName);

    for (AActor* Point : PatrolPoints)
    {
        float Dist = FVector::Dist(CurrentLocation, Point->GetActorLocation());
        
        // Skip if this is basically where we already are
        if (FVector::Dist(Point->GetActorLocation(), CurrentPatrolLoc) < 100.f) continue;

        if (Dist < BestDistance)
        {
            BestDistance = Dist;
            BestPoint = Point;
        }
    }

    // Fallback — if all points filtered, just pick the first
    if (!BestPoint && PatrolPoints.Num() > 0)
        BestPoint = PatrolPoints[0];

    if (!BestPoint) return EBTNodeResult::Failed;

    Blackboard->SetValueAsVector(PatrolLocationKey.SelectedKeyName, BestPoint->GetActorLocation());
    return EBTNodeResult::Succeeded;
}