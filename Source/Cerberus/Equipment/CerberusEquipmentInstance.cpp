// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusEquipmentInstance.h"

#include "CerberusEquipmentDefinition.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

UCerberusEquipmentInstance::UCerberusEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	:Super (ObjectInitializer)
{
}

void UCerberusEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, SpawnedActors);
}

void UCerberusEquipmentInstance::SpawnEquipmentActors(const TArray<FCerberusEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			AttachTarget = Char->GetMesh();
		}

		for (const FCerberusEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);

			SpawnedActors.Add(NewActor);
		}
	}
}

void UCerberusEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

/*
* This function call will be passed to a blueprint implementable event
* Things like animations will occur here and etc.
*/
void UCerberusEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void UCerberusEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}