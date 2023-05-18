// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CerberusExperienceActionSet.generated.h"

/**
 * 
 */
UCLASS()
class CERBERUS_API UCerberusExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCerberusExperienceActionSet();

	//~UObject interface
	#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
	//~End of UObject interface

	//~UPrimaryDataAsset interface
	#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif
	//~End of UPrimaryDataAsset interface

	
public:
	// List of actions to perform as this experience is loaded/activated/deactivated/unloaded
	// UPROPERTY(EditAnywhere, Instanced, Category="Actions to Perform")
	// TArray<UGameFeatureAction*> Actions;

	// List of Game Feature Plugins this experience wants to have active
	UPROPERTY(EditAnywhere, Category="Feature Dependencies")
	TArray<FString> GameFeaturesToEnable;
};
