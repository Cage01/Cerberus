// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CerberusLootableActor.generated.h"

class ACerberusCharacter;
class UDataTable;
class UCerberusInventoryComponent;
class UCerberusInteractionComponent;

UCLASS()
class CERBERUS_API ACerberusLootableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACerberusLootableActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cerberus|Components")
	UStaticMeshComponent* LootContainerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cerberus|Components")
	UCerberusInteractionComponent* LootInteraction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cerberus|Components")
	UCerberusInventoryComponent* Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cerberus|Components")
	UDataTable* LootTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cerberus|Components")
	FIntPoint LootRolls;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnInteract(ACerberusCharacter* Character);
	
};
