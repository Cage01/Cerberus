// Copyright Epic Games, Inc. All Rights Reserved.

#include "CerberusGameMode.h"
#include "Character/CerberusCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACerberusGameMode::ACerberusGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
