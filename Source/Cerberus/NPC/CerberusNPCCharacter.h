// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CerberusNPCCharacter.generated.h"

UCLASS()
class CERBERUS_API ACerberusNPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACerberusNPCCharacter();

	UPROPERTY(EditDefaultsOnly, Category="Cerberus|NPC")
	FText NPCName;

	UPROPERTY(EditDefaultsOnly, Category="Cerberus|NPC")
	FText NPCDescription;
	
	UPROPERTY(EditDefaultsOnly, Category="Cerberus|NPC")
	bool bIsHostile;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
