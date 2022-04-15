// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusItemInstanceBase.h"
#include "Net/UnrealNetwork.h"


UCerberusItemInstanceBase::UCerberusItemInstanceBase(const FObjectInitializer& ObjectInitializer)
	:Super (ObjectInitializer)
{
}

UWorld* UCerberusItemInstanceBase::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
		return OwningPawn->GetWorld();
	else
		return nullptr;
}

void UCerberusItemInstanceBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
}

UObject* UCerberusItemInstanceBase::GetInstigator() const
{
	return Instigator;
}

void UCerberusItemInstanceBase::SetInstigator(UObject* InInstigator)
{
	Instigator = InInstigator;
}

APawn* UCerberusItemInstanceBase::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

APawn* UCerberusItemInstanceBase::GetTypedPawn(TSubclassOf<APawn> PawnType) const
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

void UCerberusItemInstanceBase::OnRep_Instigator()
{
}
