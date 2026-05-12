#pragma once
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_OnAttackEnd.generated.h"

UCLASS()
class TOTG2_API UAnimNotify_OnAttackEnd : public UAnimNotify
{
    GENERATED_BODY()
public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};