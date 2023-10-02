// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusStatsComponent.h"

#include "HAL/ThreadHeartBeat.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UCerberusStatsComponent::UCerberusStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	SetIsReplicated(true);

	MaxHealth = 100.f;
	Health = MaxHealth;
}

void UCerberusStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCerberusStatsComponent, Health);
}

float UCerberusStatsComponent::GetHealth()
{
	return Health;
}

float UCerberusStatsComponent::GetMaxHealth()
{
	return MaxHealth;
}

float UCerberusStatsComponent::ModifyHealth(const float Delta)
{
	const float OldHealth = Health;

	Health = FMath::Clamp<float>(Health + Delta, 0.f, MaxHealth);

	return Health - OldHealth;
}

void UCerberusStatsComponent::OnRep_Health(float OldHealth)
{
	OnHealthModified(Health - OldHealth);
}


// Called when the game starts
void UCerberusStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

