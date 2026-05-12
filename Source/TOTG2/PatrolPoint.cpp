#include "PatrolPoint.h"
#include "Components/BillboardComponent.h"

APatrolPoint::APatrolPoint()
{
    PrimaryActorTick.bCanEverTick = false;

    // Visible in editor only — no runtime cost
    UBillboardComponent* Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
    RootComponent = Billboard;
}