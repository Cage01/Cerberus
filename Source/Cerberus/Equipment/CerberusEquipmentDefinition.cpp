// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusEquipmentDefinition.h"
#include "CerberusEquipmentInstance.h"

UCerberusEquipmentDefinition::UCerberusEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	InstanceType = UCerberusEquipmentInstance::StaticClass();
}
