// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameInstances/Lab_GameInstance.h"
#include "AbilitySystemGlobals.h"

void ULab_GameInstance::Init()
{
	Super::Init();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
