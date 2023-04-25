// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Cerberus/Items/CerberusItem.h"
#include "Components/GameFrameworkComponent.h"
#include "CerberusInventoryComponent.generated.h"

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemsAdded UMETA(DisplayName = "No items added"),
	IAR_SomeItemsAdded UMETA(DisplayName = "Some items added"),
	IAR_AllItemsAdded UMETA(DisplayName = "All items added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

public:
	FItemAddResult() {};
	FItemAddResult(int32 InItemQuantity) : AmountToGive(InItemQuantity), ActualAmountGiven(0) {};
	FItemAddResult(int32 InItemQuantity, int32 InQuantityAdded) : AmountToGive(InItemQuantity), ActualAmountGiven(InQuantityAdded) {};
	
	UPROPERTY(BlueprintReadOnly, Category="Cerberus|Inventory|AddResult")
	int32 AmountToGive;
	
	UPROPERTY(BlueprintReadOnly, Category="Cerberus|Inventory|AddResult")
	int32 ActualAmountGiven;
	
	UPROPERTY(BlueprintReadOnly, Category="Cerberus|Inventory|AddResult")
	EItemAddResult Result;
	
	UPROPERTY(BlueprintReadOnly, Category="Cerberus|Inventory|AddResult")
	FText ErrorText;

	//Helper functions
	static FItemAddResult AddedNone(const int32 InItemQuantity, const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult(InItemQuantity);
		AddedNoneResult.Result = EItemAddResult::IAR_NoItemsAdded;
		AddedNoneResult.ErrorText = ErrorText;
		
		return AddedNoneResult;
	}

	static FItemAddResult AddedSome(const int32 InItemQuantity, const int32 InQuantityAdded, const FText& ErrorText)
	{
		FItemAddResult AddedSomeResult(InItemQuantity, InQuantityAdded);
		AddedSomeResult.Result = EItemAddResult::IAR_SomeItemsAdded;
		AddedSomeResult.ErrorText = ErrorText;
		
		return AddedSomeResult;
	}

	static FItemAddResult AddedAll(const int32 InItemQuantity)
	{
		FItemAddResult AddedAllResult(InItemQuantity, InItemQuantity);
		AddedAllResult.Result = EItemAddResult::IAR_AllItemsAdded;
		
		return AddedAllResult;
	}
};

class UCerberusAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAdded, UCerberusItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, UCerberusItem*, Item);

/**
 * UCerberusInventoryComponent
 * A component that allows an Actor to hold items. These can be used, looted or interacted with in some way with the inventory.
 * 
 * An Item must always be spawned into the world, or contained within an inventory.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CERBERUS_API UCerberusInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class UCerberusItem;

public:	
	UCerberusInventoryComponent(const FObjectInitializer& ObjectInitializer);

	/** Returns the Inventory component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Cerberus|Inventory")
	static UCerberusInventoryComponent* FindInventoryComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCerberusInventoryComponent>() : nullptr); }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
	
	/** Will add an item to the inventory thats already been initialized and doesnt need to be created. */
	UFUNCTION(BlueprintCallable, Category="Cerberus|Inventory")
	FItemAddResult TryAddItem(UCerberusItem* Item);

	/** Will attempt to create an item from the given class and add it to the inventory */
	UFUNCTION(BlueprintCallable, Category="Cerberus|Inventory")
	FItemAddResult TryAddItemFromClass(TSubclassOf<UCerberusItem> ItemClass, const int32 Quantity);
	/**
	 * Adds an item to the inventory
	 * @param ItemCloass Typically a blueprint class of the object that should be created and added to the inventory
	 */
	// UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Cerberus|Inventory")
	// UCerberusItem* AddItem(TSubclassOf<UCerberusItem> ItemClass);

	/**
	 * Removes an Item from the inventory
	 * @param Item A reference to the actual item object instance to be removed
	 */ 
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Cerberus|Inventory")
	bool RemoveItem(UCerberusItem* Item);

	UFUNCTION(BlueprintPure, Category="Cerberus|Inventory")
	bool HasItem(TSubclassOf<UCerberusItem> ItemClass, const int32 Quantity = 1) const;
	
	/** Return the first item with the same class as the given item */
	UFUNCTION(BlueprintPure, Category="Cerberus|Inventory")
	UCerberusItem* FindItem(UCerberusItem* Item) const;

	/** Return the first item with the same class as the ItemClass*/
	UFUNCTION(BlueprintPure, Category="Cerberus|Inventory")
	UCerberusItem* FindItemByClass(TSubclassOf<UCerberusItem> ItemClass) const;

	/** Get all inventory items that are inherited from ItemClass, useful for sorting and etc. */
	UFUNCTION(BlueprintPure, Category="Cerberus|Inventory")
	TArray<UCerberusItem*> FindItemsByClass(TSubclassOf<UCerberusItem> ItemClass) const;

	int32 ConsumeItem(UCerberusItem* Item);
	int32 ConsumeItem(UCerberusItem* Item, const int32 Quantity);
	
	void InitializeWithAbilitySystem(UCerberusAbilitySystemComponent* InASC);
	void UninitializeFromAbilitySystem();

	/** Getter function for inventory capacity */
	UFUNCTION(BlueprintPure, Category="Cerberus|Inventory")
	FORCEINLINE int32 GetCapacity() const { return Capacity; }

	/** Setter function for inventory capacity */
	UFUNCTION(BlueprintCallable, Category="Cerberus|Inventory")
	void SetCapacity(int32 Value);

	/** Getter function for an array of all the items inside an inventory */
	UFUNCTION(BlueprintPure, Category="Cerberus|Inventory")
	FORCEINLINE TArray<UCerberusItem*> GetItems() const { return Items; };

	UFUNCTION(Client, Reliable)
	void ClientRefreshInventory();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Cerberus|Inventory")
	TArray<TSubclassOf<UCerberusItem>> DefaultItems;

	UPROPERTY(ReplicatedUsing=OnRep_Items, VisibleAnywhere, Category="Cerberus|Inventory")
	TArray<UCerberusItem*> Items;

	UPROPERTY(EditDefaultsOnly, Category="Cerberus|Inventory")
	int32 Capacity;
	
	UPROPERTY(BlueprintAssignable, Category="Cerberus|Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
	
	UPROPERTY(BlueprintAssignable, Category="Cerberus|Inventory")
	FOnItemAdded OnItemAdded;

	UPROPERTY(BlueprintAssignable, Category="Cerberus|Inventory")
	FOnItemRemoved OnItemRemoved;
	
	UPROPERTY()
	UCerberusAbilitySystemComponent* AbilitySystemComponent;

private:
	/** Dont call Items.Add() directly, use this function instead, as it handles replication and ownership */
	UCerberusItem* AddItem(UCerberusItem* Item, int32 Quantity);
	
	UFUNCTION()
	void OnRep_Items();

	UPROPERTY()
	int32 ReplicatedItemsKey;

	/** Internal, non-BP exposed add item function. Dont call this directly, use TryAddItem(), or TryAddItemFromClass() instead. */
	FItemAddResult TryAddItem_Internal(UCerberusItem* Item);

	UFUNCTION()
	void ItemAdded(UCerberusItem* Item);

	UFUNCTION()
	void ItemRemoved(UCerberusItem* Item);
	
	UFUNCTION()
	void InventoryUpdated();
	
};
