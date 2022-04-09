// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cerberus/Items/CerberusItemInterface.h"
#include "GameFramework/Pawn.h"
#include "UObject/Object.h"

#include "CerberusEquipmentInstance.generated.h"

struct FCerberusEquipmentActorToSpawn;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CERBERUS_API UCerberusEquipmentInstance : public UObject, public ICerberusItemInterface
{
	GENERATED_BODY()

public:
	UCerberusEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void SetInstigator(UObject* InInstigator) override;
	virtual UObject* GetInstigator_Implementation() const override;
	virtual APawn* GetPawn_Implementation() const override;
	virtual APawn* GetTypedPawn_Implementation(TSubclassOf<APawn> PawnType) const override;

	
	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override final;
	//~End of UObject interface
	
	UFUNCTION(BlueprintPure, Category=Equipment)
	TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }

	virtual void SpawnEquipmentActors(const TArray<FCerberusEquipmentActorToSpawn>& ActorsToSpawn);
	virtual void DestroyEquipmentActors();
	
	virtual void OnEquipped();
	virtual void OnUnequipped();


protected:
	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnUnequipped"))
	void K2_OnUnequipped();

private:
	UFUNCTION()
	void OnRep_Instigator();

private:
	UPROPERTY(ReplicatedUsing=OnRep_Instigator)
	UObject* Instigator;

	UPROPERTY(Replicated)
	TArray<AActor*> SpawnedActors;
};
