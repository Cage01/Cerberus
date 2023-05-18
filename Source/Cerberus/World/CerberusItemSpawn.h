// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/TargetPoint.h"
#include "CerberusItemSpawn.generated.h"

class UCerberusItem;

USTRUCT(BlueprintType)
struct FLootTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	//The item(s) to spawn
	UPROPERTY(EditDefaultsOnly, Category="Cerberus|Loot")
	TArray<TSubclassOf<UCerberusItem>> Items;

	//The percentage chance of spawning this item if we roll successfully
	UPROPERTY(EditDefaultsOnly, Category="Cerberus|Loot", meta=(ClampMin = 0.001, ClampMax = 1.0))
	float Probability = 1.f;
};

/**
 * 
 */
UCLASS()
class CERBERUS_API ACerberusItemSpawn : public ATargetPoint
{
	GENERATED_BODY()
	
};
