// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "CerberusItemSpawn.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Items), Blueprintable, Abstract)
class CERBERUS_API ACerberusItemSpawn : public ATargetPoint
{
	GENERATED_BODY()

public:
	ACerberusItemSpawn();


	UPROPERTY(EditAnywhere, Category = "Cerberus|Loot")
	class UDataTable* LootTable;

	/**Because pickups use a Blueprint base, we use a UPROPERTY to select it*/
	UPROPERTY(EditDefaultsOnly, Category = "Cerberus|Loot")
	TSubclassOf<class ACerberusPickup> PickupClass;

	/**Range used for generating respawn time*/
	UPROPERTY(EditDefaultsOnly, Category = "Cerberus|Loot")
	FIntPoint RespawnRange;

protected:

	FTimerHandle TimerHandle_RespawnItem;

	UPROPERTY()
	TArray<AActor*> SpawnedPickups;

	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnItem();

	//This is bound to the item being destroyed, so we can queue up another item to be spawned in
	UFUNCTION()
	void OnItemTaken(AActor* DestroyedActor);
};
