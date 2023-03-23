// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusItem.h"

#include "Net/UnrealNetwork.h"

#if WITH_EDITOR
void UCerberusItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(UCerberusItem, Quantity))
	{
		Quantity = FMath::Clamp(Quantity, 1, bStackable ? MaxStackSize : 1);
	}
	// else if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(UCerberusItem, BondSockets))
	// {
	// 	// If greater than the allowed number of sockets, it will remove the last element
	// 	if (BondSockets.Num() > MaxBondLevel)
	// 	{
	// 		BondSockets.Pop();
	// 	}
	// }

}
#endif

UCerberusItem::UCerberusItem()
{
	Weight = 1.f;
	bStackable = true;
	Quantity = 1;
	// bBondable = false;
	ItemDisplayName = FText::FromString("Item");
	UseActionText = FText::FromString("Use");
}

UCerberusInventoryComponent* UCerberusItem::GetInventory() const
{
	return OwningInventory;
}

void UCerberusItem::SetQuantity(int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, bStackable ? MaxStackSize : 1);
		MarkDirtyForReplication();
	}
}

void UCerberusItem::Use(ACerberusCharacter* Character)
{
}

void UCerberusItem::AddedToInventory(UCerberusInventoryComponent* InventoryComponent)
{
}

bool UCerberusItem::ShouldShowInInventory() const
{
	return true;
}

void UCerberusItem::OnRep_Quantity()
{
	OnItemModified.Broadcast();
}

// void UCerberusItem::OnRep_CurrentProficiencyLevel()
// {
// 	OnProficiencyModified.Broadcast();
// }


void UCerberusItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCerberusItem, Quantity);
	//DOREPLIFETIME_CONDITION(UCerberusItem, OwningInventory, COND_InitialOnly);
}
