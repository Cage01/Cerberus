// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusItemProficiency.generated.h"


class UCerberusItemModule;
UENUM(BlueprintType)
enum class ECerberusProficiencySocketType : uint8
{
 None,
 StatDefense,
 StatOffense,
 StatUtility,
 Module
};
/**
 * 
 */
USTRUCT(BlueprintType)
struct FCerberusItemProficiency
{
 GENERATED_BODY();

 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cerberus|Items")
 bool bIsLocked;

 UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Cerberus|Items")
 ECerberusProficiencySocketType ProficiencySocketType = ECerberusProficiencySocketType::None;

 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cerberus|Items", meta=(EditCondition="ProficiencySocketType == ECerberusProficiencySocketType::Module"))
 UCerberusItemModule* Module;


};
