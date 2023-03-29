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
	UPROPERTY(BlueprintReadOnly, Category="Cerberus|Inventory", meta=(ExposeOnSpawn = true))
	UCerberusItem* Item;
};
