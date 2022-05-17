// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cerberus/Equipment/CerberusEquipmentInstance.h"
#include "UObject/Object.h"
#include "CerberberusWeaponInstance.generated.h"

/**
 * UCerberusWeaponInstance
 *
 * A weapon that will be spawned and applied to the pawn.
 */
UCLASS(BlueprintType, Blueprintable)
class UCerberberusWeaponInstance : public UCerberusEquipmentInstance
{
	GENERATED_BODY()

public:
	UCerberberusWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UCerberusEquipmentInstance interface
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;
	//~End of UCerberusEquipmentInstance interface
};
