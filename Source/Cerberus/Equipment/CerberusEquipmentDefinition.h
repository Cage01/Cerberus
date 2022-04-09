// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusEquipmentInstance.h"
#include "UObject/Object.h"
#include "CerberusEquipmentDefinition.generated.h"

/**
 * 
 */
UCLASS()
class CERBERUS_API UCerberusEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UCerberusEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<UCerberusEquipmentInstance> InstanceType;
};
