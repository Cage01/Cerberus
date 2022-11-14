// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusInventoryComponent.h"
#include "Cerberus/CerberusLogChannels.h"
#include "Containers/Map.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

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
}


void UCerberusInventoryComponent::OnRep_ItemsUpdated() const
{
	OnInventoryUpdated.Broadcast();
}

void UCerberusInventoryComponent::OnUnregister()
{
	const AActor* lOwner = GetOwner();
	if (lOwner && lOwner->HasAuthority())
	{
		RemoveAllItems();
	}
	
	Super::OnUnregister();
}

void UCerberusInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCerberusInventoryComponent, Items, COND_None, REPNOTIFY_Always);
}

bool UCerberusInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	bWroteSomething |= Channel->ReplicateSubobjectList(Items, *Bunch, *RepFlags);
	return bWroteSomething;
}

UCerberusItem* UCerberusInventoryComponent::AddItem(TSubclassOf<UCerberusItem> ItemClass)
{
	if (ItemClass && !ItemClass->HasAnyClassFlags(CLASS_Abstract))
	{
		AActor* lOwner = GetOwner();
			
		checkf(lOwner != nullptr, TEXT("Invalid Inventory Owner"));
		checkf(lOwner->HasAuthority(), TEXT("Called without Authority!"));
		// Use the Actor as the Outer.
		UCerberusItem* NewItem = NewObject<UCerberusItem>(lOwner, ItemClass);
		checkf(NewItem != nullptr, TEXT("Unable to create inventory slot"));
		
		// Set immediately for replication
		NewItem->OwningInventory = this;
		NewItem->ID = Items.Num();
		Items.Add(NewItem);

		OnItemAdded.Broadcast(this, NewItem, NewItem->ID);
		return NewItem;
	}

	
	return nullptr;
}



bool UCerberusInventoryComponent::RemoveItem(UCerberusItem* Item)
{
	if (Item)
	{
		AActor* lOwner = GetOwner();
		checkf(lOwner != nullptr, TEXT("Invalid Inventory Owner"));
		checkf(lOwner->HasAuthority(), TEXT("Called without Authority!"));

		const int32 RemovedIndex = Items.Remove(Item);
		if (RemovedIndex != INDEX_NONE)
		{
			Item->Destroy();
			OnItemRemoved.Broadcast(this, Item, RemovedIndex);
			return true;
		}
	}

	
	return false;
}

void UCerberusInventoryComponent::RemoveAllItems()
{
	// AActor* lOwner = GetOwner();
	// checkf(lOwner != nullptr, TEXT("Invalid Inventory Owner"));
	// checkf(lOwner->HasAuthority(), TEXT("Called without Authority!"));

	for (UCerberusItem* Item : Items)
	{
		RemoveItem(Item);
	}
	Items.Empty();
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
		for (const TSubclassOf<UCerberusItem> Item : DefaultItems)
		{
				AddItem(Item);
		}
	
	}
	
}

void UCerberusInventoryComponent::UninitialieFromAbilitySystem()
{
	AbilitySystemComponent = nullptr;
}