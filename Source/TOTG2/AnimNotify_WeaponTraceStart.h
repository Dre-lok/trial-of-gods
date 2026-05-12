#pragma once
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_WeaponTraceStart.generated.h"

UCLASS()
class TOTG2_API UAnimNotify_WeaponTraceStart : public UAnimNotify
{
    GENERATED_BODY()
public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
