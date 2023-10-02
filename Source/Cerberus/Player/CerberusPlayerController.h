// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusPlayerState.h"
#include "Character/ALSPlayerController.h"
#include "CerberusPlayerController.generated.h"

struct FNotification;
class UInputMappingContext;
class UCerberusItem;
class UCerberusInventoryComponent;
/**
 * ACerberusPlayerController
 *
 * The base player controller class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class CERBERUS_API ACerberusPlayerController : public AALSPlayerController
{
	GENERATED_BODY()

public:
	ACerberusPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Cerberus|PlayerController")
	ACerberusPlayerState* GetCerberusPlayerState() const;

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

	/**
	 * Define the input values for the controller. These are defined by Data Asset (Input Mapping Context) files.
	 * Which are filled with an array of Data Asset (Input Action)'s. Be sure to add to this map when adding controls.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cerberus|Input")
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;
};
