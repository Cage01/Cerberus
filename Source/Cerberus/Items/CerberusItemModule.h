// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusItem.h"
#include "CerberusItemModule.generated.h"

/**
 * UCerberusItemModule
 *
 * Base object for item modules, which will slot into various pieces of equipment to give new abilities or characteristics.
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class CERBERUS_API UCerberusItemModule : public UCerberusItem
{
	GENERATED_BODY()

public:
	UCerberusItemModule();
};
