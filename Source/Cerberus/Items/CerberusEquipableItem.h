// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusItem.h"
#include "Cerberus/UniversalComponents/CerberusEquipmentComponent.h"
#include "CerberusEquipableItem.generated.h"

/**
 * UCerberusEquipableItem
 *
 * Base class for all equipable items in the project. Including but not limited to weapons, armor, throwables such as grenades etc.
 * Not designed to be a base class for creating blueprints. If a new type of equipable item is needed, it must extend from this class
 */
UCLASS(Abstract, NotBlueprintable, BlueprintType)
class CERBERUS_API UCerberusEquipableItem : public UCerberusItem
{
	GENERATED_BODY()

public:
	UCerberusEquipableItem();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cerberus|Equipables")
	EEquipableSlot Slot;

	/** Will update the equip status of this item and correctly map it to the appropriate slot, and replace items already occupying that slot */
	virtual void Use(ACerberusCharacter* Character) override;

	/** Equipping an item. Be sure you want to use this over the Use() function. In most cases you will use that rather than this */
	UFUNCTION(BlueprintCallable, Category="Cerberus|Equipables")
	virtual bool Equip(AActor* Owner);

	/** Unequipping an item. Be sure you want to use this over the Use() function. In most cases you will use that rather than this */
	UFUNCTION(BlueprintCallable, Category="Cerberus|Equipables")
	virtual bool UnEquip(AActor* Owner);

	virtual bool ShouldShowInInventory() const override;

	/** This will return true of this item is equipped */
	UFUNCTION(BlueprintPure, Category="Cerberus|Equipables")
	bool IsEquipped() { return bEquipped; }
	
	void SetEquipped(bool bNewEquipped);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const override;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_EquipStatusChanged)
	bool bEquipped;

	UFUNCTION()
	void OnRep_EquipStatusChanged();
};
