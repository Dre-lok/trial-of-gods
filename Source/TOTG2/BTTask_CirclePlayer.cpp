#include "BTTask_CirclePlayer.h"
#include "AEnemyBase.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_CirclePlayer::UBTTask_CirclePlayer()
{
    NodeName = "Circle Player";
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_CirclePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIC = OwnerComp.GetAIOwner();
    if (!AIC) return EBTNodeResult::Failed;

    AEnemyBase* Enemy = Cast<AEnemyBase>(AIC->GetPawn());
    if (!Enemy) return EBTNodeResult::Failed;

    ElapsedTime = 0.f;
    CircleDuration = FMath::RandRange(1.0f, Enemy->CircleDuration);
    CircleDirection = FMath::RandBool() ? 1 : -1; // random left or right

    Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->WalkSpeed;

    return EBTNodeResult::InProgress;
}

void UBTTask_CirclePlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    ElapsedTime += DeltaSeconds;

    AAIController* AIC = OwnerComp.GetAIOwner();
    if (!AIC) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; }

    AEnemyBase* Enemy = Cast<AEnemyBase>(AIC->GetPawn());
    AActor* Target = Cast<AActor>(
        OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("TargetActor"))
    );
    if (!Enemy || !Target) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; }

    // Get perpendicular direction around player
    FVector ToPlayer = (Target->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal();
    FVector Right = FVector::CrossProduct(ToPlayer, FVector::UpVector) * CircleDirection;
    FVector CircleTarget = Enemy->GetActorLocation() + Right * 150.f;

    AIC->MoveToLocation(CircleTarget, 20.f, false);

    // Face player while circling
    Enemy->SetActorRotation(
        FRotationMatrix::MakeFromX(Target->GetActorLocation() - Enemy->GetActorLocation()).Rotator()
    );

    if (ElapsedTime >= CircleDuration)
    {
        // Restore chase speed before re-engaging
        Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->ChaseSpeed;
        OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName("AttacksRemaining"), 0);
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}

EBTNodeResult::Type UBTTask_CirclePlayer::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIC = OwnerComp.GetAIOwner();
    if (AIC)
    {
        AEnemyBase* Enemy = Cast<AEnemyBase>(AIC->GetPawn());
        if (Enemy)
            Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->ChaseSpeed;
    }
    return Super::AbortTask(OwnerComp, NodeMemory); // NodeMemory was missing
}