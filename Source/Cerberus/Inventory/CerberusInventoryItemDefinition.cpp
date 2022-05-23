// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusInventoryItemDefinition.h"

UCerberusInventoryItemDefinition::UCerberusInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

const UCerberusInventoryItemFragment* UCerberusInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UCerberusInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UCerberusInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

const UCerberusInventoryItemFragment* UCerberusInventoryFunctionLibrary::FindItemDefinitionFragment(TSubclassOf<UCerberusInventoryItemFragment> ItemDef, TSubclassOf<UCerberusInventoryItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UCerberusInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}
