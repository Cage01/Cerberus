// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum WeaponType
{
	Hitscan		UMETA(DisplayName = "Hitscan"),
	Projectile	UMETA(DisplayName = "Projectile"),
	Melee	UMETA(DisplayName = "Melee"),
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



	/** a timer handle used for providing the fire rate delay in-between spawns.*/
	FTimerHandle FiringTimer;

	/** Function for beginning weapon fire. */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void StartFire();

	/** Function for ending weapon fire. */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void StopFire();

	/** Server function for spawning projectiles. */
	UFUNCTION(Server, Reliable)
	void ServerOnFire();

	/** Delay between shots in seconds. Also prevents an overflow of server functions from binding SpawnProjectile directly to input.*/
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float FireRate;

	/** If true, we are firing projectiles */ //@TODO : options for melee and non-projectile type weapons 
	bool bIsFiringWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 ClipSize;

	// The damage type and damage that will be done by this weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
	TSubclassOf<class UDamageType> DamageType;

	// The damage dealt by this weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
	float Damage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Range;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TEnumAsByte<WeaponType> Type;

	//Fire modes for each weapon type. Should return if hit was successful or not.
	bool FireHitscan();
	bool FireProjectile();
	bool FireMelee();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
