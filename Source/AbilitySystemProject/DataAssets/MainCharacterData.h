// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityLabGameTypes.h"
#include "Engine/DataAsset.h"
#include "MainCharacterData.generated.h"

/**
 * A data asset class representing the main character's data in the game.
 * This class is used to store and manage information specific to the main character,
 * including abilities, effects, and animations.
 *
 * Inherits from UDataAsset, making it a reusable asset that can be easily edited
 * and applied throughout the project.
 */
UCLASS(BlueprintType, Blueprintable)
class ABILITYSYSTEMPROJECT_API UMainCharacterData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	FCharacterData CharacterData;
};
