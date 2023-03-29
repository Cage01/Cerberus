// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CerberusItemTooltip.generated.h"

class UCerberusInventoryItemWidget;
/**
 * 
 */
UCLASS()
class CERBERUS_API UCerberusItemTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category= "Cerberus|Inventory|Tooltip", meta=(ExposeOnSpawn = true))
	UCerberusInventoryItemWidget* InventoryItemWidget;
	
};
