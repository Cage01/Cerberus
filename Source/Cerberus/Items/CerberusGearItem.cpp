// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusGearItem.h"

#include "Cerberus/Character/CerberusCharacter.h"
#include "Cerberus/UniversalComponents/CerberusEquipmentComponent.h"
#include "Net/UnrealNetwork.h"

UCerberusGearItem::UCerberusGearItem()
{
	MeshTransform = FTransform(
		FRotator(0.f, 0.f, 0.f),
		FVector(0.f, 0.f, 0.f),
		FVector(1.f));
	
	DamageResistanceMultiplier = 0.1f;
	MaxDurability = 100;
	Durability = MaxDurability;
}

void UCerberusGearItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCerberusGearItem, Durability);
}

bool UCerberusGearItem::Equip(AActor* Owner)
{
	bool bEquipSuccessful = Super::Equip(Owner);

	//If the item was successfully equipped and the owner is valid, update the owner and show the new skeletal mesh
	if (bEquipSuccessful && Owner)
	{
		if (UCerberusEquipmentComponent* EquipmentComponent = UCerberusEquipmentComponent::FindEquipmentComponent(Owner))
		{
			EquipmentComponent->UpdateSkeletalMesh(this);
			return true;
		}
	}
	return false;
}

bool UCerberusGearItem::UnEquip(AActor* Owner)
{
	bool bUnEquipSuccessful = Super::UnEquip(Owner);

	//If the item was successfully equipped and the owner is valid, update the owner and show the new skeletal mesh
	if (bUnEquipSuccessful && Owner)
	{
		if (UCerberusEquipmentComponent* EquipmentComponent = UCerberusEquipmentComponent::FindEquipmentComponent(Owner))
		{
			EquipmentComponent->ResetSkeletalMesh(Slot);
			return true;
		}
	}

	return false;
}

void UCerberusGearItem::OnRep_DurabilityChanged()
{
}
