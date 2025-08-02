// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_Steps.h"
#include "AbilitySystemProjectCharacter.h"

void UAnimNotify_Steps::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	check(MeshComp)

	AAbilitySystemProjectCharacter* Character = MeshComp ? Cast<AAbilitySystemProjectCharacter>(MeshComp->GetOwner()) : nullptr;
	if (Character)
	{
		UFootStepsActorComponent* FootStepsActorComponent = Character->GetFootStepsActorComponent();
		if (FootStepsActorComponent)
		{
			FootStepsActorComponent->HandleFootStep(Foot);
		}
	}
}
