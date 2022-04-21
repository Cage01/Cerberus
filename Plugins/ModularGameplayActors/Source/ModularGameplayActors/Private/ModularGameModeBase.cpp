// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularGameModeBase.h"

#include "ModularGameState.h"
#include "ModularPawn.h"
#include "ModularPlayerController.h"
#include "ModularPlayerState.h"

AModularGameModeBase::AModularGameModeBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	GameStateClass = AModularGameStateBase::StaticClass();
	PlayerControllerClass = AModularPlayerController::StaticClass();
	PlayerStateClass = AModularPlayerState::StaticClass();
	DefaultPawnClass = AModularPawn::StaticClass();
}

AModularGameMode::AModularGameMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	GameStateClass = AModularGameState::StaticClass();
	PlayerControllerClass = AModularPlayerController::StaticClass();
	PlayerStateClass = AModularPlayerState::StaticClass();
	DefaultPawnClass = AModularPawn::StaticClass();
}
