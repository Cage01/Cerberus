// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusItem.h"
#include "Cerberus/CerberusLogChannels.h"
#include "Net/UnrealNetwork.h"

#if WITH_EDITOR
void UCerberusItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(UCerberusItem, Quantity))
	{
		if (Quantity > MaxStackSize)
		{
			UE_LOG(LogCerberus, Warning, TEXT("You are trying to set the Quantity higher than the MaxStackSize property."))
		}
		Quantity = FMath::Clamp(Quantity, 1, bStackable ? MaxStackSize : 1);
	}

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(UCerberusItem, MaxStackSize))
	{
		if (MaxStackSize < Quantity)
		{
			UE_LOG(LogCerberus, Warning, TEXT("You are trying to set the MaxStackSize lower than the Quantity property."))
		}
		MaxStackSize = FMath::Clamp(MaxStackSize, bStackable ? Quantity : 1, MaxStackSize);
	}
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
		//MarkDirtyForReplication();
	}
}

void UCerberusItem::AddQuantity(int32 AmountToAdd)
{
	Quantity = FMath::Clamp(Quantity + AmountToAdd, 0, bStackable ? MaxStackSize : 1);
	//MarkDirtyForReplication();
}

void UCerberusItem::SubtractQuantity(int32 AmountToSubtract)
{
	Quantity = FMath::Clamp(Quantity - AmountToSubtract, 0, bStackable ? MaxStackSize : 1);
	//MarkDirtyForReplication();
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
