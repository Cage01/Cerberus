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
 * A component to make things interactable in the world
 * Add this to any item or object that a player can interact with, such as picking up weapons, opening doors, and etc.
 */
UCLASS(ClassGroup = (Custom), Meta=(BlueprintSpawnableComponent))
class CERBERUS_API UCerberusInteractionComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UCerberusInteractionComponent();

	void RefreshWidget();
	
	/** A static function that will attempt to return the inventory component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Cerberus|Interaction")
	static UCerberusInteractionComponent* FindInteractionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCerberusInteractionComponent>() : nullptr); }

	/** Called on the client when the players interaction check trace begins colliding the item */
	void BeginFocus(ACerberusCharacter* Character);
	/** Called on the client when the players interaction check trace ends colliding the item */
	void EndFocus(ACerberusCharacter* Character);

	/** Called on the client when the player begins interaction with the item */
	void BeginInteract(ACerberusCharacter* Character);
	/** Called on the client when the player nds interaction with the item */
	void EndInteract(ACerberusCharacter* Character);

	/** Client side interact */
	void Interact(ACerberusCharacter* Character);

	/** Will get a value between 0 and 1 to display on a UI. The value is based on the total time required to interact and the amount of time that has passed */
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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cerberus|Items")
	UTexture2D* InteractionThumbnail;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cerberus|Items")
	int32 InteractionQuantity;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cerberus|Interaction")
	bool bAllowMultipleInteractors;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cerberus|Interaction")
	int32 OutlineColorValue;

	UFUNCTION(BlueprintCallable, Category="Cerberus|Interaction")
	FORCEINLINE void SetIsInteractable(bool NewIsInteractable) { bIsInteractable = NewIsInteractable; }

	UFUNCTION(BlueprintCallable, Category="Cerberus|Interaction")
	void SetInteractableNameText(const FText& NewNameText);

	UFUNCTION(BlueprintCallable, Category="Cerberus|Interaction")
	void SetInteractableActionText(const FText& NewActionText);
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cerberus|Interaction")
	bool bIsInteractable;

private:
	UFUNCTION()
	void ToggleOutline(int32 Value) const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
