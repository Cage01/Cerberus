// Copyright Epic Games, Inc. All Rights Reserved.

#include "CerberusGameMode.h"
#include "CerberusCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACerberusGameMode::ACerberusGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Core/Characters/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
}

void ACerberusGameMode::Killed(AController* Attacker, AController* Victim, APawn* KilledPawn,
	const UDamageType* DamageType)
{
}

void ACerberusGameMode::RequestedRespawn_Implementation(APlayerController* PlayerController)
{
	
}
