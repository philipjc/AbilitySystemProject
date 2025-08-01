// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystemProjectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Attributes/Lab_AttributeSet.h"
#include "AbilitySystem/Components/Lab_AbilitySystemComponent.h"
#include "ActorComponents/LabCharacterMovementComponent.h"
#include "DataAssets/MainCharacterData.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// AAbilitySystemProjectCharacter::AAbilitySystemProjectCharacter()
// {
	// Old Constructor	
// }

AAbilitySystemProjectCharacter::AAbilitySystemProjectCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<ULabCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
		// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// AbilitySystem
	AbilitySystemComponent = CreateDefaultSubobject<ULab_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<ULab_AttributeSet>(TEXT("AttributeSet"));

	// New Movement Component
	
}

void AAbilitySystemProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAbilitySystemProjectCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AAbilitySystemProjectCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAbilitySystemProjectCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

bool AAbilitySystemProjectCharacter::ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffect,
                                                               const FGameplayEffectContextHandle& EffectContext) const
{
	if (!GameplayEffect.Get()) return false;
	
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1.f, EffectContext);

	if (SpecHandle.IsValid())
	{
		const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
	}
	
	return false;
}

void AAbilitySystemProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAbilitySystemProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/** Ability Lab changed */
void AAbilitySystemProjectCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(MainCharacterDataAsset))
	{
		SetCharacterData(MainCharacterDataAsset->CharacterData);
	}
}

UAbilitySystemComponent* AAbilitySystemProjectCharacter::GetAbilitySystemComponent() const
{
	if (!IsValid(AbilitySystemComponent)) return nullptr;
	return AbilitySystemComponent;
}

void AAbilitySystemProjectCharacter::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		// Specify to use Data Asset.
		
		for (const auto Ability : CharacterData.Abilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));
		}
	}
}

void AAbilitySystemProjectCharacter::ApplyStartupEffects()
{
	if (HasAuthority() && IsValid(AttributeSet))
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		// Specify to use Data Asset.
		
		for (const auto Effect : CharacterData.Effects)
		{
			ApplyGameplayEffectToSelf(Effect, EffectContext);
		}
	}
}

void AAbilitySystemProjectCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// Give Abilities
	GiveAbilities();
	ApplyStartupEffects();
}

void AAbilitySystemProjectCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AAbilitySystemProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AAbilitySystemProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AAbilitySystemProjectCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AAbilitySystemProjectCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AAbilitySystemProjectCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AAbilitySystemProjectCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

FCharacterData AAbilitySystemProjectCharacter::GetCharacterData() const
{
	return CharacterData;
}

void AAbilitySystemProjectCharacter::SetCharacterData(FCharacterData NewCharacterData)
{
	CharacterData = NewCharacterData;

	InitFromCharacterData(CharacterData, false);
}

void AAbilitySystemProjectCharacter::OnRep_MainCharacterData()
{
	InitFromCharacterData(CharacterData, true);
}

void AAbilitySystemProjectCharacter::InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication)
{
}

void AAbilitySystemProjectCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// To, Replicate.
	
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAbilitySystemProjectCharacter, CharacterData);
}