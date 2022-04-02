// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

DECLARE_DELEGATE(FReplicatedAttack);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CERBERUS_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttackComponent();

	template <typename T>
	T FireProjectile(FVector SpawnLocation, FRotator SpawnRotation, FActorSpawnParameters SpawnParameters);
	void FireHitscan();
	
	FReplicatedAttack ReplicatedAttack;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** a timer handle used for providing the fire rate delay in-between spawns.*/
	FTimerHandle AttackTimer;

	/** Function for beginning weapon fire. */
	UFUNCTION(BlueprintCallable, Category="Combat")
	void StartAttack();

	/** Function for ending weapon fire. */
	UFUNCTION(BlueprintCallable, Category="Combat")
	void StopAttack();
	
	/** Delay between shots in seconds. Also prevents an overflow of server functions from binding SpawnProjectile directly to input.*/
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	float AttackRate;

	/** If true, we are firing projectiles */ //@TODO : options for melee and non-projectile type weapons 
	bool bIsAttacking;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
