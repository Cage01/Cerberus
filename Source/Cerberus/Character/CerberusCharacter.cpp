// Copyright Epic Games, Inc. All Rights Reserved.

#include "CerberusCharacter.h"

#include "CerberusHealthComponent.h"
#include "CerberusPawnExtensionComponent.h"
#include "Camera/CameraComponent.h"
#include "Cerberus/CerberusGameplayTags.h"
#include "Cerberus/AbilitySystem/CerberusAbilitySystemComponent.h"
#include "Cerberus/AbilitySystem/Attributes/CerberusHealthSet.h"
#include "Cerberus/Player/CerberusPlayerController.h"
#include "Cerberus/Player/CerberusPlayerState.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ACerberusCharacter

ACerberusCharacter::ACerberusCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Setup Pawn Extension component (Contains AbilitySystemComponent)
	PawnExtension = CreateDefaultSubobject<UCerberusPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	PawnExtension->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnExtension->OnAbilitySystemUnitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));
	
	// Setting up Health
	HealthComponent = CreateDefaultSubobject<UCerberusHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	HealthComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);
	
	
}

ACerberusPlayerState* ACerberusCharacter::GetCerberusPlayerState() const
{
	return CastChecked<ACerberusPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

ACerberusPlayerController* ACerberusCharacter::GetCerberusPlayerController() const
{
	return CastChecked<ACerberusPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

UCerberusAbilitySystemComponent* ACerberusCharacter::GetCerberusAbilitySystemComponent() const
{
	return Cast<UCerberusAbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* ACerberusCharacter::GetAbilitySystemComponent() const
{
	return PawnExtension->GetCerberusAbilitySystemComponent();
}

/* Setup server based functionality */
void ACerberusCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PawnExtension->HandleControllerChanged();
}

void ACerberusCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	PawnExtension->HandleControllerChanged();
}

/* Setup client based functionality */
void ACerberusCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Will check if the pawn is ready to be initialized, and if it is, it will start up the ability system and fire off delegates.
	PawnExtension->HandlePlayerStateReplicated();

	if (PawnExtension->IsPawnReadyToInitialize())
	{
		ACerberusPlayerState* CerberusPS = GetPlayerState<ACerberusPlayerState>();
		check(CerberusPS);

		PawnExtension->InitializeAbilitySystem(CerberusPS->GetCerberusAbilitySystemComponent(), CerberusPS);
	}

}

void ACerberusCharacter::OnAbilitySystemInitialized()
{
	UCerberusAbilitySystemComponent* CerberusASC = GetCerberusAbilitySystemComponent();
	check(CerberusASC);
	
	HealthComponent->InitializeWithAbilitySystem(CerberusASC);	
	
}

void ACerberusCharacter::OnAbilitySystemUninitialized()
{
	HealthComponent->UninitialieFromAbilitySystem();
}

//@TODO The movement ability system isnt set up just yet. Will likely need to come back to it
void ACerberusCharacter::InitializeGameplayTags()
{
	// Movement system related tags - Not necessary just yet
	if (UCerberusAbilitySystemComponent* CerberusASC = GetCerberusAbilitySystemComponent())
	{
		const FCerberusGameplayTags& GameplayTags = FCerberusGameplayTags::Get();

		for (const TPair<uint8, FGameplayTag>& TagMapping : GameplayTags.MovementModeTagMap)
		{
			// ...etc
		}
	}
}

void ACerberusCharacter::OnDeathStarted(AActor* OwningActor)
{
	DisableMovementAndCollision();
}

void ACerberusCharacter::OnDeathFinished(AActor* OwningActor)
{
	//Calls this function on the next tick
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

void ACerberusCharacter::DisableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	//Disables collision
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	//@TODO Lyra disables movement in the MovementComponent -- Will need to look into
}

void ACerberusCharacter::DestroyDueToDeath()
{
	K2_OnDeathFinished();
	
	UninitAndDestroy();
}

void ACerberusCharacter::UninitAndDestroy()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.0f);

		if (UCerberusAbilitySystemComponent* CerberusASC = GetCerberusAbilitySystemComponent())
		{
			if (CerberusASC->GetAvatarActor() == this)
			{
				PawnExtension->UninitializeAbilitySystem();
			}
		}
	}

	SetActorHiddenInGame(true);
}


//////////////////////////////////////////////////////////////////////////
// Input
void ACerberusCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// @TODO: These inputs may become obsolete with the addition of the GameplayAbility and EnhancedInput classes.
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ACerberusCharacter::ToggleInventory);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACerberusCharacter::Interact);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ACerberusCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ACerberusCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ACerberusCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ACerberusCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACerberusCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACerberusCharacter::TouchStopped);
}

void ACerberusCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ACerberusCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ACerberusCharacter::ToggleInventory()
{
	// Check if inventory UI is open -> Perform correct action.
}

void ACerberusCharacter::Interact()
{
	//Interact with item in the world.
}

void ACerberusCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ACerberusCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ACerberusCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACerberusCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
