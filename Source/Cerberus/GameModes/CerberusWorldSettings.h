// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "CerberusWorldSettings.generated.h"

class UCerberusExperienceDefinition;
/**
 * The default world settings object, used primarily to set the default gameplay experience to use when playing on this map
 * This extends the default world settings class when creating a new level and adds on extra WorldSettings
 */
UCLASS()
class CERBERUS_API ACerberusWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:

	ACerberusWorldSettings();
//
// #if WITH_EDITOR
// 	virtual void CheckForErrors() override;
// #endif
//
// public:
// 	/** Returns the default experience to use when a server opens this map if it is not overridden by the user-facing experience */
// 	//FPrimaryAssetId GetDefaultGameplayExperience() const;
//
// protected:
// 	/** The default experience to use when a server opens this map if it is not overridden by the user-facing experience */
// 	UPROPERTY(EditDefaultsOnly, Category=GameMode)
// 	TSoftClassPtr<UCerberusExperienceDefinition> DefaultGameplayExperience;
//
// public:
//
// #if WITH_EDITORONLY_DATA
// 	// Is this level part of a front-end or other standalone experience?
// 	// When set, the net mode will be forced to Standalone when you hit Play in the editor
// 	UPROPERTY(EditDefaultsOnly, Category=PIE)
// 	bool ForceStandaloneNetMode = false;
// #endif
 };
