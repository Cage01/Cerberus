// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CerberusAbilitySystemComponent.generated.h"

class UCerberusGameplayAbility;
struct FGameplayTag;
struct FGameplayAbilitySpec;

//CERBERUS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_AbilityInputBlocked);

/**
 * UCerberusAbilitySystemComponent
 *
 * Base ability system component class used by this project
 */
UCLASS()
class CERBERUS_API UCerberusAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
};
