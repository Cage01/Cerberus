// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusGameMode.h"

#include "Cerberus/Player/CerberusPlayerController.h"
#include "Cerberus/Player/CerberusPlayerState.h"


ACerberusWorldGameMode::ACerberusWorldGameMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PlayerControllerClass = ACerberusPlayerController::StaticClass();
	PlayerStateClass = ACerberusPlayerState::StaticClass();
}
