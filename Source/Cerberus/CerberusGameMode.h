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
	ACerberusGameMode(const FObjectInitializer& ObjectInitializer);

	// //~AGameModeBase interface
	// virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	// virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	// virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	// virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	// virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	// virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	// virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;
	// virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
	// virtual void InitGameState() override;
	// //~End of AGameModeBase interface
	//
	//
	// // Restart (respawn) the specified player or bot next frame
	// // - If bForceReset is true, the controller will be reset this frame (abandoning the currently possessed pawn, if any)
	// UFUNCTION(BlueprintCallable)
	// void RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset = false);
	//
	// // Agnostic version of PlayerCanRestart that can be used for both player bots and players
	// virtual bool ControllerCanRestart(AController* Controller);
};



