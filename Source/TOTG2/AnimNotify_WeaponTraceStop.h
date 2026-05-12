#pragma once
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_WeaponTraceStop.generated.h"

UCLASS()
class TOTG2_API UAnimNotify_WeaponTraceStop : public UAnimNotify
{
    GENERATED_BODY()
public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};