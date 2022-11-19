// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cerberus/AbilitySystem/Abilities/CerberusGameplayAbility.h"
#include "Cerberus/Core/CerberusReplicatedObject.h"
#include "UObject/NoExportTypes.h"
#include "CerberusItem.generated.h"

UENUM(BlueprintType)
enum class EITemRarity : uint8
{
	IR_Common UMETA(DisplayName = "Common"),
	IR_Uncommon UMETA(DisplayName = "UnCommon"),
	IR_Rare UMETA(DisplayName = "Rare"),
	IR_SuperRare UMETA(DisplayName = "Super Rare"),
	IR_Legendary UMETA(DisplayName = "Legendary")
};

class UCerberusInventoryComponent;
/**
 * UCerberusItem
 *
 * Base Item object to be used for the project. Is also replicated 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class CERBERUS_API UCerberusItem : public UCerberusReplicatedObject
{
	GENERATED_BODY()

public:
	UCerberusItem();

	
	/*
	* Accessor for the relevant inventory component
	*/
	UFUNCTION(BlueprintPure, Category="Cerberus|Inventory|Items")
	UCerberusInventoryComponent* GetInventory() const;

	/** Tooltip for the item (Equip, Eat, Use, etc)*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items")
	FText UseActionText;

	/**The mesh to display for this items pickup*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items")
	UStaticMesh* PickupMesh;

	/**The thumbnail for this item*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cerberus|Items")
	UTexture2D* Thumbnail;

	/**The display name for the item in the inventory*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items")
	FText ItemDisplayName;

	/**The description of the item*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items", meta=(MultiLine = true))
	FText ItemDescription;

	/**Flavor text of the item, possibly story related*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items", meta=(MultiLine = true))
	FText FlavorText;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items")
	EITemRarity Rarity;
	
	/**Weight of the item*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items", meta=(ClampMin = 0.0))
	float Weight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items")
	bool bStackable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items", meta=(ClampMin = 2, EditCondition = bStackable))
	int32 MaxStackSize;

	UPROPERTY(ReplicatedUsing = OnRep_Quantity, EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items")
	int32 Quantity;

	virtual void Use(class ACerberusCharacter* Character)  PURE_VIRTUAL(UItem, );

	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(ACerberusCharacter* Character);

	UFUNCTION(BlueprintPure, Category="Cerberus|Items")
	virtual bool ShouldShowInInventory() const;
	
protected:
	int ID;

	UPROPERTY(Replicated)
	UCerberusInventoryComponent* OwningInventory;

	UFUNCTION()
	void OnRep_Quantity();

private:
	friend class UCerberusInventoryComponent;
	
};
