// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityLabGameTypes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Steps.generated.h"

/**
 * UAnimNotify_Steps is a custom animation notify class used to handle footstep events in animations.
 * This notify triggers a specific footstep-related behavior for the owning character during the animation playback.
 */
UCLASS()
class ABILITYSYSTEMPROJECT_API UAnimNotify_Steps : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFoot Foot;
};
