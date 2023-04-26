// Copyright Epic Games, Inc. All Rights Reserved.

#include "CerberusCharacter.h"

#include "CerberusPawnExtensionComponent.h"
#include "Camera/CameraComponent.h"
#include "Cerberus/Cerberus.h"
#include "Cerberus/CerberusGameplayTags.h"
#include "Cerberus/CerberusLogChannels.h"
#include "Cerberus/AbilitySystem/CerberusAbilitySystemComponent.h"
#include "Cerberus/AbilitySystem/Abilities/CerberusGameplayAbility.h"
#include "Cerberus/Actors/CerberusPreviewActor.h"
#include "Cerberus/Enums/EquipableSlot.h"
#include "Cerberus/UniversalComponents/CerberusInventoryComponent.h"
#include "Cerberus/Player/CerberusPlayerController.h"
#include "Cerberus/Player/CerberusPlayerState.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Cerberus/Items/CerberusItem.h"
#include "Cerberus/UniversalComponents/CerberusHealthComponent.h"
#include "Cerberus/UniversalComponents/CerberusEquipmentComponent.h"

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
	EquipmentComponent = CreateDefaultSubobject<UCerberusEquipmentComponent>(TEXT("EquipmentComponent"));
	//TODO Set default skeletal meshes here
	////EquipmentComponent->DefaultMeshes.Add(/**Slot*/, CreateDefaultSubobject<USkeletalMesh>(TEXT("Name")));
	
	HelmetMesh = EquipmentComponent->EquippedMeshes.Add(EEquipableSlot::GIS_HELMET, CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HelmetMesh")));
	ChestMesh = EquipmentComponent->EquippedMeshes.Add(EEquipableSlot::GIS_CHEST, CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChestMesh")));
	LegsMesh = EquipmentComponent->EquippedMeshes.Add(EEquipableSlot::GIS_LEGS, CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegsMesh")));
	BackMesh = EquipmentComponent->EquippedMeshes.Add(EEquipableSlot::GIS_BACK, CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackMesh")));

	for (auto& PlayerMesh : EquipmentComponent->EquippedMeshes)
	{
		USkeletalMeshComponent* MeshComponent = PlayerMesh.Value;
		MeshComponent->SetupAttachment(GetMesh());
		MeshComponent->SetLeaderPoseComponent(GetMesh());
	}

	EquipmentComponent->EquippedMeshes.Add(EEquipableSlot::EIS_HEAD, GetMesh());
	
	// Initializing Pawn Extension Component which has nice utility with AbilitySystem
	PawnExtensionComponent = CreateDefaultSubobject<UCerberusPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	PawnExtensionComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnExtensionComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));

	// Initializing Health and other states related to health
	HealthComponent = CreateDefaultSubobject<UCerberusHealthComponent>(TEXT("HealthComponent"));

	// Initializing Inventory
	InventoryComponent = CreateDefaultSubobject<UCerberusInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->SetCapacity(20);

	InteractionCheckFrequency = 0.2f;
	InteractionCheckDistance = 1000.f;
}

void ACerberusCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	const FVector Location = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 1000000000.f);
	const FRotator Rotation = FRotator::ZeroRotator;
	// ACerberusPreviewActor* PreviewActor = GetWorld()->SpawnActor<ACerberusPreviewActor>(Location, Rotation);
	// PreviewActor->MasterMesh = GetMesh();
	// PreviewActor->SubMeshes = EquipmentComponent->EquippedMeshes;
	//PreviewActor->EquippedItems = EquipmentComponent->EquippedItems;
	//PreviewActor->Initialize();

	//Spawn preview actor for UI and inventory menu's
	if (ACerberusPreviewActor* PreviewActor = GetWorld()->SpawnActor<ACerberusPreviewActor>(PreviewActorClass, Location, Rotation))
	{
		PreviewActor->MasterMesh->SetSkeletalMeshAsset(GetMesh()->GetSkeletalMeshAsset());
		PreviewActor->SetSubMeshMap(EquipmentComponent->EquippedMeshes);

		//Pass preview actor to the equipment component to keep track of gear changes
		EquipmentComponent->PreviewActor = PreviewActor;
	}
}

ACerberusPlayerState* ACerberusCharacter::GetCerberusPlayerState() const
{
	return CastChecked<ACerberusPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}
ACerberusPlayerController* ACerberusCharacter::GetCerberusPlayerController() const
{
	return CastChecked<ACerberusPlayerController>(GetController(), ECastCheckedType::NullAllowed);
}


//////////////////////////////////////////////////////////////////////////
// Items
void ACerberusCharacter::UseItem(UCerberusItem* Item)
{
	//Tell the server to initiate using the item
	if (!HasAuthority() && Item)
	{
		ServerUseItem(Item);
	}

	if (HasAuthority())
	{
		if (InventoryComponent && !InventoryComponent->FindItem(Item))
		{
			return;
		}
	}

	if (Item)
	{
		Item->OnUse(this);
		Item->Use(this);
	}
}

void ACerberusCharacter::ServerUseItem_Implementation(UCerberusItem* Item)
{
	UseItem(Item);
}

bool ACerberusCharacter::ServerUseItem_Validate(UCerberusItem* Item)
{
	return true;
}



//////////////////////////////////////////////////////////////////////////
// Replication

void ACerberusCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

/**
 * @brief 
 * @param ChangedPropertyTracker 
 */
void ACerberusCharacter::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);
}

/* Setup client based functionality */
void ACerberusCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	PawnExtensionComponent->HandlePlayerStateReplicated();
	InitializeAbilitySystem();
}

void ACerberusCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	// Needed in case the PC wasn't valid when we Init-ed the ASC.
	if (ACerberusPlayerState* PS = GetCerberusPlayerState())
	{
		PS->GetCerberusAbilitySystemComponent()->RefreshAbilityActorInfo();
	}
}


//////////////////////////////////////////////////////////////////////////
///
UCerberusAbilitySystemComponent* ACerberusCharacter::GetCerberusAbilitySystemComponent() const
{
	return Cast<UCerberusAbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* ACerberusCharacter::GetAbilitySystemComponent() const
{
	return PawnExtensionComponent->GetCerberusAbilitySystemComponent();
}

void ACerberusCharacter::InitializeAttributes()
{
	if (GetCerberusAbilitySystemComponent() && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = GetCerberusAbilitySystemComponent()->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = GetCerberusAbilitySystemComponent()->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = GetCerberusAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			if (!GEHandle.WasSuccessfullyApplied())
				UE_LOG(LogCerberusAbilitySystem, Error, TEXT("ACerberusCharacter: Gameplay effect was not sucessfully applied"))
		}
	}
}

void ACerberusCharacter::GiveAbilities()
{
	if (HasAuthority() && GetCerberusAbilitySystemComponent() && !DefaultAbilities.IsEmpty())
	{
		for (TSubclassOf<UCerberusGameplayAbility>& Ability : DefaultAbilities)
		{
			GetCerberusAbilitySystemComponent()->GiveAbility(
				FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));
		}
		
	}
}

/* Setup server based functionality */
void ACerberusCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PawnExtensionComponent->HandleControllerChanged();
	InitializeAbilitySystem();
	
}

void ACerberusCharacter::UnPossessed()
{
	Super::UnPossessed();

	PawnExtensionComponent->HandleControllerChanged();
}

void ACerberusCharacter::InitializeAbilitySystem()
{
	if (PawnExtensionComponent->IsPawnReadyToInitialize())
	{
		ACerberusPlayerState* CerberusPS = GetCerberusPlayerState();
		check(CerberusPS)

		PawnExtensionComponent->InitializeAbilitySystem(CerberusPS->GetCerberusAbilitySystemComponent(), CerberusPS);
		
	}
}

void ACerberusCharacter::OnAbilitySystemInitialized()
{
	UCerberusAbilitySystemComponent* CerberusASC = GetCerberusAbilitySystemComponent();
	check(CerberusASC);

	HealthComponent->InitializeWithAbilitySystem(CerberusASC);
	InventoryComponent->InitializeWithAbilitySystem(CerberusASC);

	InitializeAttributes();

	if (HasAuthority())
	{
		GiveAbilities();

		UE_LOG(LogCerberusAbilitySystem, Log, TEXT("Character was given abilities by the %s"), *GetClientServerContextString());
	}
}

void ACerberusCharacter::OnAbilitySystemUninitialized()
{
	HealthComponent->UninitializeFromAbilitySystem();
	InventoryComponent->UninitializeFromAbilitySystem();
}


void ACerberusCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!HasAuthority())
	{
		if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
		{
			PerformInteractionCheck();
		}
	}

}



//////////////////////////////////////////////////////////////////////////
// Interaction

void ACerberusCharacter::PerformInteractionCheck()
{
	if (GetController() == nullptr)
		return;

	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();
	FVector EyesLoc;
	FRotator EyesRot;

	
	EyesRot = GetFollowCamera()->GetComponentRotation();
	EyesLoc = GetFollowCamera()->GetComponentLocation();

	// Get the distance between the character and the camera
	float ActorDistance = FVector::Dist(EyesLoc, GetActorLocation());

	// Start the line trace where the characters location but aligns to the crosshair
	FVector TraceStart = (EyesRot.Vector() * ActorDistance) + EyesLoc;
	FVector TraceEnd = (EyesRot.Vector() * InteractionCheckDistance) + TraceStart;

	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	// Debug
	// const FName TraceTag(this->GetName());
	// GetWorld()->DebugDrawTraceTag = TraceTag;
	// QueryParams.TraceTag = TraceTag;

	if (UCerberusInteractionComponent* Interactable = InteractionLineTrace(TraceStart, TraceEnd, QueryParams); Interactable != nullptr)
	{
		if (GetInteractable() != Interactable)
		{
			InteractionData.TraceStart = TraceStart;
			InteractionData.TraceEnd = TraceEnd;
			FoundNewInteractable(Interactable);
		}
	} else
	{
		if (GetInteractable() != nullptr)
		{
			InteractionData.TraceStart = FVector::Zero();
			InteractionData.TraceEnd = FVector::Zero();
			CouldntFindInteractable();
		}
	}

}

void ACerberusCharacter::ServerInteractableDataUpdate_Implementation(FInteractionData Data)
{
	InteractionData = Data;
}

void ACerberusCharacter::FoundNewInteractable(UCerberusInteractionComponent* Interactable)
{
	EndInteract();

	if (UCerberusInteractionComponent* OldInteractable = GetInteractable())
	{
		OldInteractable->EndFocus(this);
	}
	
	InteractionData.ViewedInteractionComponent = Interactable;
	ServerInteractableDataUpdate(InteractionData); //Notify server of change
	Interactable->BeginFocus(this);
}

UCerberusInteractionComponent* ACerberusCharacter::InteractionLineTrace(FVector TraceStart, FVector TraceEnd, FCollisionQueryParams QueryParams)
{
	FHitResult TraceHit;
	
	if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		if (TraceHit.GetActor())
		{
			// Check if we hit an interactable object
			if (UCerberusInteractionComponent* InteractionComponent = UCerberusInteractionComponent::FindInteractionComponent(TraceHit.GetActor()))
			{
				float Distance = (TraceStart - TraceHit.ImpactPoint).Size();
				if (InteractionComponent != GetInteractable() && Distance <= InteractionComponent->InteractionDistance)
				{
					return InteractionComponent;
				}
				if (Distance > InteractionComponent->InteractionDistance && GetInteractable())
				{
					return nullptr;
				}

				return GetInteractable();
			}
		}
	}
	
	return nullptr;
}

// void ACerberusCharacter::ServerPreformLineTrace_Implementation(FVector TraceStart, FVector TraceEnd)
// {
// 	FCollisionQueryParams QueryParams;
// 	QueryParams.AddIgnoredActor(this);
//
// 	InteractionLineTrace(TraceStart, TraceEnd, QueryParams);
// }
//
// bool ACerberusCharacter::ServerPreformLineTrace_Validate(FVector TraceStart, FVector TraceEnd)
// {
// 	// Perform any validation checks on the input parameters here
// 	return true;
// }


void ACerberusCharacter::CouldntFindInteractable()
{
	// Clear out interaction timer if its active
	if (GetWorldTimerManager().IsTimerActive(TimerHandle_Interact))
		GetWorldTimerManager().ClearTimer(TimerHandle_Interact);
	
	if(UCerberusInteractionComponent* Interactable = GetInteractable())
	{
		Interactable->EndFocus(this);

		if (InteractionData.bInteractHeld)
			EndInteract();
	}

	InteractionData.ViewedInteractionComponent = nullptr;
	ServerInteractableDataUpdate(InteractionData);
}

void ACerberusCharacter::BeginInteract()
{
	if (!HasAuthority())
	{
		ServerBeginInteract();
	}
		
	if (HasAuthority())
	{
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		
		//Validate interaction object
		if (const UCerberusInteractionComponent* InteractionComponent = InteractionLineTrace(InteractionData.TraceStart, InteractionData.TraceEnd, QueryParams))
		{
			if (InteractionComponent != GetInteractable())
			{
				return;
			}
		} else
		{
			return;
		}
	}

	// The server will only check if we're looking at an item once we begin interacting with it, to avoid the server running checks every tick
	// if (HasAuthority())
	// {
	// 	PerformInteractionCheck(true);
	// }
		
	
	InteractionData.bInteractHeld = true;

	if (UCerberusInteractionComponent* Interactable = GetInteractable())
	{
		Interactable->BeginInteract(this);

		if(FMath::IsNearlyZero(Interactable->InteractionTime))
		{
			Interact();
		}
		else
		{
			GetWorldTimerManager().SetTimer(TimerHandle_Interact, this, &ACerberusCharacter::Interact, Interactable->InteractionTime, false);
		}
	}
}

void ACerberusCharacter::EndInteract()
{
	if (!HasAuthority())
		ServerEndInteract();

	
	InteractionData.bInteractHeld = false;

	GetWorldTimerManager().ClearTimer(TimerHandle_Interact);

	if (UCerberusInteractionComponent* Interactable = GetInteractable())
	{
		Interactable->EndInteract(this);
	}
}

void ACerberusCharacter::ServerEndInteract_Implementation()
{
	EndInteract();
}

void ACerberusCharacter::ServerBeginInteract_Implementation()
{
	BeginInteract();
}

void ACerberusCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer((TimerHandle_Interact));

	if (UCerberusInteractionComponent* Interactable = GetInteractable())
	{
		Interactable->Interact(this);
	}
}

bool ACerberusCharacter::IsInteracting() const
{
	return GetWorldTimerManager().IsTimerActive(TimerHandle_Interact);
}

float ACerberusCharacter::GetRemainingInteractTime() const
{
	return GetWorldTimerManager().GetTimerRemaining(TimerHandle_Interact);
}



//////////////////////////////////////////////////////////////////////////
// Input
void ACerberusCharacter::SetupBinds()
{
	if (GetCerberusAbilitySystemComponent() && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "ECerberusAbilityInputID",
			static_cast<int32>(ECerberusAbilityInputID::Confirm), static_cast<int32>(ECerberusAbilityInputID::Cancel));

		GetCerberusAbilitySystemComponent()->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

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

	// Set up interaction
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACerberusCharacter::BeginInteract);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &ACerberusCharacter::EndInteract);
	
	SetupBinds();
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
