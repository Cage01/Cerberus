// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusPlayerState.h"
#include "CerberusPlayerController.generated.h"

struct FNotification;
class UCerberusItem;
class UCerberusInventoryComponent;
class UCerberusAbilitySystemComponent;
/**
 * ACerberusPlayerController
 *
 * The base player controller class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class CERBERUS_API ACerberusPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACerberusPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Cerberus|PlayerController")
	ACerberusPlayerState* GetCerberusPlayerState() const;

	UFUNCTION(BlueprintCallable, Category="Cerberus|PlayerController")
	UCerberusAbilitySystemComponent* GetCerberusAbilitySystemComponent() const;

	/**[Client] Tells blueprints to activate and display a notification */
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientShowNotification(FNotification Notification);

	/** Basic notification that will be displayed on a players screen */
	UFUNCTION(BlueprintImplementableEvent)
	void ShowNotification(FNotification Notification);


	/** An event to trigger a menu widget that contains all the items in a specified InventoryComponent */
	UFUNCTION(BlueprintImplementableEvent)
	void ShowLootMenu(const UCerberusInventoryComponent* LootSource);

	/** An event to trigger an active loot menu widget to be removed from the screen */
	UFUNCTION(BlueprintImplementableEvent)
	void HideLootMenu();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHitPlayer();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowIngameUI();
	
};
