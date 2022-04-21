// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CerberusPawnData.generated.h"

class APawn;


/**
 * 
 */
UCLASS(BlueprintType, Const, meta=(DisplayName="Cerberus Pawn Data", ShortTooltip="Data asset used to define a Pawn."))
class CERBERUS_API UCerberusPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCerberusPawnData(const FObjectInitializer& ObjectInitializer);

	// Type of pawn being used
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cerberus|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Set of abilities available to the pawn
	// UPROPERTY()
	// TArray<CerberusAbilitySet*>
};
