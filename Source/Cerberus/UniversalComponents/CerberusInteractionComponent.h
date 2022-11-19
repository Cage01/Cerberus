// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CerberusInteractionComponent.generated.h"

class ACerberusCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginInteract, ACerberusCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndInteract, ACerberusCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginFocus, ACerberusCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndFocus, ACerberusCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, ACerberusCharacter*, Character);


/**
 * UCerberusInteractionComponent
 *
 * Base class to interact with things in the world (lights, doors, items, loot, other players, etc)
 */
UCLASS(ClassGroup = (Custom), Meta=(BlueprintSpawnableComponent))
class CERBERUS_API UCerberusInteractionComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UCerberusInteractionComponent();

	void RefreshWidget();
	
	// Returns the Interaction component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "Cerberus|Interaction")
	static UCerberusInteractionComponent* FindInteractionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCerberusInteractionComponent>() : nullptr); }

	// Called on the client when the players interaction check trace begins/ends hitting the item
	void BeginFocus(ACerberusCharacter* Character);
	void EndFocus(ACerberusCharacter* Character);

	// Called on the client when the player begins/ends interaction with the item
	void BeginInteract(ACerberusCharacter* Character);
	void EndInteract(ACerberusCharacter* Character);

	// Client side interact
	void Interact(ACerberusCharacter* Character);

	UFUNCTION(BlueprintPure, Category="Cerberus|Interaction")
	float GetInteractPercentage();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cerberus|Interaction")
	float InteractionTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cerberus|Interaction")
	float InteractionDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cerberus|Interaction")
	FText NameText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cerberus|Interaction")
	FText ActionText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cerberus|Interaction")
	FText ActionType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cerberus|Interaction")
	bool bAllowMultipleInteractors;

	UFUNCTION(BlueprintCallable, Category="Cerberus|Interaction")
	void SetInteractableNameText(const FText& NewNameText);

	UFUNCTION(BlueprintCallable, Category="Cerberus|Interaction")
	void SetInteractableActionText(const FText& NewActionText);

	UFUNCTION(BlueprintCallable, Category="Cerberus|Interaction")
	void SetInteractableActionType(const FText& NewActionType);
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FOnBeginInteract OnBeginInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FOnEndInteract OnEndInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FOnBeginFocus OnBeginFocus;

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FOnEndFocus OnEndFocus;

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FOnInteract OnInteract;

protected:
	// Called whe the game starts
	virtual void Deactivate() override;

	bool CanInteract(ACerberusCharacter* Character) const;

	UPROPERTY()
	TArray<ACerberusCharacter*> Interactors;
};
