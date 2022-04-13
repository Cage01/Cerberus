// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusUsableItemDefinition.h"
#include "CerberusUsableItemInstance.h"

UCerberusUsableItemDefinition::UCerberusUsableItemDefinition(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	InstanceType = UCerberusUsableItemInstance::StaticClass();
}
