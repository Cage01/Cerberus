// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "GameplayTagContainer.h"

#include "GameplayTagStack.generated.h"

struct FGameplayTagStackContainer;

/**
 * Represents one stack of a gameplay tag (tag + count)
 */
USTRUCT(BlueprintType)
struct FGameplayTagStack : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FGameplayTagStack()
	{}

	FGameplayTagStack(int32 InTagID, FGameplayTag InTag, int32 InStackCount)
		: TagID(InTagID)
		, Tag(InTag)
		, StackCount(InStackCount)
	{
	}

	FString GetDebugString() const;

private:
	friend FGameplayTagStackContainer;

	UPROPERTY()
	int32 TagID = -1;
	
	UPROPERTY()
	FGameplayTag Tag;

	UPROPERTY()
	int32 StackCount = 0;
	
	UPROPERTY()
	int32 MaxStackSize = 1;
};

/** Container of gameplay tag stacks */
USTRUCT(BlueprintType)
struct FGameplayTagStackContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	FGameplayTagStackContainer()
	//	: Owner(nullptr)
	{
	}

public:
	/** Creates a stack tag with the number stacks and the max size allowed to the tag */
	void InitializeStack(FGameplayTag Tag, int32 StackCount, int32 MaxStackSize);
	
	/** Adds a specified number of stacks to the tag (does nothing if StackCount is below 1) */
	void AddStack(FGameplayTag Tag, int32 StackCount);

	/** Removes a specified number of stacks from the tag (does nothing if StackCount is below 1) */
	void RemoveStack(FGameplayTag Tag, int32 StackCount);

	/** Sets the max stack size */
	void SetMaxStackSize(FGameplayTag Tag, int32 MaxStackSize);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	int32 GetStackCount(FGameplayTag Tag) const
	{
		return TagToCountMap.FindRef(Tag);
	}

	// Returns true if there is at least one stack of the specified tag
	bool ContainsTag(FGameplayTag Tag) const
	{
		return TagToCountMap.Contains(Tag);
	}

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FGameplayTagStack, FGameplayTagStackContainer>(Stacks, DeltaParms, *this);
	}

private:
	/** Replicated list of gameplay tag stacks */
	UPROPERTY()
	TArray<FGameplayTagStack> Stacks;

	/** @TODO I would like to use this for setting max number of stacks, but when the max is met or exceeded a new stack of the same type can be created */
	TMap<FGameplayTag, TArray<int32>> TagToIDsMap;
	
	/** Accelerated list of tag stacks for queries */
	TMap<FGameplayTag, int32> TagToCountMap;
};

template<>
struct TStructOpsTypeTraits<FGameplayTagStackContainer> : public TStructOpsTypeTraitsBase2<FGameplayTagStackContainer>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
