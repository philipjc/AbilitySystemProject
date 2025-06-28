// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityLabGameTypes.h"
#include "Engine/DataAsset.h"
#include "MainCharacterData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ABILITYSYSTEMPROJECT_API UMainCharacterData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	FCharacterData CharacterData;
};
