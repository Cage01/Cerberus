// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CerberusReplicatedObject.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class CERBERUS_API UCerberusReplicatedObject : public UObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetWorld() const override;


	UFUNCTION(BlueprintPure, Category="Cerberus|Object")
	AActor* GetOwningActor() const;
	
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack) override;
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Cerberus|Object")
	void Destroy();

protected:
	virtual void OnDestroyed()
	{
		
	}
};
