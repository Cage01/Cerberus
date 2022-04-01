﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cerberus/Dev/Public/Projectile.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

USTRUCT()
struct FAmmoAmountChangedPayload
{
	GENERATED_BODY()

	int32 CurrentAmmoInClip;
	int32 ClipCapacity;
	int32 CurrentAmmo;
};

UCLASS()
class CERBERUS_API AWeaponBase : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Server function for spawning projectiles. */
	UFUNCTION(Server, Reliable)
	virtual void ServerOnAttack();

	void PreformHitscan();
	virtual void SpawnProjectile();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class UAttackComponent* AttackComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 MaxAmmo;

	// The damage type and damage that will be done by this weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	bool bIsHitscan;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float Range;
	
	// The damage dealt by this weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
	float Damage;

	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};