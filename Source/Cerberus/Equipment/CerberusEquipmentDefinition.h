// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CerberusEquipmentDefinition.generated.h"

class UCerberusEquipmentInstance;

USTRUCT()
struct FCerberusEquipmentActorToSpawn
{
	GENERATED_BODY()

	FCerberusEquipmentActorToSpawn() {}

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};

/**
 * UCerberusEquipmentDefinition
 *
 * Definition of a piece of equipment that can be applied to a pawn
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class CERBERUS_API UCerberusEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UCerberusEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<UCerberusEquipmentInstance> InstanceType;

	/*
	 * Apparently the Ability sets refer to things like firing, reloading, and auto reload. Which has base functionality in c++ code
	 * and has edited defaults for specific things like weapons.
	 */
	// Gameplay ability sets to grant when this is equipped
	// UPROPERTY(EditDefaultsOnly, Category=Equipment)
	// TArray<TObjectPtr<const ULyraAbilitySet>> AbilitySetsToGrant;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<FCerberusEquipmentActorToSpawn> ActorsToSpawn;
};
