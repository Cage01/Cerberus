// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CerberusPlayerState.generated.h"

/**
 * ACerberusPlayerState
 *
 * Base player state class used by the project.
 * 
 */
UCLASS(Config = Game)
class CERBERUS_API ACerberusPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACerberusPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface
	

};
