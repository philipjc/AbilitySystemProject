// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "LabPhysicalMaterial.generated.h"

UCLASS()
class ABILITYSYSTEMPROJECT_API ULabPhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PhysicalMaterial")
	class USoundBase* GroundSurfaceSound = nullptr;
	
};
