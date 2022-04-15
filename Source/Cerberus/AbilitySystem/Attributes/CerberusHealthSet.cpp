// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusHealthSet.h"
#include "Net/UnrealNetwork.h"

UCerberusHealthSet::UCerberusHealthSet()
	: Health(100.0f)
	, MaxHealth(100.0f)
{
	bOutOfHealth = false;
}

void UCerberusHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCerberusHealthSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCerberusHealthSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UCerberusHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCerberusHealthSet, Health, OldValue);
}

void UCerberusHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCerberusHealthSet, MaxHealth, OldValue);
}
