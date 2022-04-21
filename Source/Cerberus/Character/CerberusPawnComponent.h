// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "CerberusPawnComponent.generated.h"



UINTERFACE(BlueprintType)
class CERBERUS_API UCerberusReadyInterface : public UInterface
{
	GENERATED_BODY()
};

class ICerberusReadyInterface
{
	GENERATED_BODY()

public:
	virtual bool IsPawnComponentReadyToInitialize() const = 0;
};


UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class CERBERUS_API UCerberusPawnComponent : public UPawnComponent, public ICerberusReadyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	UCerberusPawnComponent(const FObjectInitializer& ObjectInitializer);

	virtual bool IsPawnComponentReadyToInitialize() const override { return true; }
};
