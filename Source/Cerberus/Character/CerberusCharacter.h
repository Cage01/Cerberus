// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Cerberus/Items/CerberusItem.h"
#include <GameplayEffectTypes.h>
#include "Cerberus/AbilitySystem/Attributes/CerberusHealthSet.h"
#include "Cerberus/Items/CerberusEquipableItem.h"
#include "Cerberus/UniversalComponents/CerberusInteractionComponent.h"
#include "GameFramework/Character.h"
#include "CerberusCharacter.generated.h"

class UCerberusEquipmentComponent;
class UCerberusGearItem;
class ACerberusPickup;
class UCerberusHealthComponent;
class UCerberusInventoryComponent;
class UCerberusGameplayAbility;
class ACerberusPlayerController;
class ACerberusPlayerState;
class UCerberusPawnExtensionComponent;
class UCerberusAbilitySystemComponent;

USTRUCT()
struct FInteractionData
{
	GENERATED_BODY()

	FInteractionData()
	{
		ViewedInteractionComponent = nullptr;
		TraceStart = FVector::Zero();
		TraceEnd = FVector::Zero();
		LastInteractionCheckTime = 0.f;
		bInteractHeld = false;
	}
	
	//The current interactable we're viewing if there is one
	UPROPERTY()
	UCerberusInteractionComponent* ViewedInteractionComponent;

	UPROPERTY()
	FVector TraceStart;

	UPROPERTY()
	FVector TraceEnd;
	
	//The time when we last checked for an interactable
	UPROPERTY()
	float LastInteractionCheckTime;

	UPROPERTY()
	bool bInteractHeld;
};

/**
 * ACerberusCharacter
 *
 *	Base class used for (all?) humanoid characters. Will contain an InventoryComponent, HealthComponent, AbilitySystemComponent, StaticMesh, and the ability to interact.
 */

UCLASS(config=Game, meta=(ShortTooltip="The base character pawn class used by the project."))
class ACerberusCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACerberusCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;

	/** Player Controller */
	UFUNCTION(BlueprintCallable, Category="Cerberus|Character")
	ACerberusPlayerState* GetCerberusPlayerState() const;
	ACerberusPlayerController* GetCerberusPlayerController() const;

	/** Character Equipment */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cerberus|Character", meta = (AllowPrivateAccess = "true"))
	UCerberusEquipmentComponent* EquipmentComponent;
	
	/** Character Inventory */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cerberus|Character", meta = (AllowPrivateAccess = "true"))
	UCerberusInventoryComponent* InventoryComponent;

	/** Character Health */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cerberus|Character", meta = (AllowPrivateAccess = "true"))
	UCerberusHealthComponent* HealthComponent;

	/** Pawn Extension to handle unpredictable initialization flow from networking */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cerberus|Character", meta = (AllowPrivateAccess = "true"))
	UCerberusPawnExtensionComponent* PawnExtensionComponent;


	

	//////////////////////////////////////////////////////////////////////////
	// Items
	
	//TODO: Make this into an ability with GAS?
	UFUNCTION(BlueprintCallable, Category="Cerberus|Character|Items")
	void UseItem(UCerberusItem* Item);

	/**[Server] Use an item from our inventory*/
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUseItem(UCerberusItem* Item);
	
	/**
	 * @brief Initializes a default set of attributes for the character to have
	 */
	virtual void InitializeAttributes();
	/**
	 * @brief Give the character a default set of abilities to use
	 */
	virtual void GiveAbilities();
	/**
	 * @brief Sets up the binds defined in Cerberus.h for the Gameplay Ability System to use to then map to the correct input bindings and preform actions
	 */
	virtual void SetupBinds();

public:
	/** Ability System (May remove in the future)*/
	UFUNCTION(BlueprintCallable, Category="Cerberus|Character")
	UCerberusAbilitySystemComponent* GetCerberusAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//TODO: Probably want to move this to another location?
	UPROPERTY(BlueprintReadWrite, Category="Cerberus|Character")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Cerberus|Character")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Cerberus|Character")
	TArray<TSubclassOf<UCerberusGameplayAbility>> DefaultAbilities;
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;
	


	
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Character|Preview")
	TSubclassOf<ACerberusPreviewActor> PreviewActorClass;


	
	// Setting up character interaction 
	UPROPERTY(EditDefaultsOnly, Category="Cerberus|Character|Interaction")
	float InteractionCheckFrequency;

	UPROPERTY(EditDefaultsOnly, Category="Cerberus|Character|Interaction")
	float InteractionCheckDistance;
	
	void PerformInteractionCheck();

	UFUNCTION(Server, Reliable)
	void ServerInteractableDataUpdate(FInteractionData Data);
	
	void FoundNewInteractable(UCerberusInteractionComponent* Interactable);
	void CouldntFindInteractable();

	UCerberusInteractionComponent* InteractionLineTrace(FVector TraceStart, FVector TraceEnd, FCollisionQueryParams QueryParams);
	
	// UFUNCTION(Server, Reliable, WithValidation)
	// void ServerPreformLineTrace(FVector TraceStart, FVector TraceEnd);

	void BeginInteract();
	void EndInteract();

	UFUNCTION(Server, Reliable)
	void ServerBeginInteract();

	UFUNCTION(Server, Reliable)
	void ServerEndInteract();
	
	void Interact();

	// Information about the current state of the players interaction
	UPROPERTY()
	FInteractionData InteractionData;

	// Helper function to make getting the interactable faster
	FORCEINLINE UCerberusInteractionComponent* GetInteractable() const { return InteractionData.ViewedInteractionComponent; }

	FTimerHandle TimerHandle_Interact;

public:

	// True if we're interacting with an item that has an interaction time (Something that requires you to hold the button rather than press)
	bool IsInteracting() const;
	
	// Get the time we interact with the current interactable
	float GetRemainingInteractTime() const;

protected:
	virtual void Tick(float DeltaSeconds) override;
	
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
	
	virtual void InitializeAbilitySystem();
	virtual void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemUninitialized();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	
	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	

	/** Skeletal Mesh setup for equipment **/
	UPROPERTY(EditAnywhere, Category="Cerberus|Character")
	USkeletalMeshComponent* HelmetMesh;
	UPROPERTY(EditAnywhere, Category="Cerberus|Character")
	USkeletalMeshComponent* ChestMesh;
	UPROPERTY(EditAnywhere, Category="Cerberus|Character")
	USkeletalMeshComponent* LegsMesh;
	UPROPERTY(EditAnywhere, Category="Cerberus|Character")
	USkeletalMeshComponent* BackMesh;
	
};

