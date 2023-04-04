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
	// Added for debugging purposes
	OnItemAdded.AddDynamic(this, &UCerberusInventoryComponent::ItemAdded);
	OnItemRemoved.AddDynamic(this, &UCerberusInventoryComponent::ItemRemoved);
	
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	
	Capacity = 20;
	AbilitySystemComponent = nullptr;

	// Component must be replicate to replicate its subobjects
	SetIsReplicatedByDefault(true);

	// New method of SubObject replication in 5.1
	bReplicateUsingRegisteredSubObjectList = true;
}

void UCerberusInventoryComponent::OnRep_Items()
{
	OnInventoryUpdated.Broadcast();

	
	for (auto& Item : Items)
	{
		// This is a trick to tell whether the item has be initialized on the client or not, and to send out the delegate if not
		// There may be a better way to do it though.
		if (!Item->World)
		{
			// Wont replicate the item if the player host is preforming this action
			if (GetWorld()->GetNetMode() != NM_ListenServer)
				OnItemAdded.Broadcast(Item);
			
			Item->World = GetWorld();
		}
	}
}

UCerberusItem* UCerberusInventoryComponent::AddItem(UCerberusItem* Item, int32 Quantity)
{
	if (GetOwner() && GetOwner()->HasAuthority() && Item)
	{
		UCerberusItem* NewItem = NewObject<UCerberusItem>(GetOwner(), Item->GetClass());
		NewItem->SetQuantity(Quantity);
		NewItem->OwningInventory = this;
		NewItem->AddedToInventory(this);
		Items.Add(NewItem);
		NewItem->MarkDirtyForReplication();
		AddReplicatedSubObject(NewItem);
		OnItemAdded.Broadcast(NewItem);
		OnRep_Items();
		
		return NewItem;
	}

	return nullptr;
}


FItemAddResult UCerberusInventoryComponent::TryAddItem_Internal(UCerberusItem* Item)
{
	if (GetOwner() && GetOwner()->HasAuthority() && Item)
	{
		const int32 AddAmount = Item->GetQuantity();

		if (Item->bStackable)
		{
			int32 TotalAdded = 0;
			TArray<UCerberusItem*> FoundItems = FindItemsByClass(Item->GetClass());

			if (FoundItems.IsEmpty())
			{
				//Check if there is space to add the item
				if (Items.Num() + 1 < GetCapacity())
				{
					//TODO: This doesnt take into account situations where only some items were added from the stack 
					const int32 ActualAddAmount = FMath::Min(AddAmount, Item->GetMaxStackSize());
					AddItem(Item,ActualAddAmount);
					return FItemAddResult::AddedAll(ActualAddAmount);
				}
				
				//Otherwise the inventory was full 
				return FItemAddResult::AddedNone(AddAmount, LOCTEXT("InventoryCapacityFullText", "Couldn't add item to your Inventory. Inventory is full"));
			}
			
			for ( const auto& FoundItem : FoundItems)
			{
				if (FoundItem->GetStackRemaining() > 0)
				{
					const int32 ActualAddAmount = FMath::Min(AddAmount, FoundItem->GetStackRemaining());
					FoundItem->AddQuantity(ActualAddAmount);
					TotalAdded += ActualAddAmount;

					ensure(FoundItem->GetQuantity() <= FoundItem->GetMaxStackSize());
				}
			}

			// Determine the correct AddResult and return
			if (TotalAdded <= 0)
				return FItemAddResult::AddedNone(AddAmount, LOCTEXT("InventoryCapacityFullText", "Couldn't add item to your Inventory. Inventory is full"));
			
			if (TotalAdded < Item->GetQuantity())
				return FItemAddResult::AddedSome(AddAmount, TotalAdded, LOCTEXT("InventoryStackErrorText", "Couldn't add entire stack of Item to inventory."));
			
			if (TotalAdded == Item->GetQuantity())
				return FItemAddResult::AddedAll(AddAmount);

		} else
		{
			if (Items.Num() + 1 < GetCapacity())
			{
				ensure(Item->GetQuantity() == 1);
				AddItem(Item, AddAmount);
				return FItemAddResult::AddedAll(Item->GetQuantity());
			}
			
			//Otherwise the inventory was full 
			return FItemAddResult::AddedNone(AddAmount, LOCTEXT("InventoryCapacityFullText", "Couldn't add item to your Inventory. Inventory is full"));
		}
	}
	
	return FItemAddResult::AddedNone(-1, LOCTEXT("ErrorMessage", ""));
}


void UCerberusInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCerberusInventoryComponent, Items);
}

bool UCerberusInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	//Check if the array of items needs to replicate
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

bool UCerberusInventoryComponent::RemoveItem(UCerberusItem* Item)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		if (Item)
		{
			Items.RemoveSingle(Item);
			OnItemRemoved.Broadcast(Item);

			OnRep_Items();
			
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
		return ConsumeItem(Item, Item->GetQuantity());
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
		
		Item->SubtractQuantity(RemoveQuantity);
		
		//We now have 0 of this item, remove it from the inventory
		if (Item->GetQuantity() <= 0)
		{
			RemoveItem(Item);
		}
		//Broadcast to the client that items have been consumed
		ClientRefreshInventory();
		
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
		for (const auto& ItemClass : DefaultItems)
		{
			TryAddItemFromClass(ItemClass, 1);
		}
	}
}

void UCerberusInventoryComponent::UninitializeFromAbilitySystem()
{
	AbilitySystemComponent = nullptr;

	const AActor* lOwner = GetOwner();
	if (lOwner && lOwner->HasAuthority())
	{
		// Removing all items
		for (auto& Item : Items)
		{
			RemoveItem(Item);
		}
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

void UCerberusInventoryComponent::ItemAdded(UCerberusItem* Item)
{
	FString RoleString = GetOwner()->HasAuthority() ? "server" : "client";
	UE_LOG(LogTemp, Warning, TEXT("Item added: %s on %s"), *GetNameSafe(Item), *RoleString);
}

void UCerberusInventoryComponent::ItemRemoved(UCerberusItem* Item)
{
	FString RoleString = GetOwner()->HasAuthority() ? "server" : "client";
	UE_LOG(LogTemp, Warning, TEXT("Item Removed: %s on %s"), *GetNameSafe(Item), *RoleString);
}
#undef LOCTEXT_NAMESPACE