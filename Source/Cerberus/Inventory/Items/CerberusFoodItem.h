// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusItem.h"
#include "CerberusFoodItem.generated.h"

/**
 * 
 */
UCLASS()
class CERBERUS_API UCerberusFoodItem : public UCerberusItem
{
	GENERATED_BODY()

protected:
	virtual void Use(class ACerberusCharacter* Character) override;
};
