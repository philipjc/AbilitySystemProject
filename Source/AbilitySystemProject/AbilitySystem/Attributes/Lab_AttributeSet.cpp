// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/Lab_AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void ULab_AttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	// Movement
	else if (Data.EvaluatedData.Attribute == GetMaxMovementSpeedAttribute())
	{
		const ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningActor());
		
		if (!OwningCharacter) { return; }

		if (UCharacterMovementComponent* CharacterMovement = OwningCharacter->GetCharacterMovement())
		{
			const float MaximumCharacterMoveSpeed = GetMaxMovementSpeed();
			CharacterMovement->MaxWalkSpeed = MaximumCharacterMoveSpeed;
		}
	}
}

void ULab_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULab_AttributeSet, Health, OldValue);
}

void ULab_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULab_AttributeSet, MaxHealth, OldValue);
}

void ULab_AttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULab_AttributeSet, Stamina, OldValue);
}

void ULab_AttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULab_AttributeSet, MaxStamina, OldValue);
}

void ULab_AttributeSet::OnRep_MaxMovementSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULab_AttributeSet, MaxMovementSpeed, OldValue);
}

void ULab_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Health
	DOREPLIFETIME_CONDITION_NOTIFY(ULab_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULab_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	// Movement
	DOREPLIFETIME_CONDITION_NOTIFY(ULab_AttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULab_AttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}
