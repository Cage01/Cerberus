// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusCurrencySet.h"
#include "Net/UnrealNetwork.h"

UCerberusCurrencySet::UCerberusCurrencySet()
	:Quantity(0)
{
	
}

void UCerberusCurrencySet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCerberusCurrencySet, Quantity, COND_OwnerOnly, REPNOTIFY_Always);
}

void UCerberusCurrencySet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetQuantityAttribute())
	{
		// Dont let quantity drop below 0
		NewValue = FMath::Min(NewValue, 0.0f);
	}
}

void UCerberusCurrencySet::OnRep_Quantity(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCerberusCurrencySet, Quantity, OldValue);
}
