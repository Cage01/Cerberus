// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusReplicatedObject.h"

UCerberusReplicatedObject::UCerberusReplicatedObject()
{
	RepKey = 0;
}

UWorld* UCerberusReplicatedObject::GetWorld() const
{
	if (const UObject* MyOuter = GetOuter())
	{
		return MyOuter->GetWorld();
	}

	return  nullptr;
}

AActor* UCerberusReplicatedObject::GetOwningActor() const
{
	return GetTypedOuter<AActor>();
}

void UCerberusReplicatedObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	if (const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}
}
bool UCerberusReplicatedObject::CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack)
{
	check(!HasAnyFlags(RF_ClassDefaultObject));

	AActor* Owner = GetOwningActor();
	UNetDriver* NetDriver = Owner->GetNetDriver();
	if (NetDriver)
	{
		NetDriver->ProcessRemoteFunction(Owner, Function, Parms, OutParms, Stack, this);
		return true;
	}
	return false;
}
int32 UCerberusReplicatedObject::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	check (GetOuter() != nullptr)
	return GetOuter()->GetFunctionCallspace(Function, Stack);
}

void UCerberusReplicatedObject::MarkDirtyForReplication()
{
}

void UCerberusReplicatedObject::Destroy()
{
	if (IsValid(this))
	{
		checkf(GetOwningActor()->HasAuthority() == true, TEXT("Destroy:: Object does not have authority to destroy itself!"))

		OnDestroyed();
		MarkAsGarbage();
	}
}
