// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusEquipmentSet.h"

#include "Net/UnrealNetwork.h"

UCerberusEquipmentSet::UCerberusEquipmentSet()
{
}

void UCerberusEquipmentSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCerberusEquipmentSet, BondingLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCerberusEquipmentSet, MaxBondingLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCerberusEquipmentSet, CurrentExperience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCerberusEquipmentSet, ExperienceToNextLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCerberusEquipmentSet, LevelRequiredToUse, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCerberusEquipmentSet, Rarity, COND_None, REPNOTIFY_Always);
}

void UCerberusEquipmentSet::OnRep_BondingLevel(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCerberusEquipmentSet, BondingLevel, OldValue);
}

void UCerberusEquipmentSet::OnRep_MaxBondingLevel(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCerberusEquipmentSet, MaxBondingLevel, OldValue);
}

void UCerberusEquipmentSet::OnRep_CurrentExperience(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCerberusEquipmentSet, CurrentExperience, OldValue);
}

void UCerberusEquipmentSet::OnRep_ExperienceToNextLevel(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCerberusEquipmentSet, ExperienceToNextLevel, OldValue);
}

void UCerberusEquipmentSet::OnRep_LevelRequiredToUse(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCerberusEquipmentSet, LevelRequiredToUse, OldValue);
}

void UCerberusEquipmentSet::OnRep_Rarity(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCerberusEquipmentSet, Rarity, OldValue);
}
