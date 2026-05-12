#include "BTTask_EnemyAttack.h"
#include "AEnemyBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTTask_EnemyAttack::UBTTask_EnemyAttack()
{
    NodeName = "Enemy Attack";
    bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    AEnemyBase* Enemy = Cast<AEnemyBase>(AIController->GetPawn());
    if (!Enemy) return EBTNodeResult::Failed;

    ACharacter* Target = Cast<ACharacter>(
        OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("TargetActor"))
    );
    if (!Target) return EBTNodeResult::Failed;

    float Distance = FVector::Dist(Enemy->GetActorLocation(), Target->GetActorLocation());
    if (Distance > AttackRange) return EBTNodeResult::Failed;

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    int32 Remaining = BB->GetValueAsInt(FName("AttacksRemaining"));

    // Initialize budget when empty
    if (Remaining <= 0)
        Remaining = FMath::RandRange(MinAttacks, MaxAttacks);

    // Decrement after we know we're committing to an attack
    Remaining--;
    BB->SetValueAsInt(FName("AttacksRemaining"), Remaining);

    UE_LOG(LogTemp, Warning, TEXT("[ATTACK] Budget after decrement: %d"), Remaining);

    // Face target
    FVector Direction = Target->GetActorLocation() - Enemy->GetActorLocation();
    Direction.Z = 0;
    Enemy->SetActorRotation(Direction.Rotation());

    // Play montage
    float MontageLength = 0.f;
    if (Enemy->AttackMontage)
    {
        UAnimInstance* AnimInst = Enemy->GetMesh()->GetAnimInstance();
        if (AnimInst)
            MontageLength = AnimInst->Montage_Play(Enemy->AttackMontage);
    }

    ActiveOwnerComp = &OwnerComp;
    float FinishDelay = FMath::Max(MontageLength, 0.5f);
    Enemy->GetWorldTimerManager().SetTimer(
        AttackFinishTimer, this, &UBTTask_EnemyAttack::FinishAttackTask, FinishDelay, false
    );

    return EBTNodeResult::InProgress;
}

void UBTTask_EnemyAttack::FinishAttackTask()
{
    if (!ActiveOwnerComp) return;

    UBlackboardComponent* BB = ActiveOwnerComp->GetBlackboardComponent();
    int32 Remaining = BB->GetValueAsInt(FName("AttacksRemaining"));


    // Remaining=0 means last attack just fired — fail out to StepBack
    EBTNodeResult::Type Result = (Remaining <= 0) ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;


    FinishLatentTask(*ActiveOwnerComp, Result);
    ActiveOwnerComp = nullptr;
}

void UBTTask_EnemyAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController)
    {
        AEnemyBase* Enemy = Cast<AEnemyBase>(AIController->GetPawn());
        if (Enemy)
        {
            Enemy->GetWorldTimerManager().ClearTimer(AttackFinishTimer);
            Enemy->OnAttackTraceStop();
        }
    }
    ActiveOwnerComp = nullptr;
}