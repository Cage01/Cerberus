// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusPlayerState.h"
#include "Engine/ActorChannel.h"

//TODO Giving of default abilities should probably happen here
ACerberusPlayerState::ACerberusPlayerState(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	NetUpdateFrequency = 100.0f;
}


void ACerberusPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ACerberusPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();



	// This appears to be some server side optimization stuff
	// if (GetNetMode() != NM_Client)
	// {
	// 	AGameStateBase* GameState = GetWorld()->GetGameState();
	// 	check(GameState);
	// 	ULyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	// 	check(ExperienceComponent);
	// 	ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	// }
}

