// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CerberusGameData.generated.h"

class UGameplayEffect;
/**
 * UCerberusGameData
 *
 * Non-mutable data asset that contains global game data
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Cerberus Game Data", ShortTooltip = "Data asset containing global game data"))
class CERBERUS_API UCerberusGameData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCerberusGameData();

	//static const UCerberusGameData& Get();

public:
	// Gameplay effect used to apply damage.  Uses SetByCaller for the damage magnitude.
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects", meta = (DisplayName = "Damage Gameplay Effect (SetByCaller)"))
	TSoftClassPtr<UGameplayEffect> DamageGameplayEffect_SetByCaller;

	// Gameplay effect used to apply healing.  Uses SetByCaller for the healing magnitude.
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects", meta = (DisplayName = "Heal Gameplay Effect (SetByCaller)"))
	TSoftClassPtr<UGameplayEffect> HealGameplayEffect_SetByCaller;

	// Gameplay effect used to add and remove dynamic tags.
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects")
	TSoftClassPtr<UGameplayEffect> DynamicTagGameplayEffect;
};
