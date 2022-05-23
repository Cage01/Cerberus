// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberberusWeaponInstance.h"

UCerberberusWeaponInstance::UCerberberusWeaponInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UCerberberusWeaponInstance::OnEquipped()
{
	Super::OnEquipped();

	UWorld* World = GetWorld();
	check(World);
	TimeLastEquipped = World->GetTimeSeconds();
}

void UCerberberusWeaponInstance::OnUnequipped()
{
	Super::OnUnequipped();
}

void UCerberberusWeaponInstance::UpdateFiringTime()
{
	UWorld* World = GetWorld();
	check(World)
	TimeLastFired = World->GetTimeSeconds();
}

float UCerberberusWeaponInstance::GetTimeSinceLastInteractedWith() const
{
	UWorld* World = GetWorld();
	check(World);
	const double WorldTime = World->GetTimeSeconds();

	double Result = WorldTime - TimeLastEquipped;

	if (TimeLastFired > 0.0)
	{
		const double TimeSinceFired = WorldTime - TimeLastFired;
		Result = FMath::Min(Result, TimeSinceFired);
	}

	return Result;
}

TSubclassOf<UAnimInstance> UCerberberusWeaponInstance::PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const
{
	//const FLyraAnimLayerSelectionSet& SetToQuery = (bEquipped ? EquippedAnimSet : UneuippedAnimSet);
	//return SetToQuery.SelectBestLayer(CosmeticTags);

	return nullptr;
}
