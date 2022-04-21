// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusPawnComponent.h"
#include "CerberusPawnExtensionComponent.generated.h"

class UCerberusPawnData;
class UCerberusAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCerberusDynamicMulticastDelegate);

/**
 * UCerberusPawnExtensionComponent
 *
 * Component used to add functionality to all Pawn classes.
 */

UCLASS()
class CERBERUS_API UCerberusPawnExtensionComponent : public UCerberusPawnComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCerberusPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category="Cerberus|Pawn")
	static UCerberusPawnExtensionComponent* FindPawnExtentionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCerberusPawnExtensionComponent>() : nullptr); }

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const UCerberusPawnData* InPawnData);

	UFUNCTION(BlueprintPure, Category="Cerberus|Pawn")
	UCerberusAbilitySystemComponent* GetCerberusAbilitySystemComponent() { return AbilitySystemComponent; }

	// Should be called by the owning pawn to become the avatar of the ability system.
	void InitializeAbilitySystem(UCerberusAbilitySystemComponent* InASC, AActor* InOwnerActor);

	// Should be called by the owning pawn to remove itself as the avatar of the ability system.
	void UninitializeAbilitySystem();

	// Should be called by the owning pawn when the pawn's controller changes.
	void HandleControllerChanged();

	// Should be called by the owning pawn when the player state has been replicated.
	void HandlePlayerStateReplicated();

	// Should be called by the owning pawn when the input component is setup.
	//void SetupPlayerInputComponent();

	// Call this anytime the pawn needs to check if it's ready to be initialized (pawn data assigned, possessed, etc..).
	bool CheckPawnReadyToInitialize();

	// Returns weather the pawn is ready to be initialized
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category= "Cerberus|Pawn", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsPawnReadyToInitialize() const { return bPawnReadyToInitialize; }

	// Register with the OnPawnReadyToInitialize delegate and broadcast if the condition is already met
	void OnPawnReadyToInitialize_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);

	// Register with the OnAbilitySystemInitialized delegate and broadcast if condition is already met.
	void OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);

	// Register with the OnAbilitySystemUnitialized delegate
	void OnAbilitySystemUnitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate);
	
protected:
	virtual void OnRegister() override;

	UFUNCTION()
	void OnRep_PawnData();

	// Delegate fired when pawn has everything needed for initialization.
	FSimpleMulticastDelegate OnPawnReadyToInitialize;

	UPROPERTY(BlueprintAssignable, Meta = (DisplayName = "On Pawn Ready To Initialize"))
	FCerberusDynamicMulticastDelegate BP_OnPawnReadyToInitialize;

	// Delegate fired when our pawn becomes the ability system's avatar actor
	FSimpleMulticastDelegate OnAbilitySystemInitialized;

	// Delegate fired when our pawn is removed as the ability system's avatar actor
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;
	
private:
	UPROPERTY(EditInstanceOnly, ReplicatedUsing = OnRep_PawnData, Category="Cerberus|Pawn")
	UCerberusPawnData* PawnData;
	
	UPROPERTY(EditAnywhere, Category="Cerberus|Abilities")
	UCerberusAbilitySystemComponent* AbilitySystemComponent;
	
	bool bPawnReadyToInitialize : true;
	
};
