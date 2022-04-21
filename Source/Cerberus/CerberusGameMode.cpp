// Copyright Epic Games, Inc. All Rights Reserved.

#include "CerberusGameMode.h"
#include "Character/CerberusCharacter.h"
#include "UObject/ConstructorHelpers.h"


ACerberusGameMode::ACerberusGameMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
//
// void ACerberusGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
// {
// 	Super::InitGame(MapName, Options, ErrorMessage);
// }
//
// UClass* ACerberusGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
// {
// 	return Super::GetDefaultPawnClassForController_Implementation(InController);
// }
//
// APawn* ACerberusGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
// 	const FTransform& SpawnTransform)
// {
// 	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
// }
//
// bool ACerberusGameMode::ShouldSpawnAtStartSpot(AController* Player)
// {
// 	return Super::ShouldSpawnAtStartSpot(Player);
// }
//
// void ACerberusGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
// {
// 	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
// }
//
// AActor* ACerberusGameMode::ChoosePlayerStart_Implementation(AController* Player)
// {
// 	return Super::ChoosePlayerStart_Implementation(Player);
// }
//
// void ACerberusGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
// {
// 	Super::FinishRestartPlayer(NewPlayer, StartRotation);
// }
//
// bool ACerberusGameMode::PlayerCanRestart_Implementation(APlayerController* Player)
// {
// 	return Super::PlayerCanRestart_Implementation(Player);
// }
//
// void ACerberusGameMode::InitGameState()
// {
// 	Super::InitGameState();
// }
//
// void ACerberusGameMode::RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset)
// {
// }
//
// bool ACerberusGameMode::ControllerCanRestart(AController* Controller)
// {
// 	return true;
// }
