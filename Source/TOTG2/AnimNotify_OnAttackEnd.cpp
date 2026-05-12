#include "AnimNotify_OnAttackEnd.h"
#include "AtkComponent.h"

void UAnimNotify_OnAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (AActor* Owner = MeshComp->GetOwner())
    {
        if (UAtkComponent* Atk = Owner->FindComponentByClass<UAtkComponent>())
        {
            Atk->OnAttackEnd();
        }
    }
}