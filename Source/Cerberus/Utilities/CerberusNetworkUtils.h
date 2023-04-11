// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "CerberusNetworkUtils.generated.h"

/**
 * 
 */

// Might be better extending UBlueprintFunctionLibrary?
UCLASS()
class CERBERUS_API UCerberusNetworkUtils : public UObject
{
	GENERATED_BODY()
    
public:
	UFUNCTION(BlueprintPure, Category = "Cerberus|Network")
	static bool IsListenServerHost(UWorld* World);
};
