// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Cerberus/UniversalComponents/CerberusInteractionComponent.h"
#include "CerberusInteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class CERBERUS_API UCerberusInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Cerberus|Interaction")
	void UpdateInteractionWidget(UCerberusInteractionComponent* InteractionComponent);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateInteractionWidget();

	UPROPERTY(BlueprintReadOnly, Category="Cerberus|Interaction", meta=(ExposeOnSpawn))
	UCerberusInteractionComponent* OwningInteractionComponent;
};
