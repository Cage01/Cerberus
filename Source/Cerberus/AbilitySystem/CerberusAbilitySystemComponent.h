// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CerberusAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class CERBERUS_API UCerberusAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UCerberusAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
