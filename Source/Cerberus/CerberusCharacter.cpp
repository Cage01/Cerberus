// Copyright Epic Games, Inc. All Rights Reserved.

#include "CerberusCharacter.h"

#include "CerberusGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Core/Combat/Projectile.h"
#include "GameFramework/GameModeBase.h"


//////////////////////////////////////////////////////////////////////////
// ACerberusCharacter

ACerberusCharacter::ACerberusCharacter()
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

	// Set Health Values
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	//@TODO : and other weapon types?
	//Initialize projectile class
	ProjectileClass =  AProjectile::StaticClass();

	FireRate = 0.25f;
	bIsFiringWeapon = false;
}

/** Property replication */
void ACerberusCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate current health
	DOREPLIFETIME(ACerberusCharacter, CurrentHealth);
}


//////////////////////////////////////////////////////////////////////////
// Health
void ACerberusCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void ACerberusCharacter::OnHealthUpdate()
{
	//@TODO : Update progress bars
	//Client-specific functionality
	if (IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("Health: %f"), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("Player Died"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	//Server-specific functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining"), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	}
}

void ACerberusCharacter::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		//TODO: Keep track of player health in cache or database ?
		
		OnHealthUpdate();
	}
}

float ACerberusCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageApplied = CurrentHealth - DamageAmount;
	SetCurrentHealth(damageApplied);

	if (CurrentHealth <= 0)
	{
		Die(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	return damageApplied;
}

bool ACerberusCharacter::Die(float DamageAmount, FDamageEvent const& DamageEvent, AController* Attacker,
	AActor* DamageCauser)
{
	if(!CanDie())
	{
		return false;
	}

	/* Fallback to default DamageType if none is specified */
	UDamageType const* const DamageType = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	Attacker = GetDamageInstigator(Attacker, *DamageType);

	/* Notify the gamemode we got killed for game state */
	AController* Victim = Controller ? Controller : Cast<AController>(GetOwner());
	GetWorld()->GetAuthGameMode<ACerberusGameMode>()->Killed(Attacker, Victim, this, DamageType);

	OnDeath();
	return true;
}

bool ACerberusCharacter::CanDie() const
{
	/* Check if character is already dying, destroyed or if we have authority */
	if (bIsDying || IsValid(this) || !HasAuthority() || GetWorld()->GetAuthGameMode() == nullptr)
		return false;
	
	return true;
}

void ACerberusCharacter::OnDeath()
{
	if (IsLocallyControlled())
	{
		if (bIsDying)
			return;

		bIsDying = true;

		//Disconnect controller from pawn
		
		//Preform rag-doll physics?
		SetRagdollPhysics();

		//Destroy pawn

	}
}

void ACerberusCharacter::SetRagdollPhysics()
{
	
}


//////////////////////////////////////////////////////////////////////////
// Input

void ACerberusCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

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

	// Handle firing projectiles
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACerberusCharacter::StartFire);
}

void ACerberusCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ACerberusCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
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

void ACerberusCharacter::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		// StopFire is called when the time with the length of FireRate finishes -- bIsFiringWeapon will be set to false automatically
		//@TODO : this does not account for automatic firing / Seems to be good for semi-automatic weapons
		World->GetTimerManager().SetTimer(FiringTimer, this, &ACerberusCharacter::StopFire, FireRate, false);
		HandleFire();
	}
}

void ACerberusCharacter::StopFire()
{
	bIsFiringWeapon = false;
}

void ACerberusCharacter::HandleFire_Implementation()
{
	FVector spawnLocation = GetActorLocation() + (GetControlRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
	FRotator spawnRotation = GetControlRotation();

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	AProjectile* spawnedProjectile = GetWorld()->SpawnActor<AProjectile>(spawnLocation, spawnRotation, spawnParameters);
}
