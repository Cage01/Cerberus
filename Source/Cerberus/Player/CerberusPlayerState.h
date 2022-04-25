// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularPlayerState.h"
#include "CerberusPlayerState.generated.h"

class UCerberusAbilitySystemComponent;
class UAbilitySystemComponent;

/**
 * ACerberusPlayerState
 *
 * Base player state class used by the project.
 */
UCLASS(Config = Game)
class CERBERUS_API ACerberusPlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACerberusPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Cerberus|PlayerState")
	UCerberusAbilitySystemComponent* GetCerberusAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UFUNCTION()
	void OnRep_PawnData();

private:
	UPROPERTY(VisibleAnywhere, Category= "Cerberus|PlayerState")
	UCerberusAbilitySystemComponent* AbilitySystemComponent;
	
};
