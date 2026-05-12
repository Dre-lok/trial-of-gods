#include "BTTask_Roam.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Character.h"

UBTTask_Roam::UBTTask_Roam()
{
    NodeName = "Roam";
}

EBTNodeResult::Type UBTTask_Roam::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIC = OwnerComp.GetAIOwner();
    if (!AIC) return EBTNodeResult::Failed;

    APawn* Pawn = AIC->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSys) return EBTNodeResult::Failed;

    FNavLocation RoamTarget;
    bool bFound = NavSys->GetRandomReachablePointInRadius(
        Pawn->GetActorLocation(), RoamRadius, RoamTarget
    );

    if (!bFound) return EBTNodeResult::Failed;

    AIC->MoveToLocation(RoamTarget.Location, 50.f, false);
    return EBTNodeResult::Succeeded;
}