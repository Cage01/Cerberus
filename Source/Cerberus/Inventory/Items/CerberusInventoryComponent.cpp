// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusInventoryComponent.h"
#include "Containers/Map.h"

// Sets default values for this component's properties
UCerberusInventoryComponent::UCerberusInventoryComponent()
{
	Capacity = 20;
}

// Called when the game starts
void UCerberusInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (const UCerberusInventorySlot* slot : DefaultItems)
	{
		AddItem(slot->Item, slot->Size);
	}
	
}


TPair<bool, int> UCerberusInventoryComponent::AddItem(UCerberusItem* Item, int Count)
{
	if (!Item)
		return TPair<bool, int>{false, Count};
	
	Item->OwningInventory = this;
	Item->World = GetWorld();
	
	for (int i = 0; i < Items.Num(); i++)
	{
		
		if (Items[i]->Item == Item)
		{
			const int currentCount = Items[i]->Size;
			if ( currentCount < Item->StackSize)
			{
				//Will add as much to a stack as possible before max is reached
				const int remaining = Item->StackSize - currentCount;
				const int newCount = Count - remaining;

				if (newCount == 0)
				{
					Items[i]->Item += Count;
					break;
				}
				Items[i]->Size += (Count - newCount);
				Count = newCount;
			}
		}
	}

	//Attempt to add item to a new slot in the inventory
	if (Count > 0)
		return NewItem(Item, Count);

	//Update UI
	OnInventoryUpdated.Broadcast();
	
	return TPair<bool, int>{true, 0};
}


TPair<bool, int> UCerberusInventoryComponent::NewItem(UCerberusItem* Item, int Count)
{
	if (Capacity > Items.Num())
	{
		UCerberusInventorySlot* slot = NewObject<UCerberusInventorySlot>();
		slot->Item = Item;
		slot->Size = std::min(Count, Item->StackSize);
		Items.Add(slot);

		if (slot->Size < Count)
		{
			const int newCount = Count - slot->Size;
			if (Count > 0)
				return NewItem(Item, newCount);
		}

		return TPair<bool, int>{true, 0};
	}

	return TPair<bool, int>{false, Count};
}


bool UCerberusInventoryComponent::RemoveItem(UCerberusItem* Item, int32 Count)
{
	Item->OwningInventory = nullptr;
	Item->World = nullptr;

	for (int i = 0; i < Items.Num(); i++)
	{
		
		if (Items[i]->Item == Item)
		{
			const int remaining = Items[i]->Size - Count;
			if (remaining <= 0)
			{
				Items.RemoveAt(i);

				Count = abs(remaining);
				if (Count == 0)
				{
					return true;
				}
			} else
			{
				Items[i]->Size = remaining;
				return true;
			}
		}
	}

	return false;
}


