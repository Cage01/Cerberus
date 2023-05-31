#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CerberusLootTableRow.generated.h"

USTRUCT(BlueprintType)
struct FLootTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	//The item(s) to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	TArray<TSubclassOf<class UCerberusItem>> Items;

	//The percentage chance of spawning this item if we hit it on the roll
	UPROPERTY(EditDefaultsOnly, Category = "Loot", meta = (ClampMin = 0.001, ClampMax = 1.0))
	float Probability = 1.f;

};