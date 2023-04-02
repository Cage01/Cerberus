// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CerberusReplicatedObject.generated.h"

/**
 * Base Replicated object for the server
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class CERBERUS_API UCerberusReplicatedObject : public UObject
{
	GENERATED_BODY()

public:
	UCerberusReplicatedObject();
	
	virtual UWorld* GetWorld() const override;

	/**Used to efficiently replicate inventory items*/
	UPROPERTY()
	int32 RepKey;



	UFUNCTION(BlueprintPure, Category="Cerberus|Object")
	AActor* GetOwningActor() const;
	
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack) override;
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Cerberus|Object")
	void Destroy();

protected:
	virtual void MarkDirtyForReplication() PURE_VIRTUAL(UCerberusReplicatedObject::MarkDirtyForReplication, );
	
	virtual void OnDestroyed()
	{
		
	}
};
