// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Lab_AbilitySystemComponent.generated.h"

/**
 * @class ULab_AbilitySystemComponent
 * @brief A specialized implementation of the UAbilitySystemComponent provided by Unreal Engine's Gameplay Ability System.
 *
 * This customized ability system component extends the base functionality of UAbilitySystemComponent to support
 * project-specific gameplay mechanics. It is designed to be used in conjunction with the custom character class
 * and associated systems within the AbilitySystemProject.
 *
 * ULab_AbilitySystemComponent is responsible for managing and replicating gameplay abilities and effects.
 * It provides the core functionality for handling the interaction between abilities and attributes on actors.
 *
 * The class is utilized in the AbilitySystemProjectCharacter to enable advanced gameplay ability features,
 * supporting both client and server replication.
 */
UCLASS()
class ABILITYSYSTEMPROJECT_API ULab_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
};
