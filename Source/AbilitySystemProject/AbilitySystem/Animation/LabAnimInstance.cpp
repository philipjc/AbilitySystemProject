// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystemProjectCharacter.h"
#include "CharacterAnimDataAsset.h"
#include "LabAnimInstance.h"

UBlendSpace* ULabAnimInstance::GetLocomotionBlendSpace() const
{
	if (const AAbilitySystemProjectCharacter* Character = Cast<AAbilitySystemProjectCharacter>(GetOwningActor()))
	{
		const FCharacterData Data = Character->GetCharacterData();
		if (Data.CharacterAnimDataAsset)
		{
			return Data.CharacterAnimDataAsset->CharacterAnimationData.MovementAnimBlendSpace;
		}
	}
	// Or return Default
	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.MovementAnimBlendSpace : nullptr;
}

UAnimSequenceBase* ULabAnimInstance::GetIdleAnimation() const
{
	if (const AAbilitySystemProjectCharacter* Character = Cast<AAbilitySystemProjectCharacter>(GetOwningActor()))
	{
		FCharacterData Data = Character->GetCharacterData();
		
		if (Data.CharacterAnimDataAsset)
		{
			return Data.CharacterAnimDataAsset->CharacterAnimationData.IdleAnimAsset;
		}
	}
	// Or return Default
	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.IdleAnimAsset : nullptr;
}
