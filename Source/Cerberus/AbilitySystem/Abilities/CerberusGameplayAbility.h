// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Cerberus/Cerberus.h"
#include "CerberusGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class CERBERUS_API UCerberusGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UCerberusGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Cerberus|Abilities")
	ECerberusAbilityInputID AbilityInputID = ECerberusAbilityInputID::None;
};
