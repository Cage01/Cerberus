// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusAttributeSet.h"
#include "CerberusEquipmentSet.generated.h"

/**
 * UCerberusEquipmentSet
 *
 * Base attribute set for equipment to use in the project
 */
UCLASS()
class CERBERUS_API UCerberusEquipmentSet : public UCerberusAttributeSet
{
	GENERATED_BODY()

public:
	UCerberusEquipmentSet();

	ATTRIBUTE_ACCESSORS(UCerberusEquipmentSet, BondingLevel);
	ATTRIBUTE_ACCESSORS(UCerberusEquipmentSet, MaxBondingLevel);
	ATTRIBUTE_ACCESSORS(UCerberusEquipmentSet, CurrentExperience);
	ATTRIBUTE_ACCESSORS(UCerberusEquipmentSet, ExperienceToNextLevel);
	ATTRIBUTE_ACCESSORS(UCerberusEquipmentSet, LevelRequiredToUse);
	ATTRIBUTE_ACCESSORS(UCerberusEquipmentSet, Rarity);
	
protected:
	UFUNCTION()
	void OnRep_BondingLevel(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxBondingLevel(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_CurrentExperience(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_ExperienceToNextLevel(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_LevelRequiredToUse(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_Rarity(const FGameplayAttributeData& OldValue);
	
private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BondingLevel, Category="Cerberus|Equipment", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BondingLevel;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxBondingLevel, Category="Cerberus|Equipment", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxBondingLevel;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentExperience, Category="Cerberus|Equipment", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CurrentExperience;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ExperienceToNextLevel, Category="Cerberus|Equipment", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ExperienceToNextLevel;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LevelRequiredToUse, Category="Cerberus|Equipment", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData LevelRequiredToUse;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Rarity, Category="Cerberus|Equipment", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Rarity;
};
