// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusPlayerState.h"
#include "GameplayTagContainer.h"
#include "ModularPlayerController.h"
#include "CerberusPlayerController.generated.h"

class UCerberusAbilitySystemComponent;
/**
 * ACerberusPlayerController
 *
 * The base player controller class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class CERBERUS_API ACerberusPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	ACerberusPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Cerberus|PlayerController")
	ACerberusPlayerState* GetCerberusPlayerState() const;

	UFUNCTION(BlueprintCallable, Category="Cerberus|PlayerController")
	UCerberusAbilitySystemComponent* GetCerberusAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category="Cerberus|PlayerController")
	FORCEINLINE bool IsListenServer() { return  (GetNetMode() == NM_ListenServer); }
	
};
