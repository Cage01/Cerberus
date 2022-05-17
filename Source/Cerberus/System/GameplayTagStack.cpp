// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayTagStack.h"

//////////////////////////////////////////////////////////////////////
// FGameplayTagStack

FString FGameplayTagStack::GetDebugString() const
{
	return FString::Printf(TEXT("%sx%d"), *Tag.ToString(), StackCount);
}

//////////////////////////////////////////////////////////////////////
// FGameplayTagStackContainer

void FGameplayTagStackContainer::InitializeStack(FGameplayTag Tag, int32 StackCount, int32 MaxStackSize)
{
	if (ContainsTag(Tag))
	{
		FFrame::KismetExecutionMessage(TEXT("Cannot initialize a tag that already exists"), ELogVerbosity::Warning);
		return;
	}

	FGameplayTagStack& NewStack = Stacks.Emplace_GetRef(1, Tag, StackCount);
	NewStack.MaxStackSize = MaxStackSize;
	MarkItemDirty(NewStack);
	TagToCountMap.Add(Tag, StackCount);
}

void FGameplayTagStackContainer::AddStack(FGameplayTag Tag, int32 StackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to AddStack"), ELogVerbosity::Warning);
		return;
	}

	if (StackCount > 0)
	{
		for (FGameplayTagStack& Stack : Stacks)
		{
			if (Stack.Tag == Tag)
			{
				const int32 NewCount = Stack.StackCount + StackCount;

				// Should only set if its within the MaxStackSize bounds
				if (NewCount <= Stack.MaxStackSize)
				{
					Stack.StackCount = NewCount;
					TagToCountMap[Tag] = NewCount;
					MarkItemDirty(Stack);
				}

				return;
			}
		}

		// Appears to create a new tag stack with the input count
		FGameplayTagStack& NewStack = Stacks.Emplace_GetRef(1, Tag, StackCount);
		MarkItemDirty(NewStack);
		TagToCountMap.Add(Tag, StackCount);
	}
}

void FGameplayTagStackContainer::RemoveStack(FGameplayTag Tag, int32 StackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to RemoveStack"), ELogVerbosity::Warning);
		return;
	}

	//@TODO: Should we error if you try to remove a stack that doesn't exist or has a smaller count?
	if (StackCount > 0)
	{
		for (auto It = Stacks.CreateIterator(); It; ++It)
		{
			FGameplayTagStack& Stack = *It;
			if (Stack.Tag == Tag)
			{
				if (Stack.StackCount <= StackCount)
				{
					It.RemoveCurrent();
					TagToCountMap.Remove(Tag);
					MarkArrayDirty();
				}
				else
				{
					const int32 NewCount = Stack.StackCount - StackCount;
					Stack.StackCount = NewCount;
					TagToCountMap[Tag] = NewCount;
					MarkItemDirty(Stack);
				}
				return;
			}
		}
	}
}

void FGameplayTagStackContainer::SetMaxStackSize(FGameplayTag Tag, int32 MaxStackSize)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to SetMaxStackSize"), ELogVerbosity::Warning);
		return;
	}

	for (FGameplayTagStack& Stack : Stacks)
	{
		if (Stack.Tag == Tag)
		{
			Stack.MaxStackSize = MaxStackSize;
			MarkItemDirty(Stack);
		}
	}
	
}

void FGameplayTagStackContainer::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const FGameplayTag Tag = Stacks[Index].Tag;
		TagToCountMap.Remove(Tag);
	}
}

void FGameplayTagStackContainer::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FGameplayTagStack& Stack = Stacks[Index];
		TagToCountMap.Add(Stack.Tag, Stack.StackCount);
	}
}

void FGameplayTagStackContainer::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		const FGameplayTagStack& Stack = Stacks[Index];
		TagToCountMap[Stack.Tag] = Stack.StackCount;
	}
}
