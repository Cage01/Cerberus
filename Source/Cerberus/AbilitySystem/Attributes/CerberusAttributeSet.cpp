// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusAttributeSet.h"
#include "Cerberus/AbilitySystem/CerberusAbilitySystemComponent.h"

UCerberusAttributeSet::UCerberusAttributeSet()
{
}

UWorld* UCerberusAttributeSet::GetWorld() const
{
	/*
	*
	*	A UObject does not know inherently
	*	what world it belongs to. The purpose
	*	of the GetWorld function is for that
	*	Object to be able to let others know
	*	which World it is in. Actors do this
	*	via their Level reference and
	*	Components do this either via their
	*	Actor owner or the World they’ve been
	*	registered with. Each other Object
	*	type has to find its own path back to
	*	World, often through a reference to an
	*	Actor.
	*
	*	If you want to be able to call
	*	GetWorld on your class, you need to
	*	override the function and provide a
	*	valid path to a World.
	*
	*/
	
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UCerberusAbilitySystemComponent* UCerberusAttributeSet::GetCerberusAbilitySystemComponent() const
{
	return Cast<UCerberusAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
