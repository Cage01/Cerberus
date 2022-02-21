// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CerberusGameMode.generated.h"

UCLASS(minimalapi)
class ACerberusGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACerberusGameMode();

	void Killed(AController* Killer, AController* KilledPlayer, APawn* KilledPawn, const UDamageType* DamageType);
	
protected:
	UFUNCTION(Server, Reliable)
	void RequestedRespawn(APlayerController* PlayerController);
};



