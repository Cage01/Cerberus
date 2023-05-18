// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CerberusGameMode.generated.h"

/**
 *  Post login event, triggered when a player joins the game as well as after non-seamless ServerTravel
 *
 *  This is called after the player has finished initialization
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGameModeCombinedPostLogin, AGameModeBase* /*GameMode*/, AController* /*NewPlayer*/);

UCLASS(Config = Game, meta=(ShortTooltip="The base game mode class used by this project"))
class ACerberusWorldGameMode : public AGameMode
{
  GENERATED_BODY()

public:
	ACerberusWorldGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
