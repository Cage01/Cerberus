// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusHealthComponent.h"


UCerberusHealthComponent::UCerberusHealthComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	
}

void UCerberusHealthComponent::InitializeWithAbilitySystem(UCerberusAbilitySystemComponent* InASC)
{

	AbilitySystemComponent = InASC;
	HealthSet = AbilitySystemComponent->GetSet<UCerberusHealthSet>();
	
}

void UCerberusHealthComponent::UninitialieWithAbilitySystem()
{
}

float UCerberusHealthComponent::GetHealth() const
{
	return (HealthSet ? HealthSet->GetHealth() : 0.0f);
}

float UCerberusHealthComponent::GetMaxHealth() const
{
	return (HealthSet ? HealthSet->GetMaxHealth() : 0.0f);
}

float UCerberusHealthComponent::GetHealthNormalized() const
{
	if (HealthSet)
	{
		const float Health = HealthSet->GetHealth();
		const float MaxHealth = HealthSet->GetMaxHealth();

		return ((MaxHealth > 0.0f) ? (Health / MaxHealth) : 0.0f);
	}

	return 0.0f;
}

ECerberusDeathState UCerberusHealthComponent::GetDeathState() const
{
	return DeathState;
}

void UCerberusHealthComponent::StartDeath()
{
}

void UCerberusHealthComponent::FinishDeath()
{
}

void UCerberusHealthComponent::DamageSelfDestruct(bool bFellOutOfWorld)
{
}

void UCerberusHealthComponent::OnUnregister()
{
	Super::OnUnregister();
}

void UCerberusHealthComponent::ClearGameplayTags()
{
}

void UCerberusHealthComponent::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
}

void UCerberusHealthComponent::HandleMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
}

void UCerberusHealthComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser,
	const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude)
{
}

void UCerberusHealthComponent::OnRep_DeathState(ECerberusDeathState OldDeathState)
{
}
