// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusItem.h"
#include <GameplayEffectTypes.h>
#include "CerberusConsumableItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CERBERUS_API UCerberusConsumableItem : public UCerberusItem
{
	GENERATED_BODY()
	
public:

	
protected:
	virtual void Use(ACerberusCharacter* Character) override;
};
