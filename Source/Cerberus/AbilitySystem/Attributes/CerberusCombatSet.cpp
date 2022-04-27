// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusCombatSet.h"

#include "Net/UnrealNetwork.h"

UCerberusCombatSet::UCerberusCombatSet()
	:BaseDamage(0.0f)
	,BaseHeal(0.0f)
{
}

void UCerberusCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCerberusCombatSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCerberusCombatSet, BaseHeal, COND_OwnerOnly, REPNOTIFY_Always);
}


void UCerberusCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCerberusCombatSet, BaseDamage, OldValue);
}

void UCerberusCombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCerberusCombatSet, BaseHeal, OldValue);
}
