// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusUsableItemInstance.h"
#include "Net/UnrealNetwork.h"

UCerberusUsableItemInstance::UCerberusUsableItemInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UCerberusUsableItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, SpawnedActors);
}

/*
* This function call will be passed to a blueprint implementable event
* Things like animations will occur here and etc.
*/
void UCerberusUsableItemInstance::OnUse()
{
	K2_OnUse();
}
