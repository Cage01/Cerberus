// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusItem.h"
#include "UObject/Object.h"
#include "CerberusInventorySlot.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, DefaultToInstanced)
class CERBERUS_API UCerberusInventorySlot : public UObject
{
	GENERATED_BODY()

public:
	UCerberusInventorySlot();
	
	UPROPERTY(BlueprintReadWrite, Category="Cerberus|Inventory")
	UCerberusItem* Item;

	UPROPERTY(BlueprintReadWrite, Category="Cerberus|Inventory")
	int Size;
};
