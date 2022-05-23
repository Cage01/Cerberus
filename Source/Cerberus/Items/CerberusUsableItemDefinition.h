// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CerberusUsableItemDefinition.generated.h"

class UCerberusAbilitySet;
class UCerberusUsableItemInstance;

/**
 * 
 */
UCLASS()
class CERBERUS_API UCerberusUsableItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UCerberusUsableItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	/*
	 * Apparently the Ability sets refer to things like firing, reloading, and auto reload. Which has base functionality in c++ code
	 * and has edited defaults for specific things like weapons.
	 */
	// Gameplay ability sets to grant when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<TObjectPtr<const UCerberusAbilitySet>> AbilitySetsToGrant;

	
	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<UCerberusUsableItemInstance> InstanceType;
};
