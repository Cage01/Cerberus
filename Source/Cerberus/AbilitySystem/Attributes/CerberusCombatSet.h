// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusAttributeSet.h"
#include "CerberusCombatSet.generated.h"

/**
 * UCerberusCombatSet
 *
 *  Class that defines attributes that are necessary for applying damage or healing.
 *	Attribute examples include: damage, healing, attack power, and shield penetrations.
 */
UCLASS(BlueprintType)
class CERBERUS_API UCerberusCombatSet : public UCerberusAttributeSet
{
	GENERATED_BODY()

public:
	UCerberusCombatSet();

	ATTRIBUTE_ACCESSORS(UCerberusCombatSet, BaseDamage);
	ATTRIBUTE_ACCESSORS(UCerberusCombatSet, BaseHeal);

protected:
	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_BaseHeal(const FGameplayAttributeData& OldValue);
	
private:
	// @TODO Do these need to be only BlueprintReadOnly? Seems like it might be useful to have the defaults editable in Blueprints
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BaseDamage, Category="Cerberus|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BaseHeal, Category="Cerberus|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal;
};
