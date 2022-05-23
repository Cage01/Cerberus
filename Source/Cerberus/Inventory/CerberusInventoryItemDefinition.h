// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CerberusInventoryItemDefinition.generated.h"


class UCerberusInventoryItemInstance;

//////////////////////////////////////////////////////////////////////

/**
 * UCerberusInventoryFragment
 * Represents a fragment of an item definition
 *
 * Fragments are probably best described as instanced properties. For example it can represent the actual equipable item itself
 * Or it can be the item with the icon you see in your inventory. All of it ties to the CerberusInventoryItemDefinition.
 * Allowing the definition to effectively have multiple states to exist in
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class UCerberusInventoryItemFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(UCerberusInventoryItemInstance* Instance) const {}
};


/**
 * UCerberusInventoryItemDefinition
 */
UCLASS(Blueprintable, Const, Abstract)
class UCerberusInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UCerberusInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cerberus|Disaply")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cerberus", Instanced)
	TArray<TObjectPtr<UCerberusInventoryItemFragment>> Fragments;

public:
	const UCerberusInventoryItemFragment* FindFragmentByClass(TSubclassOf<UCerberusInventoryItemFragment> FragmentClass) const;
};


//@TODO: Make into a subsystem instead?
UCLASS()
class UCerberusInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const UCerberusInventoryItemFragment* FindItemDefinitionFragment(TSubclassOf<UCerberusInventoryItemFragment> ItemDef, TSubclassOf<UCerberusInventoryItemFragment> FragmentClass);
};
