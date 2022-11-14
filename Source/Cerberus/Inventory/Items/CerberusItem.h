// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cerberus/AbilitySystem/Abilities/CerberusGameplayAbility.h"
#include "Cerberus/Core/CerberusReplicatedObject.h"
#include "UObject/NoExportTypes.h"
#include "CerberusItem.generated.h"

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
	
	/**Weight of the item*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items", meta=(ClampMin = 0.0))
	float Weight;
	
	virtual void Use(class ACerberusCharacter* Character)  PURE_VIRTUAL(UItem, );

	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(ACerberusCharacter* Character);

protected:
	int ID;

	UPROPERTY(Replicated)
	UCerberusInventoryComponent* OwningInventory;

private:
	friend class UCerberusInventoryComponent;
	
};
