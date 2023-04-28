// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusEquipableItem.h"
#include "CerberusGearItem.generated.h"

/**
 * Base Gear Item for the project. This class defines the use of things like armor, clothes, bags and etc that will be applied and shown on the character
 */
UCLASS(Blueprintable)
class CERBERUS_API UCerberusGearItem : public UCerberusEquipableItem
{
	GENERATED_BODY()

public:
	UCerberusGearItem();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	virtual bool Equip(AActor* Owner) override;
	virtual bool UnEquip(AActor* Owner) override;
	
	/** The physical representation of this gear item */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Gear")
	USkeletalMesh* Mesh;

	/** Optional material instance to apply different looks and styles to a piece of gear */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Gear")
	UMaterialInstance* MaterialInstance;

	//TODO: Define attributes of gear

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Gear", meta=(ClampMin = 0.0, ClampMax = "1.0"))
	float DamageResistanceMultiplier;

	//TODO Add a PostEdiPropertyChange function to clamp these values in the editor
	UPROPERTY(ReplicatedUsing=OnRep_DurabilityChanged, EditAnywhere, BlueprintReadWrite, Category="Cerberus|Gear", meta=(ClampMin = 0.0))
	int32 Durability;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Gear", meta=(ClampMin = 1))
	int32 MaxDurability;

	UFUNCTION()
	void OnRep_DurabilityChanged();

};