#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AEnemyAIController.generated.h"

UCLASS()
class TOTG2_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

    public:
        AEnemyAIController();
        virtual void OnPossess(APawn* InPawn) override;

        UPROPERTY(EditDefaultsOnly, Category="AI")
        UBehaviorTree* BehaviorTree;

        UPROPERTY(EditDefaultsOnly, Category="AI")
        float PatrolSpeed = 200.f;

        UPROPERTY(EditDefaultsOnly, Category="AI")
        float ChaseSpeed = 500.f;

    private:
        UPROPERTY()
        UAIPerceptionComponent* AIPerception;

        UPROPERTY()
        UAISenseConfig_Sight* SightConfig;

        FTimerHandle LoseTargetTimer;

        UFUNCTION()
        void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};