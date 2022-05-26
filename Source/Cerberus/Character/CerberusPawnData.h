// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cerberus/AbilitySystem/CerberusAbilitySet.h"
#include "Engine/DataAsset.h"
#include "CerberusPawnData.generated.h"

class UCerberusAbilityTagRelationshipMapping;
class APawn;


/**
 * UCerberusPawnData
 *
 *Non-mutable data asset that contains properties used to define a pawn.
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

	//Set of abilities available to the pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cerberus|Abilities")
	TArray<UCerberusAbilitySet*> AbilitySets;
	
	// What mapping of ability tags to use for actions taking by this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Abilities")
	UCerberusAbilityTagRelationshipMapping* TagRelationshipMapping;

	// // Input configuration used by player controlled pawns to create input mappings and bind input actions.
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Input")
	// UCerberusInputConfig* InputConfig;

	// // Default camera mode used by player controlled pawns.
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Camera")
	// TSubclassOf<UCerberusCameraMode> DefaultCameraMode;
};
