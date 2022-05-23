// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cerberus/Equipment/CerberusEquipmentInstance.h"
#include "CerberberusWeaponInstance.generated.h"

/**
 * UCerberusWeaponInstance
 *
 * A weapon that will be spawned and applied to the pawn.
 */
UCLASS(BlueprintType, Blueprintable)
class UCerberberusWeaponInstance : public UCerberusEquipmentInstance
{
	GENERATED_BODY()

public:
	UCerberberusWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UCerberusEquipmentInstance interface
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;
	//~End of UCerberusEquipmentInstance interface

	UFUNCTION(BlueprintCallable)
	void UpdateFiringTime();

	UFUNCTION(BlueprintPure)
	float GetTimeSinceLastInteractedWith() const;

protected:
	// Choose the best layer from EquippedAnimSet or UneuippedAnimSet based on the specified gameplay tags
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Animation)
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const;

	
private:
	double TimeLastEquipped = 0.0;
	double TimeLastFired = 0.0;
};
