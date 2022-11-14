// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusPlayerState.h"

#include "AbilitySystemComponent.h"
#include "Cerberus/AbilitySystem/CerberusAbilitySystemComponent.h"
#include "Cerberus/AbilitySystem/Attributes/CerberusHealthSet.h"
#include "Engine/ActorChannel.h"

//TODO Giving of default abilities should probably happen here
ACerberusPlayerState::ACerberusPlayerState(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Initialize AbilitySystemComponent
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UCerberusAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Initialize player attribute sets
	CreateDefaultSubobject<UCerberusHealthSet>("HealthSet");
	
	// AbilitySystemComponent needs to be updated at a high frequency, and PlayerState doesn't typically update as fast as necessary
	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* ACerberusPlayerState::GetAbilitySystemComponent() const
{
	return GetCerberusAbilitySystemComponent();
}

void ACerberusPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ACerberusPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

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

bool ACerberusPlayerState::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	check(Channel)
	check(Bunch)
	check(RepFlags)

	bool WroteSomething = false;

	for (UActorComponent* ActorComp : ReplicatedComponents)
	{
		if (ActorComp && ActorComp->GetIsReplicated())
		{
			if (!ActorComp->IsA(AbilitySystemComponent->GetClass()) || RepFlags->bNetOwner || !AbilitySystemComponent->ReplicationProxyEnabled)
			{
				WroteSomething |= ActorComp->ReplicateSubobjects(Channel, Bunch, RepFlags);
				WroteSomething |= Channel->ReplicateSubobject(ActorComp, *Bunch, *RepFlags);
			}
		}
	}
	
	return WroteSomething;
}
