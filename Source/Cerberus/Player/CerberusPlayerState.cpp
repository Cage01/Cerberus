// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusPlayerState.h"

#include "AbilitySystemComponent.h"
#include "Cerberus/AbilitySystem/CerberusAbilitySystemComponent.h"

ACerberusPlayerState::ACerberusPlayerState(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UCerberusAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

}

UAbilitySystemComponent* ACerberusPlayerState::GetAbilitySystemComponent() const
{
	return GetCerberusAbilitySystemComponent();
}

void ACerberusPlayerState::OnRep_PawnData()
{
}
