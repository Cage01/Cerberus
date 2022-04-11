// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusItemBase.h"
#include "Net/UnrealNetwork.h"


UCerberusItemBase::UCerberusItemBase(const FObjectInitializer& ObjectInitializer)
	:Super (ObjectInitializer)
{
}

UWorld* UCerberusItemBase::GetWorld() const
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

void UCerberusItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
}

UObject* UCerberusItemBase::GetInstigator() const
{
	return Instigator;
}

void UCerberusItemBase::SetInstigator(UObject* InInstigator)
{
	Instigator = InInstigator;
}

APawn* UCerberusItemBase::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

APawn* UCerberusItemBase::GetTypedPawn(TSubclassOf<APawn> PawnType) const
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

void UCerberusItemBase::OnRep_Instigator()
{
}
