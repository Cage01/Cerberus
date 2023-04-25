// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusItem.h"
#include "Cerberus/UniversalComponents/CerberusEquipmentComponent.h"
#include "CerberusEquipableItem.generated.h"

/**
 * Defined slots for an item to be equipped to.
 */
// UENUM(BlueprintType)
// enum class EEquipableSlot: uint8
// {
// 	EIS_HEAD UMETA(DisplayName = "Head"),
// 	EIS_HELMET UMETA(DisplayName = "Helmet"),
// 	EIS_CHEST UMETA(DisplayName = "Chest"),
// 	EIS_LEGS UMETA(DisplayName = "Legs"),
// 	EIS_BACK UMETA(DisplayName = "Back"),
// 	EIS_PRIMARY_WEAPON UMETA(DisplayName = "Primary Weapon"),
// 	EIS_SECONDARY_WEAPON UMETA(DisplayName = "Secondary Weapon"),
// 	EIS_THROWABLE UMETA(DisplayName = "Throwable"),
// };

/**
 * UCerberusEquipableItem
 *
 * Base class for all equipable items in the project. Including but not limited to weapons, armor, throwables such as grenades etc.
 * Not designed to be a base class for creating blueprints. If a new type of equipable item is needed, it must extend from this class
 */
UCLASS(Abstract, NotBlueprintable)
class CERBERUS_API UCerberusEquipableItem : public UCerberusItem
{
	GENERATED_BODY()

public:
	UCerberusEquipableItem();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cerberus|Equipables")
	EEquipableSlot Slot;
	
	virtual void Use(ACerberusCharacter* Character) override;

	UFUNCTION(BlueprintCallable, Category="Cerberus|Equipables")
	virtual bool Equip(AActor* Owner);
	
	UFUNCTION(BlueprintCallable, Category="Cerberus|Equipables")
	virtual bool UnEquip(AActor* Owner);

	virtual bool ShouldShowInInventory() const override;

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
