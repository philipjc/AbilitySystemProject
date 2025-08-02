// Fill out your copyright notice in the Description page of Project Settings.


#include "FootStepsActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemProjectCharacter.h"
#include "AbilitySystem/PhysicalMaterials/LabPhysicalMaterial.h"

#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<int32> CVarShowFootsteps(
	TEXT("ShowDebugFootsteps"),
	0,
	TEXT("Draws debug info for footsteps")
	TEXT("	0: off/n")
	TEXT("	1: on/n"),
	ECVF_Cheat
	);

// Sets default values for this component's properties
UFootStepsActorComponent::UFootStepsActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UFootStepsActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UFootStepsActorComponent::HandleFootStep(const EFoot Footstep) const
{
	if (const AAbilitySystemProjectCharacter* Character = Cast<AAbilitySystemProjectCharacter>(GetOwner()))
	{
		const int32 DebugShowFootsteps = CVarShowFootsteps.GetValueOnAnyThread();
		
		if (const USkeletalMeshComponent* Mesh = Character->GetMesh())
		{
			FHitResult Hit;
			const FVector SocketLocation = Mesh->GetSocketLocation(Footstep == EFoot::Left ? LeftFootSocketName : RightFootSocketName);

			const FVector Location = SocketLocation + FVector::UpVector * 20;

			FCollisionQueryParams CollisionParams;
			CollisionParams.bReturnPhysicalMaterial = true;
			CollisionParams.AddIgnoredActor(Character);

			if (GetWorld()->LineTraceSingleByChannel(Hit, Location, Location + FVector::UpVector * -50.f, ECollisionChannel::ECC_WorldDynamic, CollisionParams))
			{
				if (Hit.bBlockingHit)
				{
					if (Hit.PhysMaterial.Get())
					{
						const ULabPhysicalMaterial* PhysicalMaterial = Cast<ULabPhysicalMaterial>(Hit.PhysMaterial.Get());

						if (PhysicalMaterial)
						{
							UGameplayStatics::PlaySoundAtLocation(this, PhysicalMaterial->GroundSurfaceSound, Location, 1.f);
						}

						if (DebugShowFootsteps > 0)
						{
							DrawDebugString(GetWorld(), Location, GetNameSafe(PhysicalMaterial), nullptr, FColor::Red, 4.f);
						}
					}

					if (DebugShowFootsteps > 0)
					{
						DrawDebugSphere(GetWorld(), Location, 14, 14, FColor::Green, false, 4.4);
					}
				}
				else
				{
					if (DebugShowFootsteps > 0)
					{
						DrawDebugLine(GetWorld(), Location, Location + FVector::UpVector * -50.f, FColor::Orange, false, 4, 0, 1);
					}
				}
			}
			else
			{
				if (DebugShowFootsteps > 0)
				{
					DrawDebugLine(GetWorld(), Location, Location + FVector::UpVector * -50.f, FColor::Orange, false, 4, 0, 1);
					DrawDebugSphere(GetWorld(), Location, 14, 14, FColor::Green, false, 4.f);
				}
			}
		}
	}
}