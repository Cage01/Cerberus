// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CerberusItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, Blueprintable)
class UCerberusItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CERBERUS_API ICerberusItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Item)
	UObject* GetInstigator() const;

	virtual void SetInstigator(UObject* InInstigator) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Item)
	APawn* GetPawn() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Item, meta=(DeterminesOutputType=PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;


};
