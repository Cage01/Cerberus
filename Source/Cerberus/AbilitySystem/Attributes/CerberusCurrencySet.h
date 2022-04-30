// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusAttributeSet.h"
#include "CerberusCurrencySet.generated.h"

/**
 * UCerberusCurrencySet
 *
 *	Class that defines attributes that are necessary for spending and receiving currency.
 *  Attribute examples include: Gold, Credits, (Maybe) Crafting materials, etc
 */
UCLASS(BlueprintType)
class CERBERUS_API UCerberusCurrencySet : public UCerberusAttributeSet
{
	GENERATED_BODY()
	
public:
	UCerberusCurrencySet();

	ATTRIBUTE_ACCESSORS(UCerberusCurrencySet, Quantity);
	
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
	
protected:
	UFUNCTION()
	void OnRep_Quantity(const FGameplayAttributeData& OldValue);

private:
	UPROPERTY(BlueprintReadOnly, Category="Cerberus|Currency", ReplicatedUsing=OnRep_Quantity, Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Quantity;


	// -------------------------------------------------------------------
	//	Meta Attribute (please keep attributes that aren't 'stateful' below 
	// -------------------------------------------------------------------
	
private:
	// Incoming event. This is mapped directly to +Quantity
	UPROPERTY(BlueprintReadOnly, Category="Cerberus|Currency", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Receiving;
	

	// Incoming event. This is mapped directly to -Quantity
	UPROPERTY(BlueprintReadOnly, Category="Cerberus|Currency", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Giving;
};
