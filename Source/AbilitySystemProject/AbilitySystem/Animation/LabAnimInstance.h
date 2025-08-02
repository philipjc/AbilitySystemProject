// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LabAnimInstance.generated.h"

/**
 * ULabAnimInstance is an animation instance class derived from UAnimInstance,
 * designed to handle character animation data and facilitate animation retrieval
 * for locomotion and idle states.
 */
UCLASS()
class ABILITYSYSTEMPROJECT_API ULabAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	// Default Data Set
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data Animation")
	class UCharacterAnimDataAsset* DefaultCharacterAnimDataAsset;

	/** Start: Getters to access the Data Set, and find the Animations associated */
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "Data Animation")
	class UBlendSpace* GetLocomotionBlendSpace() const;

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "Data Animation")
	class UAnimSequenceBase* GetIdleAnimation() const;

	/** End: Animation Getters */
	
};
