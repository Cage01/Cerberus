#pragma once

struct FItem
{
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FName ItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	TSubclassOf<class AActor> ItemActor; //Visual Representation, if it has one
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	UTexture2D ItemIcon;

	// Player Inventory Elements
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	bool CanEquip;

	UPROPERTY(BlueprintReadOnly, Category="Item")
	AActor* SpawnedItem;

	UPROPERTY(BlueprintReadOnly, Category="Item")
	int32 Quantity;
};

struct FMission
{
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Mission")
	FName MissionID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Mission")
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Mission")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Mission")
	TArray<FItem> Rewards;

	// Experience as well?
};
