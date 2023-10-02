// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CerberusStatsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CERBERUS_API UCerberusStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCerberusStatsComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//////////////////////////////////////////////////////////////////////////
	///// Health
protected:
	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category="Cerberus|Character|Health")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cerberus|Character|Health")
	float MaxHealth;

public:
	float GetHealth();
	float GetMaxHealth();
	
	float ModifyHealth(const float Delta);

	UFUNCTION()
	void OnRep_Health(float OldHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthModified(const float HealthDelta);

	//////////////////////////////////////////////////////////////////////////
	///// Undefined

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;


};
