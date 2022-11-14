// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusHealthComponent.h"

#include "GameplayEffectExtension.h"
#include "Cerberus/CerberusLogChannels.h"
#include "Cerberus/AbilitySystem/CerberusAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UCerberusHealthComponent::UCerberusHealthComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
	DeathState = ECerberusDeathState::NotDead;
}

void UCerberusHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCerberusHealthComponent, DeathState);
}

void UCerberusHealthComponent::InitializeWithAbilitySystem(UCerberusAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogCerberus, Error, TEXT("UCerberusHealthComponent: Health component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogCerberus, Error, TEXT("UCerberusHealthComponent: Cannot initialize health component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	HealthSet = AbilitySystemComponent->GetSet<UCerberusHealthSet>();
	if (!HealthSet)
	{
		UE_LOG(LogCerberus, Error, TEXT("UCerberusHealthComponent: Cannot initialize health component for owner [%s] with NULL health set on the ability system."), *GetNameSafe(Owner));
		return;
	}

	// Register to listen for attribute changes.
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCerberusHealthSet::GetHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCerberusHealthSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::HandleMaxHealthChanged);
	HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);

	// TEMP: Reset attributes to default values.  Eventually this will be driven by a spread sheet.
	//AbilitySystemComponent->SetNumericAttributeBase(UCerberusHealthSet::GetHealthAttribute(), HealthSet->GetMaxHealth());

	ClearGameplayTags();

	OnHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
	OnMaxHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);

	//UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(GetOwner(), UGameFrameworkComponentManager::NAME_HealthComponentReady);

}

void UCerberusHealthComponent::UninitializeFromAbilitySystem()
{
	ClearGameplayTags();

	if (HealthSet)
	{
		HealthSet->OnOutOfHealth.RemoveAll(this);
	}

	HealthSet = nullptr;
	AbilitySystemComponent = nullptr;
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


void UCerberusHealthComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();
	
	Super::OnUnregister();
}

void UCerberusHealthComponent::ClearGameplayTags()
{
	if (AbilitySystemComponent)
	{
		// const FCerberusGameplayTags& GameplayTags = FCerberusGameplayTags::Get();
		//
		// AbilitySystemComponent->SetLooseGameplayTagCount(GameplayTags.Status_Death_Dying, 0);
		// AbilitySystemComponent->SetLooseGameplayTagCount(GameplayTags.Status_Death_Dead, 0);
	}

}

static AActor* GetInstigatorFromAttrChangeData(const FOnAttributeChangeData& ChangeData)
{
	if (ChangeData.GEModData != nullptr)
	{
		const FGameplayEffectContextHandle& EffectContext = ChangeData.GEModData->EffectSpec.GetEffectContext();
		return EffectContext.GetOriginalInstigator();
	}

	return nullptr;
}

void UCerberusHealthComponent::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	OnHealthChanged.Broadcast(this, ChangeData.OldValue, ChangeData.NewValue, GetInstigatorFromAttrChangeData(ChangeData));

}

void UCerberusHealthComponent::HandleMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	OnMaxHealthChanged.Broadcast(this, ChangeData.OldValue, ChangeData.NewValue, GetInstigatorFromAttrChangeData(ChangeData));
}


void UCerberusHealthComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude)
{
// #if WITH_SERVER_CODE
// 	if (AbilitySystemComponent)
// 	{
// 		// Send the "GameplayEvent.Death" gameplay event through the owner's ability system.  This can be used to trigger a death gameplay ability.
// 		{
// 			FGameplayEventData Payload;
// 			Payload.EventTag = FLyraGameplayTags::Get().GameplayEvent_Death;
// 			Payload.Instigator = DamageInstigator;
// 			Payload.Target = AbilitySystemComponent->GetAvatarActor();
// 			Payload.OptionalObject = DamageEffectSpec.Def;
// 			Payload.ContextHandle = DamageEffectSpec.GetEffectContext();
// 			Payload.InstigatorTags = *DamageEffectSpec.CapturedSourceTags.GetAggregatedTags();
// 			Payload.TargetTags = *DamageEffectSpec.CapturedTargetTags.GetAggregatedTags();
// 			Payload.EventMagnitude = DamageMagnitude;
//
// 			FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
// 			AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
// 		}
//
// 		// Send a standardized verb message that other systems can observe
// 		{
// 			FLyraVerbMessage Message;
// 			Message.Verb = TAG_Lyra_Elimination_Message;
// 			Message.Instigator = DamageInstigator;
// 			Message.InstigatorTags = *DamageEffectSpec.CapturedSourceTags.GetAggregatedTags();
// 			Message.Target = ULyraVerbMessageHelpers::GetPlayerStateFromObject(AbilitySystemComponent->GetAvatarActor());
// 			Message.TargetTags = *DamageEffectSpec.CapturedTargetTags.GetAggregatedTags();
// 			//@TODO: Fill out context tags, and any non-ability-system source/instigator tags
// 			//@TODO: Determine if it's an opposing team kill, self-own, team kill, etc...
//
// 			UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
// 			MessageSystem.BroadcastMessage(Message.Verb, Message);
// 		}
//
// 		//@TODO: assist messages (could compute from damage dealt elsewhere)?
// 	}
//
// #endif // #if WITH_SERVER_CODE
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

void UCerberusHealthComponent::OnRep_DeathState(ECerberusDeathState OldDeathState)
{
}
