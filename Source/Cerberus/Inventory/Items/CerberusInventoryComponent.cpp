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
	
	for (UCerberusItem* Item : DefaultItems)
	{
		AddItem(Item, Item->CurrentStackSize);
	}
	
}


TPair<bool, int> UCerberusInventoryComponent::AddItem(UCerberusItem* Item, int Count)
{
	if (!Item)
		return TPair<bool, int>{false, Count};
	
	for (int i = 0; i < Items.Num(); i++)
	{
		
		if (Items[i] == Item)
		{
			const int currentCount = Items[i]->CurrentStackSize;
			if ( currentCount < Items[i]->MaxStackSize)
			{
				//Will add as much to a stack as possible before max is reached
				const int remaining = Items[i]->MaxStackSize - currentCount;
				const int newCount = Count - remaining;

				if (newCount == 0)
				{
					Items[i]->CurrentStackSize += Count;
					break;
				}
				Items[i]->CurrentStackSize += (Count - newCount);
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
		Item->CurrentStackSize = std::min(Count, Item->MaxStackSize);
		Items.Add(Item);

		if (Item->CurrentStackSize < Count)
		{
			const int newCount = Count - Item->CurrentStackSize;
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
		
		if (Items[i] == Item)
		{
			const int remaining = Items[i]->CurrentStackSize - Count;
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
				Items[i]->CurrentStackSize = remaining;
				return true;
			}
		}
	}

	return false;
}


