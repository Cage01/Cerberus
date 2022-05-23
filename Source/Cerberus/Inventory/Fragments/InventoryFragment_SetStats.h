// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Cerberus/Inventory/CerberusInventoryItemDefinition.h"
#include "InventoryFragment_SetStats.generated.h"

/**
 * 
 */
UCLASS()
class UInventoryFragment_SetStats : public UCerberusInventoryItemFragment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TMap<FGameplayTag, int32> InitialItemStats;
	
public:
	virtual void OnInstanceCreated(UCerberusInventoryItemInstance* Instance) const override;

	int32 GetItemStatByTag(FGameplayTag Tag) const;
};
