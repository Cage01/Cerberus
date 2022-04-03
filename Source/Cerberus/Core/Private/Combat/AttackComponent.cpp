// Fill out your copyright notice in the Description page of Project Settings.


#include "Cerberus/Core/Public/Combat/AttackComponent.h"


// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

template <class T>
T UAttackComponent::FireProjectile(FVector SpawnLocation, FRotator SpawnRotation, FActorSpawnParameters SpawnParameters)
{
	GetWorld()->SpawnActor<T>(SpawnLocation, SpawnRotation, SpawnParameters);
}

void UAttackComponent::StartAttack()
{
	if (!bIsAttacking)
	{
		bIsAttacking = true;
		UWorld* World = GetWorld();
		// StopFire is called when the time with the length of FireRate finishes -- bIsFiringWeapon will be set to false automatically
		World->GetTimerManager().SetTimer(AttackTimer, this, &UAttackComponent::StopAttack, AttackRate, false);

		ReplicatedAttack.ExecuteIfBound();
	}
}

void UAttackComponent::StopAttack()
{
	bIsAttacking = false;
}

// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

