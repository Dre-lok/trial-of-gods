#include "AnimNotify_WeaponTraceStop.h"
#include "AtkComponent.h"
#include "AEnemyBase.h"

void UAnimNotify_WeaponTraceStop::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (AActor* Owner = MeshComp->GetOwner())
    {
        if (UAtkComponent* Atk = Owner->FindComponentByClass<UAtkComponent>())
        {
            Atk->OnWeaponTraceStop();
            return;
        }

        if (AEnemyBase* Enemy = Cast<AEnemyBase>(Owner))
        {
            Enemy->OnAttackTraceStop();
        }
    }
}