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

UWorld* UCerberusEquipmentInstance::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

void UCerberusEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, SpawnedActors);
}

void UCerberusEquipmentInstance::SetInstigator(UObject* InInstigator)
{
	Instigator = InInstigator;
}

UObject* UCerberusEquipmentInstance::GetInstigator_Implementation() const
{
	return Instigator;
}

APawn* UCerberusEquipmentInstance::GetPawn_Implementation() const
{
	return Cast<APawn>(GetOuter());
}

APawn* UCerberusEquipmentInstance::GetTypedPawn_Implementation(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}
	return Result;
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
}

void UCerberusEquipmentInstance::OnEquipped()
{
}

void UCerberusEquipmentInstance::OnUnequipped()
{
}

void UCerberusEquipmentInstance::OnRep_Instigator()
{
}
