// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "Jump_GameplayAbility.generated.h"

// Forward declarations in the .header files.
class UGameplayEffect;

/**
 * UJump_GameplayAbility is a gameplay ability class specialized for the jump mechanic.
 * This ability inherits from UBaseGameplayAbility and provides implementations for
 * activation checks and ability activation logic associated with the jump action.
 */
UCLASS()
class ABILITYSYSTEMPROJECT_API UJump_GameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

	UJump_GameplayAbility();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
