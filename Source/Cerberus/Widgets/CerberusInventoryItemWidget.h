// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CerberusInventoryItemWidget.generated.h"

class UCerberusItem;
/**
 * 
 */
UCLASS()
class CERBERUS_API UCerberusInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** This function will only execute successfully if the new item is valid, and the Item property has no current value */
	UFUNCTION(BlueprintCallable, Category="Cerberus|Inventory")
	void SetItem(UCerberusItem* NewItem = nullptr);
	
	UPROPERTY(BlueprintReadOnly, Category="Cerberus|Inventory", meta=(ExposeOnSpawn = true))
	UCerberusItem* Item;
};
