// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CerberusAbilitySystemComponent.generated.h"

/**
 * UCerberusAbilitySystemComponent
 *
 * Base ability system component class used by this project
 */
UCLASS()
class CERBERUS_API UCerberusAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UCerberusAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface
};
