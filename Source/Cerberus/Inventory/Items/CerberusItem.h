// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CerberusItem.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class CERBERUS_API UCerberusItem : public UObject
{
	GENERATED_BODY()

public:
	UCerberusItem();

	virtual UWorld* GetWorld() const override { return World; };
	
	UPROPERTY(Transient)
	UWorld* World;

	/** Tooltip for the item (Equip, Eat, Use, etc)*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Item")
	FText UseActionText;

	/**The mesh to display for this items pickup*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Item")
	class UStaticMesh* PickupMesh;

	/**The thumbnail for this item*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cerberus|Item")
	class UTexture2D* Thumbnail;

	/**The display name for the item in the inventory*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Item")
	FText ItemDisplayName;

	/**The description of the item*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Item", meta=(MultiLine = true))
	FText ItemDescription;

	/**Flavor text of the item, possibly story related*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Item", meta=(MultiLine = true))
	FText FlavorText;
	
	/**Weight of the item*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Item", meta=(ClampMin = 0.0))
	float Weight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Item")
	int StackSize;

	/**The inventory that owns this item*/
	UPROPERTY()
	class UCerberusInventoryComponent* OwningInventory;

	virtual void Use(class ACerberusCharacter* Character)  PURE_VIRTUAL(UItem, );

	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(class ACerberusCharacter* Character);
};
