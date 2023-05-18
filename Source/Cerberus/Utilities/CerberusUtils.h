// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "Cerberus/Items/CerberusItem.h"
#include "CerberusUtils.generated.h"

/**
 * 
 */

// Might be better extending UBlueprintFunctionLibrary?
UCLASS()
class CERBERUS_API UCerberusUtils : public UObject
{
	GENERATED_BODY()
    
public:
	UFUNCTION(BlueprintPure, Category = "Cerberus|Utilities")
	static bool IsListenServerHost(UWorld* World);

	UFUNCTION(BlueprintCallable, Category="Cerberus|Utilities")
	static TArray<uint8> SerializeObject(UObject* Target);

	UFUNCTION(BlueprintCallable, Category="Cerberus|Network", meta=(DeterminesOutputType="TargetClass"))
	static UObject* DeserializeObject(AActor* Owner, TSubclassOf<UObject> TargetClass, TArray<uint8> bytes);
};
