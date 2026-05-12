#include "AEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyAIController::AEnemyAIController()
{
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 1500.f;
    SightConfig->LoseSightRadius = 2500.f;  // was 1800, give much more room
    SightConfig->PeripheralVisionAngleDegrees = 90.f;
    SightConfig->SetMaxAge(10.f);
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    AIPerception->ConfigureSense(*SightConfig);
    AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
    AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    SetGenericTeamId(FGenericTeamId(1)); // enemies = team 1
    if (BehaviorTree)
        RunBehaviorTree(BehaviorTree);
}

void AEnemyAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    UE_LOG(LogTemp, Warning, TEXT("[PERCEPTION] Actor: %s | Sensed: %s"), 
        *Actor->GetName(), 
        Stimulus.WasSuccessfullySensed() ? TEXT("TRUE") : TEXT("FALSE"));

    ACharacter* Player = Cast<ACharacter>(Actor);
    if (!Player) return;

    ACharacter* EnemyChar = Cast<ACharacter>(GetPawn());

    if (Stimulus.WasSuccessfullySensed())
    {
        // Cancel any pending lose-target timer
        GetWorldTimerManager().ClearTimer(LoseTargetTimer);
        
        GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), Player);
        if (EnemyChar)
            EnemyChar->GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
    }
    else
    {
        // Don't clear immediately — wait 3 seconds of no sight before giving up
        ACharacter* CapturedPlayer = Player;
        ACharacter* CapturedEnemy = EnemyChar;
        GetWorldTimerManager().SetTimer(LoseTargetTimer, [this, CapturedEnemy]()
        {
            if (GetBlackboardComponent())
                GetBlackboardComponent()->ClearValue(FName("TargetActor"));
            if (CapturedEnemy)
                CapturedEnemy->GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
        }, 3.0f, false);
    }
}