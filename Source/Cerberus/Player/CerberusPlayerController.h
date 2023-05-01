// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusPlayerState.h"
#include "GameplayTagContainer.h"
#include "ModularPlayerController.h"
#include "CerberusPlayerController.generated.h"

class UCerberusInventoryComponent;
class UCerberusAbilitySystemComponent;
/**
 * ACerberusPlayerController
 *
 * The base player controller class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class CERBERUS_API ACerberusPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	ACerberusPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Cerberus|PlayerController")
	ACerberusPlayerState* GetCerberusPlayerState() const;

	UFUNCTION(BlueprintCallable, Category="Cerberus|PlayerController")
	UCerberusAbilitySystemComponent* GetCerberusAbilitySystemComponent() const;

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientShowNotification(const FText& Message);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowNotification(const FText& Message);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowLootMenu(const UCerberusInventoryComponent* LootSource);

	UFUNCTION(BlueprintImplementableEvent)
	void HideLootMenu();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHitPlayer();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowIngameUI();
	
};
