// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Cerberus/Inventory/Items/CerberusItem.h"
#include <GameplayEffectTypes.h>
#include "CerberusHealthComponent.h"
#include "Cerberus/AbilitySystem/Attributes/CerberusHealthSet.h"
#include "GameFramework/Character.h"
#include "CerberusCharacter.generated.h"

class UCerberusInventoryComponent;
class UCerberusGameplayAbility;
class ACerberusPlayerController;
class ACerberusPlayerState;
//class UCerberusHealthComponent;
class UCerberusPawnExtensionComponent;
class UCerberusAbilitySystemComponent;

UCLASS(config=Game, meta=(ShortTooltip="The base character pawn class used by the project."))
class ACerberusCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Character Inventory */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cerberus|Character", meta = (AllowPrivateAccess = "true"))
	UCerberusInventoryComponent* InventoryComponent;

	/** Character Inventory */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cerberus|Character", meta = (AllowPrivateAccess = "true"))
	UCerberusHealthComponent* HealthComponent;

	/** Pawn Extension to handle unpredictable initialization flow from networking */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cerberus|Character", meta = (AllowPrivateAccess = "true"))
	UCerberusPawnExtensionComponent* PawnExtensionComponent;

	// /** GAS AbilitySystemComponent */
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cerberus|Character", meta = (AllowPrivateAccess = "true"))
	// UCerberusAbilitySystemComponent* AbilitySystemComponent;

	//TODO: Move health to its own component like before
	// UPROPERTY()
	// const UCerberusHealthSet* Health;

	
public:
	ACerberusCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category="Cerberus|Character")
	ACerberusPlayerState* GetCerberusPlayerState() const;

	UFUNCTION(BlueprintCallable, Category="Cerberus|Character")
	UCerberusAbilitySystemComponent* GetCerberusAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//TODO: Probably want to move this to another location?
	UPROPERTY(BlueprintReadWrite, Category="Cerberus|Character")
	FGameplayTagContainer GameplayTags;
	
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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Cerberus|Character")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Cerberus|Character")
	TArray<TSubclassOf<UCerberusGameplayAbility>> DefaultAbilities;

	//TODO: Make this into an ability with GAS?
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Cerberus|Inventory")
	void UseItem(UCerberusItem* Item);
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;
	

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
protected:
	
	
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


protected:
	virtual void InitializeAbilitySystem();
	virtual void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemUninitialized();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	
	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	
};

