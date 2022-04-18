#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"


/*
 * CerberusLogChannels
 *
 * Provides some handy macros for logging, an example below
 * UE_LOG(LogCerberus, Error, TEXT("CerberusClassName: Log Text"), *GetNameSafe(Owner));
 */

CERBERUS_API DECLARE_LOG_CATEGORY_EXTERN(LogCerberus, Log, All);
CERBERUS_API DECLARE_LOG_CATEGORY_EXTERN(LogCerberusExperience, Log, All);
CERBERUS_API DECLARE_LOG_CATEGORY_EXTERN(LogCerberusAbilitySystem, Log, All);

CERBERUS_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
