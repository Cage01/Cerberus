// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusInventoryManagerComponent.h"

//////////////////////////////////////////////////////////////////////
// FCerberusInventoryEntry
FString FCerberusInventoryEntry::GetDebugString() const
{
	return "";
}


//////////////////////////////////////////////////////////////////////
// FCerberusInventoryList
void FCerberusInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
}

void FCerberusInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
}

void FCerberusInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
}

UCerberusInventoryItemInstance* FCerberusInventoryList::AddEntry(
	TSubclassOf<UCerberusInventoryItemDefinition> ItemClass, int32 StackCount)
{
	return nullptr;
}

void FCerberusInventoryList::AddEntry(UCerberusInventoryItemInstance* Instance)
{
}

void FCerberusInventoryList::RemoveEntry(UCerberusInventoryItemInstance* Instance)
{
}

void FCerberusInventoryList::BroadcastChangeMessage(FCerberusInventoryEntry& Entry, int32 OldCount, int32 NewCount)
{
}

TArray<UCerberusInventoryItemInstance*> FCerberusInventoryList::GetAllItems() const
{
	return {};
}


//////////////////////////////////////////////////////////////////////
// UCerberusInventoryManagerComponent
UCerberusInventoryManagerComponent::UCerberusInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}
