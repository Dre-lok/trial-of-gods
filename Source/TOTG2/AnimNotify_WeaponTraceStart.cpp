#include "AnimNotify_WeaponTraceStart.h"
#include "AtkComponent.h"
#include "AEnemyBase.h"

void UAnimNotify_WeaponTraceStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (AActor* Owner = MeshComp->GetOwner())
    {
        if (UAtkComponent* Atk = Owner->FindComponentByClass<UAtkComponent>())
        {
            Atk->OnWeaponTraceStart();
            return;
        }

        if (AEnemyBase* Enemy = Cast<AEnemyBase>(Owner))
        {
            Enemy->OnAttackTraceStart();
        }
    }
}