// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilityLabGameTypes.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemInterface.h"
#include "ActorComponents/FootStepsActorComponent.h"
#include "AbilitySystemProjectCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

class ULab_AbilitySystemComponent;
class ULab_AttributeSet;

class UGameplayAbility;
class UGameplayAbilitySpec;
class UGameplayEffect;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third-person character
 *  Implements a controllable orbiting camera
 */
UCLASS()
class AAbilitySystemProjectCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MouseLookAction;

public:

	/** Original Constructor */
	// AAbilitySystemProjectCharacter();

	/** Lab Changes */
	AAbilitySystemProjectCharacter(const FObjectInitializer& ObjectInitializer);

	bool ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffect, const FGameplayEffectContextHandle& EffectContext) const;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Udemy Ability Course */

	void GiveAbilities();
	void ApplyStartupEffects();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(EditDefaultsOnly)
	ULab_AbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(Transient)
	ULab_AttributeSet* AttributeSet;
	

	/** End Udemy Ability Course */

	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void JumpCustom(const FInputActionValue& Value);
	void StopJumpCustom(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	/** Jump Overrides */
	virtual void Landed(const FHitResult& Hit) override;
	
public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Ability Data

	UFUNCTION(BlueprintCallable, Category = "Ability System")
	FCharacterData GetCharacterData() const;

	UFUNCTION(BlueprintCallable, Category = "Ability System")
	void SetCharacterData(FCharacterData NewCharacterData);

	class UFootStepsActorComponent* GetFootStepsActorComponent() const;
	
protected:

	UPROPERTY(ReplicatedUsing=OnRep_MainCharacterData)
	FCharacterData CharacterData;

	UFUNCTION()
	void OnRep_MainCharacterData();
	
	virtual void InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication = false);

	UPROPERTY(EditDefaultsOnly, Category = "Ability System")
	class UMainCharacterData* MainCharacterDataAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Ability System")
	class UFootStepsActorComponent* FootStepsActorComponent;

	/** Gameplay Events */
	UPROPERTY(EditDefaultsOnly, Category = "Ability System")
	TSubclassOf<UGameplayEffect> GameplayEffect_Jump;


	UPROPERTY(EditDefaultsOnly, Category = "Ability Events")
	FGameplayTag JumpEventTag;


	UPROPERTY(EditDefaultsOnly, Category = "Ability Events")
	FGameplayTagContainer InAirTags;
};

