// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusItem.h"

UCerberusItem::UCerberusItem()
{
	Weight = 1.f;
	MaxStackSize = 1000;
	CurrentStackSize = 1;
	ItemDisplayName = FText::FromString("Item");
	UseActionText = FText::FromString("Use");
}
