// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Jump_GameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemProjectCharacter.h"

/**
 * Constructor for UJump_GameplayAbility.
 *
 * This constructor initializes the gameplay ability with predefined policies:
 * - NetExecutionPolicy is set to LocalPredicted, indicating that this ability
 *   runs client-side as a predicted action without requiring server confirmation.
 * - InstancingPolicy is set to NonInstanced, meaning this ability does not require
 *   creating new instances during its execution.
 */
UJump_GameplayAbility::UJump_GameplayAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted; // No instance data to be kept... according to the tutorial.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

bool UJump_GameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;

	const AAbilitySystemProjectCharacter* Character = CastChecked<AAbilitySystemProjectCharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	if (!Character) return false;
	return Character->CanJump();
}

void UJump_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) return;

		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		
		// Do the jump

		if (ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed))
		{
			Character->Jump();
		}

	}
}
