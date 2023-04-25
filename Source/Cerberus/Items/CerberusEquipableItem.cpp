// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusEquipableItem.h"

#include "Cerberus/Character/CerberusCharacter.h"
#include "Cerberus/UniversalComponents/CerberusEquipmentComponent.h"
#include "Net/UnrealNetwork.h"

#define LOCTEXT_NAMESPACE "EquipableItem"
UCerberusEquipableItem::UCerberusEquipableItem()
{
	bStackable = false;
	bEquipped = false;
	UseActionText = LOCTEXT("ItemUseActionText", "Equip");
}

void UCerberusEquipableItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCerberusEquipableItem, bEquipped);
}

//TODO Will have to test if this works for things other than a character owned equipment component
void UCerberusEquipableItem::Use(ACerberusCharacter* Character)
{
	if (Character && Character->HasAuthority())
	{
		//TODO is this inefficient to run if an actor has a lot of components?
		if (UCerberusEquipmentComponent* FoundComponent = UCerberusEquipmentComponent::FindEquipmentComponent(Cast<AActor>(GetOuter())))
		{
			if (FoundComponent->EquippedItems.Contains(Slot) && !bEquipped)
			{
				UCerberusEquipableItem* OldItem = *FoundComponent->EquippedItems.Find(Slot);
				OldItem->SetEquipped(false);
			}

			//SetEquipped will run the OnRep function to all clients and the server to update character mesh
			SetEquipped(!IsEquipped());
		}
	}
}

bool UCerberusEquipableItem::Equip(AActor* Owner)
{
	if (UCerberusEquipmentComponent* EquipmentComponent = UCerberusEquipmentComponent::FindEquipmentComponent(Owner))
	{
		return EquipmentComponent->EquipItem(this);
	}

	return false;
}

bool UCerberusEquipableItem::UnEquip(AActor* Owner)
{
	if (UCerberusEquipmentComponent* EquipmentComponent = UCerberusEquipmentComponent::FindEquipmentComponent(Owner))
	{
		return EquipmentComponent->UnEquipItem(this);
	}

	return false;
}

bool UCerberusEquipableItem::ShouldShowInInventory() const
{
	return !bEquipped;
}

void UCerberusEquipableItem::SetEquipped(bool bNewEquipped)
{
	bEquipped = bNewEquipped;
	OnRep_EquipStatusChanged();
	MarkDirtyForReplication();
}

void UCerberusEquipableItem::OnRep_EquipStatusChanged()
{
	if (AActor* Owner = Cast<AActor>(GetOuter()))
	{
		if (bEquipped)
		{
			Equip(Owner);
		}
		else
		{
			UnEquip(Owner);
		}
	}

	//Tel the UI to update
	OnItemModified.Broadcast();
}
#undef LOCTEXT_NAMESPACE