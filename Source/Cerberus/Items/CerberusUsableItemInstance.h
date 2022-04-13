// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusItemInstanceBase.h"

#include "CerberusUsableItemInstance.generated.h"

/**
 * UCerberusUsableItemInterface
 *
 * An item that can be consumed/used a limited or unlimited amount of times.
 */
//Will it spawn actors? Maybe taking a health pack will cause an animation with an object visible


UCLASS(BlueprintType, Blueprintable)
class CERBERUS_API UCerberusUsableItemInstance : public UCerberusItemInstanceBase /*public UObject, public ICerberusItemInterface*/
{
	GENERATED_BODY()
	
public:
	UCerberusUsableItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnUse();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category=Item, meta=(DisplayName="OnUse"))
	void K2_OnUse();

// @TODO: Do I really want to spawn actors on a usable item?
private:
	UPROPERTY(Replicated)
	TArray<AActor*> SpawnedActors;
	
};
