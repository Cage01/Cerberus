// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CerberusInventoryManagerComponent.generated.h"



class UCerberusInventoryItemDefinition;
class UCerberusInventoryItemInstance;
class UCerberusInventoryManagerComponent;
struct FCerberusInventoryList;

/** A message when an item is added to the inventory */
USTRUCT(BlueprintType)
struct FCerberusInventoryChangeMessage
{
	GENERATED_BODY()

	//@TODO: Tag based names+owning actors for inventories instead of directly exposing the component?
	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	UActorComponent* InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	UCerberusInventoryItemInstance* Instance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 NewCount = 0;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 Delta = 0;
};

/** A single entry in an inventory */
USTRUCT(BlueprintType)
struct FCerberusInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FCerberusInventoryEntry()
	{}

	FString GetDebugString() const;

private:
	friend FCerberusInventoryList;
	friend UCerberusInventoryManagerComponent;

	UPROPERTY()
	UCerberusInventoryItemInstance* Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};

/** List of inventory items */
USTRUCT(BlueprintType)
struct FCerberusInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FCerberusInventoryList()
		: OwnerComponent(nullptr)
	{
	}

	FCerberusInventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	TArray<UCerberusInventoryItemInstance*> GetAllItems() const;

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FCerberusInventoryEntry, FCerberusInventoryList>(Entries, DeltaParms, *this);
	}

	UCerberusInventoryItemInstance* AddEntry(TSubclassOf<UCerberusInventoryItemDefinition> ItemClass, int32 StackCount);
	void AddEntry(UCerberusInventoryItemInstance* Instance);

	void RemoveEntry(UCerberusInventoryItemInstance* Instance);

private:
	void BroadcastChangeMessage(FCerberusInventoryEntry& Entry, int32 OldCount, int32 NewCount);

private:
	friend UCerberusInventoryManagerComponent;

private:
	// Replicated list of items
	UPROPERTY()
	TArray<FCerberusInventoryEntry> Entries;

	UPROPERTY()
	UActorComponent* OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FCerberusInventoryList> : public TStructOpsTypeTraitsBase2<FCerberusInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};

/**
 * Manages an inventory
 */
UCLASS(BlueprintType)
class UCerberusInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCerberusInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
