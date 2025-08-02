// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "AbilitySystemLog.h"
#include "AbilitySystemComponent.h"

bool UBaseGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                              const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UBaseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Effect context to use by Effects
	FGameplayEffectContextHandle EffectContext = ActorInfo->AbilitySystemComponent->MakeEffectContext();

	for (auto GameplayEffect : OngoingEffectsToJustApplyOnStart)
	{
		if (!GameplayEffect.Get()) continue;

		if (UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
		{
			if (FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext); SpecHandle.IsValid())
			{
				const FActiveGameplayEffectHandle ActivateGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

				if (!ActivateGEHandle.WasSuccessfullyApplied())
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to apply Gameplay Effect! %s"), *GetNameSafe(GameplayEffect));
				}
				else
				{
					ABILITY_LOG(Log, TEXT("Applied Gameplay Effect! %s"), *GetNameSafe(GameplayEffect));
					UE_LOG(LogTemp, Log, TEXT("Applied Gameplay Effect! %s"), *GetNameSafe(GameplayEffect));
				}
			}
		}
		
	}
	// Handling removing of Effects
	if (IsInstantiated())
	{
		for (auto GameplayEffect : OngoingEffectsToRemoveOnEnd)
		{
			if (!GameplayEffect.Get()) continue;

		if (UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
			
			if (FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext); SpecHandle.IsValid())
			{
				const FActiveGameplayEffectHandle ActivateGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				if (!ActivateGEHandle.WasSuccessfullyApplied())
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to apply Gameplay Effect! %s"), *GetNameSafe(GameplayEffect));
				}
				else
				{
					RemoveOnendEffectHandles.Add(ActivateGEHandle);
					ABILITY_LOG(Log, TEXT("Applied Gameplay Effect! %s"), *GetNameSafe(GameplayEffect));
				}
			}
			
		}
	}
}

void UBaseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	if (IsInstantiated())
	{
		for (FActiveGameplayEffectHandle ActiveEffectHandle : RemoveOnendEffectHandles)
		{
			ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEffectHandle);
		}

		RemoveOnendEffectHandles.Empty();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

// Helper function to get Actor
AAbilitySystemProjectCharacter* UBaseGameplayAbility::GetAbilityActorFromActorInfo() const
{
	return Cast<AAbilitySystemProjectCharacter>(GetAvatarActorFromActorInfo());
}
