// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusPawnComponent.h"

UCerberusPawnComponent::UCerberusPawnComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}
