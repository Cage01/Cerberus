// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Cerberus/System/GameplayTagStack.h"
#include "UObject/Object.h"
#include "CerberusInventoryItemDefinition.h"
#include "CerberusInventoryItemInstance.generated.h"


/**
 * UCerberusInventoryItemInstance
 *
 * Defines an object that sits in an inventory
 */
UCLASS(BlueprintType)
class UCerberusInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	UCerberusInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	//~End of UObject interface

	/** Adds a specified number of stacks to the tag (does nothing if StackCount is below 1) */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Cerberus|Inventory")
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	/** Removes a specified number of stacks to the tag (does nothing if StackCount is below 1) */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Cerberus|Inventory")
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	/** Returns the stack count of the specified tag (or 0 if the tag is not present) */
	UFUNCTION(BlueprintCallable, Category="Cerberus|Inventory")
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category="Cerberus|Inventory")
	bool HasStatTag(FGameplayTag Tag) const;

protected:
	//@TODO Not sure if I should include this here or the struct that handles the Gameplay Tag Stacks
	UPROPERTY(EditDefaultsOnly, Category="Cerberus|Inventory")
	int32 MaxStatTagStackSize;

private:
	void SetItemDef(TSubclassOf<UCerberusInventoryItemDefinition> InDef);

	friend struct FCerberusInventoryList;
	
private:
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	// The item definition
	UPROPERTY(Replicated)
	TSubclassOf<UCerberusInventoryItemDefinition> ItemDef;
};
