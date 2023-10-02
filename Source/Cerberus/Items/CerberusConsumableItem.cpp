// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusConsumableItem.h"

#include "Cerberus/CerberusLogChannels.h"
#include "Cerberus/Character/CerberusCharacter.h"
#include "Cerberus/UniversalComponents/CerberusInventoryComponent.h"
#include "Cerberus/Player/CerberusPlayerState.h"


void UCerberusConsumableItem::Use(ACerberusCharacter* Character)
{

	if (Character)
	{
		//Do stuff

		if (UCerberusInventoryComponent* Inventory = Character->InventoryComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("UCerberusConsumableItem: Item being used"))
			Inventory->ConsumeItem(this, 1);
		}
	}
}
