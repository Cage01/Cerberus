// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusItem.h"

#include "Net/UnrealNetwork.h"

UCerberusItem::UCerberusItem()
{
	Weight = 1.f;
	ItemDisplayName = FText::FromString("Item");
	UseActionText = FText::FromString("Use");
}

UCerberusInventoryComponent* UCerberusItem::GetInventory() const
{
	return OwningInventory;
}

bool UCerberusItem::ShouldShowInInventory() const
{
	return true;
}

void UCerberusItem::OnRep_Quantity()
{
}


void UCerberusItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		
	DOREPLIFETIME_CONDITION(UCerberusItem, OwningInventory, COND_InitialOnly);
}
