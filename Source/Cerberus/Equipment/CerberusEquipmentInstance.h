// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cerberus/Items/CerberusItemInstanceBase.h"

#include "CerberusEquipmentInstance.generated.h"

struct FCerberusEquipmentActorToSpawn;

/**
 * UCerberusEquipmentInstance
 *
 * A piece of equipment spawned and applied to a pawn
 */
UCLASS(BlueprintType, Blueprintable)
class CERBERUS_API UCerberusEquipmentInstance : public UCerberusItemInstanceBase
{
	GENERATED_BODY()

public:
	UCerberusEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category=Equipment)
	TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }

	virtual void SpawnEquipmentActors(const TArray<FCerberusEquipmentActorToSpawn>& ActorsToSpawn);
	virtual void DestroyEquipmentActors();
	
	virtual void OnEquipped();
	virtual void OnUnequipped();


protected:
	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnUnequipped"))
	void K2_OnUnequipped();


private:
	UPROPERTY(Replicated)
	TArray<AActor*> SpawnedActors;
};
