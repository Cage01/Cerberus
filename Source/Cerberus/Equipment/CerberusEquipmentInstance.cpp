// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusEquipmentInstance.h"
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

APawn* UCerberusEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

APawn* UCerberusEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
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

void UCerberusEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, SpawnedActors);
}
