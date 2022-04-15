// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CerberusItemInstanceBase.generated.h"

/**
 * UCerberusInstanceBase
 *
 * Base Instance class for all types of items in the project. Whether it be consumables, equipment, weapons or etc.
 * This base class does not include actor spawns, because not every instance of an item will need to be spawned in the world.
 * Some may have particle effects, or just general gameplay effects without a visual representation.
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class CERBERUS_API UCerberusItemInstanceBase : public UObject
{
	GENERATED_BODY()

public:
	UCerberusItemInstanceBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override final;
	//~End of UObject interface
	
	UFUNCTION(BlueprintPure, Category=Item)
	UObject* GetInstigator() const;

	void SetInstigator(UObject* InInstigator);

	UFUNCTION(BlueprintPure, Category=Item)
	APawn* GetPawn() const;

	UFUNCTION(BlueprintPure, Category=Item, meta=(DeterminesOutputType=PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;

private:
	UFUNCTION()
	void OnRep_Instigator();
	
private:
	UPROPERTY(ReplicatedUsing=OnRep_Instigator)
	UObject* Instigator;

	// @TODO: Do I want this property here? Does having this replicated array and not use it cause performance issues?
	// UPROPERTY(Replicated)
	// TArray<AActor*> SpawnedActors;
};
