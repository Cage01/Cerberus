// Fill out your copyright notice in the Description page of Project Settings.


#include "Cerberus/Core/Public/Weapons/WeaponBase.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	//Set Defaults
	MaxAmmo = 250;
	ClipSize = 30;
	FireRate = 0.1f;
	Range = 100.0f; //Probably will represent "Ideal Range" where further than that the damage begins to drop off
	Type = Hitscan;
	
	Damage = 1.0f;
	DamageType = UDamageType::StaticClass();

	bIsFiringWeapon = false;
}

void AWeaponBase::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		// StopFire is called when the time with the length of FireRate finishes -- bIsFiringWeapon will be set to false automatically
		//@TODO : this does not account for automatic firing / Seems to be good for semi-automatic weapons
		World->GetTimerManager().SetTimer(FiringTimer, this, &AWeaponBase::StopFire, FireRate, false);
		
		ServerOnFire();
	}
}

void AWeaponBase::StopFire()
{
	bIsFiringWeapon = false;
}

void AWeaponBase::ServerOnFire_Implementation()
{
	bool bSuccess = false;
	
	switch (Type)
	{
	case Hitscan:
		bSuccess = FireHitscan();
		break;
	case Projectile:
		bSuccess = FireProjectile();
		break;
	case Melee:
		bSuccess = FireMelee();
		break;
	}

	//Not sure what to do with bSuccess yet.
}


bool AWeaponBase::FireHitscan()
{
	return true;
}

bool AWeaponBase::FireProjectile()
{
	// FVector spawnLocation = GetActorLocation() + (GetControlRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
	// FRotator spawnRotation = GetControlRotation();
	//
	// FActorSpawnParameters spawnParameters;
	// spawnParameters.Instigator = GetInstigator();
	// spawnParameters.Owner = this;
	//
	// GetWorld()->SpawnActor<AProjectile>(spawnLocation, spawnRotation, spawnParameters);

	return true;
}

bool AWeaponBase::FireMelee()
{
	return true;
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

