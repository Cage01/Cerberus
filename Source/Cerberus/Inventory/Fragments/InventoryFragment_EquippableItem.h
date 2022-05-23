// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cerberus/Equipment/CerberusEquipmentDefinition.h"
#include "Cerberus/Inventory/CerberusInventoryItemDefinition.h"

#include "InventoryFragment_EquippableItem.generated.h"

/**
 * 
 */
UCLASS()
class CERBERUS_API UInventoryFragment_EquippableItem : public UCerberusInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Cerberus")
	TSubclassOf<UCerberusEquipmentDefinition> EquipmentDefinition;
};
