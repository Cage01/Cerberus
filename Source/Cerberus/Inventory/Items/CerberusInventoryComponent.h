// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusInventorySlot.h"

#include "CerberusItem.h"
#include "Components/ActorComponent.h"
#include "CerberusInventoryComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CERBERUS_API UCerberusInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCerberusInventoryComponent();

	TPair<bool, int> AddItem(UCerberusItem* Item, int Count);
	bool RemoveItem(UCerberusItem* Item, int Count);

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UCerberusInventorySlot*> DefaultItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cerberus|Inventory|Items")
	TArray<UCerberusInventorySlot*> Items;

	UPROPERTY(EditDefaultsOnly, Category="Cerberus|Inventory")
	int32 Capacity;

	UPROPERTY(BlueprintAssignable, Category="Cerberus|Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	TPair<bool, int> NewItem(UCerberusItem* Item, int Count);

		
};
