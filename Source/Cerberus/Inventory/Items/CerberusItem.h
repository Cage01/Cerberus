// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusItemProficiency.h"
#include "Cerberus/AbilitySystem/Abilities/CerberusGameplayAbility.h"
#include "Cerberus/Core/CerberusReplicatedObject.h"
#include "UObject/NoExportTypes.h"
#include "CerberusItem.generated.h"

class UCerberusItemModule;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemModified);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProficiencyModified);


UENUM(BlueprintType)
enum class EITemRarity : uint8
{
	IR_Common UMETA(DisplayName = "Common"),
	IR_Uncommon UMETA(DisplayName = "Uncommon"),
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
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class CERBERUS_API UCerberusItem : public UCerberusReplicatedObject
{
	GENERATED_BODY()

protected:
#if WITH_EDITOR
	//Clamps Quantity values in the editor
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
public:
	UCerberusItem();

	UPROPERTY(Transient)
	UWorld* World;
	
	virtual void MarkDirtyForReplication() override;
	
	/*
	* Accessor for the relevant inventory component
	*/
	UFUNCTION(BlueprintPure, Category="Cerberus|Items")
	UCerberusInventoryComponent* GetInventory() const;


	/**The display name for the item in the inventory*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items")
	FText ItemDisplayName;

	/**The description of the item*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items", meta=(MultiLine = true))
	FText ItemDescription;

	/**The thumbnail for this item*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cerberus|Items")
	UTexture2D* Thumbnail;
	
	/** Tooltip for the item (Equip, Eat, Use, etc)*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items")
	FText UseActionText;

	/**Flavor text of the item, possibly story related*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items", meta=(MultiLine = true))
	FText FlavorText;
	
	/**The mesh to display for this items pickup*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items")
	UStaticMesh* StaticMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items")
	EITemRarity Rarity;

	/**Will determine whether this item should have a popup window confirming whether to use it or not. Defaults to false.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cerberus|Items")
	bool bRequiresUseConfirm;
	
	/**This item can level up and your character can become more proficient in using it.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items")
	bool bHasProficiency;
	
	//This doesnt need to be replicated, but the th
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items", meta=(ClampMin = 1, EditCondition = bHasProficiency))
	TArray<FCerberusItemProficiency> ProficiencySockets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items")
	bool bStackable;
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items", meta=(ClampMin = 1, EditCondition = bHasProficiency))
	// int32 MaxProficiencyLevel;
	//
	// UPROPERTY(ReplicatedUsing = OnRep_CurrentProficiencyLevel, EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items", meta=(UIMin = 1, EditCondition = bHasProficiency))
	// int32 CurrentProficiencyLevel;

	/**Modules/Item Skills*/
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items")
	// bool bUsesSkills;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items", meta=(ClampMin = 0, EditCondition = bUsesSkills))
	// int32 MaxModules;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items", meta=(ClampMin = MaxModules, EditCondition = bUsesSkills))
	// TArray<UCerberusItemModule*> Modules;

	UFUNCTION()
	void OnRep_Quantity();
	
	UFUNCTION(BlueprintCallable, Category="Cerberus|Items")
	void SetQuantity(int32 NewQuantity);

	UFUNCTION(BlueprintCallable, Category="Cerberus|Items")
	void AddQuantity(int32 AmountToAdd);

	UFUNCTION(BlueprintCallable, Category="Cerberus|Items")
	void SubtractQuantity(int32 AmountToSubtract);
	
	UFUNCTION(BlueprintPure, Category="Cerberus|Items")
	FORCEINLINE int32 GetQuantity() const { return Quantity; }

	UFUNCTION(BlueprintCallable, Category="Cerberus|Items")
	FORCEINLINE int32 GetMaxStackSize() const { return MaxStackSize; }

	UFUNCTION(BlueprintCallable, Category="Cerberus|Items")
	FORCEINLINE int32 GetStackRemaining() const { return (MaxStackSize - Quantity); }

	virtual void Use(class ACerberusCharacter* Character) PURE_VIRTUAL(UCerberusItem::Use, );
	/**Can include different functionality based on context. Like if you hold the interact button on a weapon, you could automatically equip */
	virtual void AddedToInventory(UCerberusInventoryComponent* InventoryComponent);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(ACerberusCharacter* Character);

	UPROPERTY(BlueprintAssignable)
	FOnItemModified OnItemModified;

	// UPROPERTY(BlueprintAssignable)
	// FOnProficiencyModified OnProficiencyModified;

	UPROPERTY()
	UCerberusInventoryComponent* OwningInventory;
	
	UFUNCTION(BlueprintPure, Category="Cerberus|Items")
	virtual bool ShouldShowInInventory() const;
	
protected:
	int ID;
	
	UPROPERTY(ReplicatedUsing = OnRep_Quantity, EditAnywhere, Category="Cerberus|Items", meta=(UIMin = 1, EditCondition = bStackable))
	int32 Quantity;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Items", meta=(ClampMin = 2, EditCondition = bStackable))
	int32 MaxStackSize;


	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const override;
	
	// UFUNCTION()
	// void OnRep_CurrentProficiencyLevel();

private:
	//friend class UCerberusInventoryComponent;
	
};
