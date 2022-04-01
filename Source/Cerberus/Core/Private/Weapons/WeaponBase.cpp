// Fill out your copyright notice in the Description page of Project Settings.


#include "Cerberus/Core/Public/Weapons/WeaponBase.h"
#include "Cerberus/Core/Public/Combat/AttackComponent.h"
#include "Cerberus/Dev/Public/Projectile.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));
	AttackComponent->ReplicatedAttack.BindUObject(this, &AWeaponBase::ServerOnAttack);
	AddOwnedComponent(AttackComponent);

	
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}



void AWeaponBase::ServerOnAttack_Implementation()
{
	//Hitscan attack type
	if (!bIsHitscan)
		SpawnProjectile();
	else
		PreformHitscan();

}

void AWeaponBase::PreformHitscan()
{
	
}

void AWeaponBase::SpawnProjectile()
{
	FVector spawnLocation = GetActorLocation() + (GetActorRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
	FRotator spawnRotation = GetActorRotation();

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	GetWorld()->SpawnActor<AProjectile>(spawnLocation,spawnRotation, spawnParameters);
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

