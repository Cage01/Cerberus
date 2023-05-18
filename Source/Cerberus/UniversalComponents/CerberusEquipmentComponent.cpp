// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusEquipmentComponent.h"

#include "Cerberus/Actors/CerberusPreviewActor.h"
#include "Cerberus/Enums/EquipableSlot.h"
#include "Cerberus/Items/CerberusEquipableItem.h"
#include "Cerberus/Items/CerberusGearItem.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UCerberusEquipmentComponent::UCerberusEquipmentComponent()
{
	
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	SetIsReplicated(true);
	
}

 void UCerberusEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
 {
 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
 	//DOREPLIFETIME(UCerberusEquipmentComponent, EquippedItems);
 }


bool UCerberusEquipmentComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	//Check if the array of items needs to replicate
	if (Channel->KeyNeedsToReplicate(0, ReplicatedItemsKey))
	{
		for (const TPair<EEquipableSlot, UCerberusEquipableItem* >& Slot : EquippedItems)
		{
			UCerberusEquipableItem* Item = Slot.Value;
			if (Channel->KeyNeedsToReplicate(Item->GetUniqueID(), Item->RepKey))
			{
				bWroteSomething |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
			}
		}
	}

	return bWroteSomething;
}


USkeletalMeshComponent* UCerberusEquipmentComponent::FindSlotSkeletalMeshComponent(EEquipableSlot Slot)
{
	if (EquippedMeshes.Contains(Slot))
	{
		return *EquippedMeshes.Find(Slot);
	}

	return nullptr;
}

bool UCerberusEquipmentComponent::EquipItem(UCerberusEquipableItem* Item)
{
	EquippedItems.Add(Item->Slot, Item);
	UpdatePreviewActor(Item);
	
	OnEquippedItemsChanged.Broadcast(Item->Slot, Item);
	
	return true;
}

bool UCerberusEquipmentComponent::UnEquipItem(UCerberusEquipableItem* Item)
{
	if (Item)
	{
		if (EquippedItems.Contains(Item->Slot))
		{
			if (Item == *EquippedItems.Find(Item->Slot))
			{
				EquippedItems.Remove(Item->Slot);
				UpdatePreviewActor(Item); //TODO I dont think this works for unequipping currently
				
				OnEquippedItemsChanged.Broadcast(Item->Slot, nullptr);
				return true;
			}
		}
	}

	return false;
}

void UCerberusEquipmentComponent::UpdateSkeletalMesh(UCerberusGearItem* Item)
{
	//Finding the mesh in the correct slot, and swapping it out with the new item, along with the material if one is defined
	if (USkeletalMeshComponent* GearMesh = *EquippedMeshes.Find(Item->Slot))
	{
		GearMesh->SetSkeletalMesh(Item->Mesh);
		GearMesh->SetRelativeTransform(Item->MeshTransform);
		GearMesh->SetMaterial(GearMesh->GetMaterials().Num() - 1, Item->MaterialInstance);
	}
}

void UCerberusEquipmentComponent::ResetSkeletalMesh(EEquipableSlot Slot)
{
	// This will currently just remove any skeletal mesh from the character, but not replace it with skin or anything else
	if (USkeletalMeshComponent* EquipableMesh = *EquippedMeshes.Find(Slot))
	{
		if (!DefaultMeshes.IsEmpty())
		{
			if (USkeletalMesh* DefaultMesh = *DefaultMeshes.Find(Slot))
			{
				EquipableMesh->SetSkeletalMesh(DefaultMesh);

				//Put the materials back on the body mesh (Since gear may have applied a different material)
				for (int32 i = 0; i < DefaultMesh->GetMaterials().Num(); ++i)
				{
					if (DefaultMesh->GetMaterials().IsValidIndex(i))
					{
						EquipableMesh->SetMaterial(i, DefaultMesh->GetMaterials()[i].MaterialInterface);
					}
				}
			} else
			{
				EquipableMesh->SetSkeletalMesh(nullptr);
			}
		} else
		{
			EquipableMesh->SetSkeletalMesh(nullptr);
		}

	}
}

// Called when the game starts
void UCerberusEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCerberusEquipmentComponent::UpdatePreviewActor(UCerberusEquipableItem* Item)
{
	//TODO might want to be dealt with in a better way later
	// Currently want to avoid modifying the root mesh
	if (Item->Slot != EEquipableSlot::ROOT)
	{
		// If the cast isnt successful then its not an item that has a visual component to it and we should continue
		if (const UCerberusGearItem* Gear = Cast<UCerberusGearItem>(Item))
		{
			PreviewActor->UpdateSubMeshMap(Gear->Slot, Gear->Mesh, Gear->MeshTransform);
		}

		//TODO: add case for weapon updates, this will put an object into a bone slot rather than update the skeletal mesh
	}

}
