// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Items/CerberusItem.h"
#include "Components/GameFrameworkComponent.h"
#include "CerberusInventoryComponent.generated.h"


class UCerberusAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemAdded, UCerberusInventoryComponent*, InventoryComponent, UCerberusItem*, Item, int, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemRemoved,UCerberusInventoryComponent*, InventoryComponent, UCerberusItem*, Item, int, Index);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CERBERUS_API UCerberusInventoryComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCerberusInventoryComponent(const FObjectInitializer& ObjectInitializer);

	// Returns the Inventory component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "Cerberus|Inventory")
	static UCerberusInventoryComponent* FindInventoryComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCerberusInventoryComponent>() : nullptr); }

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	/**
	 * Adds an item to the inventory
	 * @param ItemCloass Typically a blueprint class of the object that should be created and added to the inventory
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Cerberus|Inventory")
	UCerberusItem* AddItem(TSubclassOf<UCerberusItem> ItemClass);

	/**
	 * Removes an Item from the inventory
	 * @param Item A reference to the actual item object instance to be removed
	 */ 
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Cerberus|Inventory")
	bool RemoveItem(UCerberusItem* Item);

	/** 
	 * Removes all items from the inventory
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Cerberus|Inventory")
	void RemoveAllItems();
	
	void InitializeWithAbilitySystem(UCerberusAbilitySystemComponent* InASC);
	void UninitialieFromAbilitySystem();


public:
	UPROPERTY(Replicated, EditDefaultsOnly, Category="Cerberus|Inventory")
	TArray<TSubclassOf<UCerberusItem>> DefaultItems;

	UPROPERTY(ReplicatedUsing=OnRep_ItemsUpdated, BlueprintReadOnly, Category="Cerberus|Inventory")
	TArray<UCerberusItem*> Items;

	UPROPERTY(EditDefaultsOnly, Category="Cerberus|Inventory")
	int32 Capacity;
	
	UPROPERTY(BlueprintAssignable, Category="Cerberus|Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
	
	UPROPERTY(BlueprintAssignable, Category="Cerberus|Inventory")
	FOnItemAdded OnItemAdded;

	UPROPERTY(BlueprintAssignable, Category="Cerberus|Inventory")
	FOnItemRemoved OnItemRemoved;

	
protected:
	UPROPERTY()
	UCerberusAbilitySystemComponent* AbilitySystemComponent;
	
	virtual void OnUnregister() override;	

private:
	UFUNCTION()
	void OnRep_ItemsUpdated() const;

	UPROPERTY()
	int32 ReplicatedItemsKey;
		
};
