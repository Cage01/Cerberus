// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusItem.h"
#include <GameplayEffectTypes.h>
#include "CerberusConsumableItem.generated.h"

/**
 * 
 */
UCLASS()
class CERBERUS_API UCerberusConsumableItem : public UCerberusItem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Cerberus|Character")
	TArray<TSubclassOf<UGameplayEffect>> Effects;
	
protected:
	virtual void Use(class ACerberusCharacter* Character) override;
};
