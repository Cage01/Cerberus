// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusEquipableItem.h"
#include "CerberusGearItem.h"
#include "CerberusWeaponItem.generated.h"

class ACerberusWeaponActor;
/**
 * 
 */
UCLASS(Blueprintable)
class CERBERUS_API UCerberusWeaponItem : public UCerberusGearItem
{
	GENERATED_BODY()

public:
	// UPROPERTY(BlueprintReadOnly, Category= "Cerberus|Weapons")
	// ACerberusWeaponActor* WeaponActor;

	UFUNCTION(BlueprintCallable, Category="Cerberus|Weapons")
	void SpawnWeapon(bool bIsSheathed = false);

	UFUNCTION(BlueprintCallable, Category="Cerberus|Weapons")
	void DespawnWeapon();
};
