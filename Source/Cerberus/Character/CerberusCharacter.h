// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CerberusHealthComponent.h"
#include "Cerberus/AbilitySystem/CerberusAbilitySystemComponent.h"
#include "Cerberus/AbilitySystem/Attributes/CerberusAttributeSet.h"
#include "CerberusCharacter.generated.h"

UCLASS(config=Game)
class ACerberusCharacter : public ACharacter/*, public IAbilitySystemInterface*/
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;


	
public:
	ACerberusCharacter();
	//UCerberusAbilitySystemComponent* GetCerberusAbilitySystemComponent() const;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	//UFUNCTION(BlueprintCallable, Category="Cerberus|Character")
	//UCerberusAbilitySystemComponent* GetCerberusAbilitySystemComponent();
	//virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

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

	void ToggleInventory();
	void Interact();

	// Begins death sequence for the character (disables collision, movement, etc)
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	// Ends the death sequence for the character (detaches controller, destroys pawn, etc)
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// void DisableMovementAndCollision();
	// void DestroyDueToDeath();
	// void UninitAndDestroy();

	// virtual void OnAbilitySystemInitialized();
	// virtual void OnAbilitySystemUninitialized();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cerberus|Health", meta= (AllowPrivateAccess = "true"))
	UCerberusHealthComponent* HealthComponent;
	
};

