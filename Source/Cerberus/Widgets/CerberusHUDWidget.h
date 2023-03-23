// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Cerberus/Player/CerberusPlayerController.h"
#include "CerberusHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class CERBERUS_API UCerberusHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Cerberus|Interaction")
	void UpdateHUDWidget(ACerberusPlayerController* CerberusController);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateHUDWidget();

	UPROPERTY(BlueprintReadOnly, Category="Cerberus|Interaction", meta=(ExposeOnSpawn))
	ACerberusPlayerController* OwningController;
};
