// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusInventoryComponent.h"
#include "Cerberus/CerberusLogChannels.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

#define LOCTEXT_NAMESPACE "Inventory"
// Sets default values for this component's properties
UCerberusInventoryComponent::UCerberusInventoryComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	
	Capacity = 20;
	AbilitySystemComponent = nullptr;

	// Component must be replicate to replicate its subobjects
	SetIsReplicatedByDefault(true);

	// New method of SubObject replication in 5.1
	bReplicateUsingRegisteredSubObjectList = true;
}

void UCerberusInventoryComponent::OnRep_ItemsUpdated() const
{
	OnInventoryUpdated.Broadcast();
}


void UCerberusInventoryComponent::OnUnregister()
{
	Super::OnUnregister();
}

UCerberusItem* UCerberusInventoryComponent::AddItem(UCerberusItem* Item)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UCerberusItem* NewItem = NewObject<UCerberusItem>(GetOwner(), Item->GetClass());
		NewItem->SetQuantity(Item->GetQuantity());
		NewItem->OwningInventory = this;
		NewItem->AddedToInventory(this);
		Items.Add(NewItem);
		NewItem->MarkDirtyForReplication();

		return NewItem;
	}

	return nullptr;
}


FItemAddResult UCerberusInventoryComponent::TryAddItem_Internal(UCerberusItem* Item)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		const int32 AddAmount = Item->GetQuantity();
		int32 TotalAdded = 0;

		// Will not add, if adding the item exceeds the available capacity
		if (Items.Num() + 1 > GetCapacity())
		{
			return FItemAddResult::AddedNone(AddAmount, LOCTEXT("InventoryCapacityFullText", "Couldn't add item to your Inventory. Inventory is full"));
		}

		if (Item->bStackable)
		{
			if (UCerberusItem* FoundItem = FindItem(Item))
			{
				if (FoundItem->GetStackRemaining() > 0)
				{
					const int32 ActualAddAmount = FMath::Min(AddAmount, FoundItem->GetStackRemaining());
					FoundItem->AddQuantity(ActualAddAmount);
					//Item->SubtractQuantity(ActualAddAmount);
					TotalAdded += ActualAddAmount;
					//Item->SetQuantity(Item->GetQuantity() - ActualAddAmount);
					FItemAddResult Result = TryAddItem_Internal(Item);
					//FText ErrorText = FText::Format(LOCTEXT("InventoryStackErrorText", "Couldn't add entire stack of {ItemName} to inventory."), Item);

					// All items have been added to existing stacks
					// if (Item->GetQuantity() == 0)
					// {
					// 	return FItemAddResult::AddedAll()
					// } 
				}
			} 
		}
		
		AddItem(Item);
		return FItemAddResult::AddedAll(Item->GetQuantity());
	}
	return FItemAddResult::AddedAll(Item->GetQuantity());
}


void UCerberusInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCerberusInventoryComponent, Items, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCerberusInventoryComponent, DefaultItems, COND_None, REPNOTIFY_Always);
}

bool UCerberusInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	
	check(Channel)
	check(Bunch)
	check(RepFlags)
	
	if (Channel->KeyNeedsToReplicate(0, ReplicatedItemsKey))
	{
		for (auto& Item : Items)
		{
			if (Channel->KeyNeedsToReplicate(Item->GetUniqueID(), Item->RepKey))
			{
				bWroteSomething |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
			}
		}
	}

	return bWroteSomething;
}

FItemAddResult UCerberusInventoryComponent::TryAddItem(UCerberusItem* Item)
{
	return TryAddItem_Internal(Item);
}

FItemAddResult UCerberusInventoryComponent::TryAddItemFromClass(TSubclassOf<UCerberusItem> ItemClass, const int32 Quantity)
{
	UCerberusItem* Item = NewObject<UCerberusItem>(GetOwner(), ItemClass);
	Item->SetQuantity(Quantity);
	return TryAddItem_Internal(Item);
}

// UCerberusItem* UCerberusInventoryComponent::AddItem(TSubclassOf<UCerberusItem> ItemClass)
// {
// 	if (ItemClass && !ItemClass->HasAnyClassFlags(CLASS_Abstract))
// 	{
// 		AActor* lOwner = GetOwner();
// 			
// 		checkf(lOwner != nullptr, TEXT("Invalid Inventory Owner"));
// 		checkf(lOwner->HasAuthority(), TEXT("Called without Authority!"));
// 		// Use the Actor as the Outer.
// 		UCerberusItem* NewItem = NewObject<UCerberusItem>(lOwner, ItemClass);
// 		checkf(NewItem != nullptr, TEXT("Unable to create inventory slot"));
// 		
// 		// Set immediately for replication
// 		NewItem->OwningInventory = this;
// 		NewItem->ID = Items.Num();
// 		Items.Add(NewItem);
//
// 		//Replicate object
// 		AddReplicatedSubObject(NewItem);
//
// 		OnItemAdded.Broadcast(this, NewItem, NewItem->ID);
// 		return NewItem;
// 	}
// 	
// 	return nullptr;
// }



bool UCerberusInventoryComponent::RemoveItem(UCerberusItem* Item)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		if (Item)
		{
			Items.RemoveSingle(Item);
			ReplicatedItemsKey++;

			return true;
		}
	}
	return false;
}

bool UCerberusInventoryComponent::HasItem(TSubclassOf<UCerberusItem> ItemClass, const int32 Quantity) const
{
	if (UCerberusItem* ItemToFind = FindItemByClass(ItemClass))
	{
		return ItemToFind->GetQuantity() >= Quantity;
	}

	return false;
}

UCerberusItem* UCerberusInventoryComponent::FindItem(UCerberusItem* Item) const
{
	if (Item)
	{
		for (auto& InvItem : Items)
		{
			if (InvItem && InvItem->GetClass() == Item->GetClass())
			{
				return InvItem;
			}
		}
	}

	return nullptr;
}

UCerberusItem* UCerberusInventoryComponent::FindItemByClass(TSubclassOf<UCerberusItem> ItemClass) const
{
	for (auto& InvItem : Items)
	{
		if (InvItem && InvItem->GetClass() == ItemClass)
		{
			return InvItem;
		}
	}

	return nullptr;
}

TArray<UCerberusItem*> UCerberusInventoryComponent::FindItemsByClass(TSubclassOf<UCerberusItem> ItemClass) const
{
	TArray<UCerberusItem*> ItemsArr;
	for (auto& InvItem : Items)
	{
		if (InvItem && InvItem->GetClass()->IsChildOf(ItemClass))
		{
			ItemsArr.Add(InvItem);
		}
	}

	return ItemsArr;
}

int32 UCerberusInventoryComponent::ConsumeItem(UCerberusItem* Item)
{
	if (Item)
	{
		ConsumeItem(Item, Item->GetQuantity());
	}

	return 0;
}

int32 UCerberusInventoryComponent::ConsumeItem(UCerberusItem* Item, const int32 Quantity)
{
	if (GetOwner() && GetOwner()->HasAuthority() && Item)
	{
		const int32 RemoveQuantity = FMath::Min(Quantity, Item->GetQuantity());

		//We shouldn't have a negative amount of the item after the drop
		ensure(!(Item->GetQuantity() - RemoveQuantity < 0));
		
		Item->SetQuantity(Item->GetQuantity() - RemoveQuantity);
		
		//We now have 0 of this item, remove it from the inventory
		if (Item->GetQuantity() <= 0)
		{
			RemoveItem(Item);
		} else
		{
			//Broadcast to the client that items have been consumed
			ClientRefreshInventory();
		}
		return RemoveQuantity;
	}
	return 0;
}

void UCerberusInventoryComponent::InitializeWithAbilitySystem(UCerberusAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogCerberus, Error, TEXT("CerberusInventoryComponent: Inventory component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}
	
	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogCerberus, Error, TEXT("CerberusInventoryComponent: Cannot initialize the AbilitySystemComponent"), *GetNameSafe(Owner));
		return;
	}
	
	if (GetOwner()->HasAuthority())
	{
		// for (UCerberusItem* Item : DefaultItems)
		// {
		// 		TryAddItem(Item);
		// }
	}
}

void UCerberusInventoryComponent::UninitialieFromAbilitySystem()
{
	AbilitySystemComponent = nullptr;

	const AActor* lOwner = GetOwner();
	if (lOwner && lOwner->HasAuthority())
	{
		//RemoveAllItems();
	}
}

void UCerberusInventoryComponent::SetCapacity(int32 Value)
{
	Capacity = Value;
	OnInventoryUpdated.Broadcast();
}

void UCerberusInventoryComponent::ClientRefreshInventory_Implementation()
{
	OnInventoryUpdated.Broadcast();
}
#undef LOCTEXT_NAMESPACE