// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusHealthComponent.h"

#include "Cerberus/CerberusLogChannels.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Cerberus/CerberusGameplayTags.h"
#include "Cerberus/AbilitySystem/CerberusAbilitySystemComponent.h"
#include "Cerberus/AbilitySystem/Attributes/CerberusHealthSet.h"
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

void UCerberusHealthComponent::OnUnregister()
{
	UninitialieFromAbilitySystem();
	
	Super::OnUnregister();
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
		UE_LOG(LogCerberus, Error, TEXT("CerberusHealthComponent: Health component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}
	
	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogCerberus, Error, TEXT("CerberusHealthComponent: Cannot initialize the AbilitySystemComponent"), *GetNameSafe(Owner));
		return;
	}

	
	HealthSet = AbilitySystemComponent->AddSet<UCerberusHealthSet>();
	if (!HealthSet)
	{
		UE_LOG(LogCerberus, Error, TEXT("CerberusHealthComponent: Cannot initialize the HealthSet with the AbilitySystemComponent"), *GetNameSafe(Owner));
		return;
	}
	
	
	// Registering the component to listen for health related attribute changes.
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCerberusHealthSet::GetHealthAttribute()).AddUObject(this, &UCerberusHealthComponent::HandleHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCerberusHealthSet::GetMaxHealthAttribute()).AddUObject(this, &UCerberusHealthComponent::HandleMaxHealthChanged);
	HealthSet->OnOutOfHealth.AddUObject(this, &UCerberusHealthComponent::HandleOutOfHealth);

	// TEMP: Reset attributes to default values.  Eventually this will be driven by a spread sheet.
	AbilitySystemComponent->SetNumericAttributeBase(UCerberusHealthSet::GetHealthAttribute(), HealthSet->GetMaxHealth());
	
	// Resets the death and dying tags to 0 stacks
	ClearGameplayTags();

	OnHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
	OnMaxHealthChanged.Broadcast(this, HealthSet->GetMaxHealth(), HealthSet->GetMaxHealth(), nullptr);
}

void UCerberusHealthComponent::UninitialieFromAbilitySystem()
{
	ClearGameplayTags();
	
	if (HealthSet)
	{
		HealthSet->OnOutOfHealth.RemoveAll(this);
	}
	
	HealthSet = nullptr;
	AbilitySystemComponent = nullptr;
}

void UCerberusHealthComponent::ClearGameplayTags()
{
	if (AbilitySystemComponent)
	{
		const FCerberusGameplayTags& GameplayTags = FCerberusGameplayTags::Get();

		AbilitySystemComponent->SetLooseGameplayTagCount(GameplayTags.Status_Death_Dying, 0);
		AbilitySystemComponent->SetLooseGameplayTagCount(GameplayTags.Status_Death_Dead, 0);
	}
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

static AActor* GetInstigatorFromAttrChangeData(const FOnAttributeChangeData& ChangeData)
{
	if (ChangeData.GEModData != nullptr)
	{
		const FGameplayEffectContextHandle& EffectContext = ChangeData.GEModData->EffectSpec.GetEffectContext();
		return EffectContext.GetOriginalInstigator();
	}

	return nullptr;
}

ECerberusDeathState UCerberusHealthComponent::GetDeathState() const
{
	return DeathState;
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
#if WITH_SERVER_CODE
	if (AbilitySystemComponent)
	{
		// Send the "GameplayEvent.Death" gameplay event through the owner's ability system.  This can be used to trigger a death gameplay ability.
		{
			// Sending gameplay events and broadcasting death messages.
			FGameplayEventData Payload;
			Payload.EventTag = FCerberusGameplayTags::Get().GameplayEvent_Death;
			Payload.Instigator =DamageInstigator;
			Payload.Target = AbilitySystemComponent->GetAvatarActor();
			Payload.ContextHandle = DamageEffectSpec.GetContext();
			Payload.InstigatorTags = *DamageEffectSpec.CapturedSourceTags.GetAggregatedTags();
			Payload.TargetTags = *DamageEffectSpec.CapturedTargetTags.GetAggregatedTags();
			Payload.EventMagnitude = DamageMagnitude;

			FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent,true);
			AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
		}
		// @TODO This may be necessary in the future. Not now
		// // Send a standardized verb message that other systems can observe
		// {
		// 	FLyraVerbMessage Message;
		// 	Message.Verb = TAG_Lyra_Elimination_Message;
		// 	Message.Instigator = DamageInstigator;
		// 	Message.InstigatorTags = *DamageEffectSpec.CapturedSourceTags.GetAggregatedTags();
		// 	Message.Target = ULyraVerbMessageHelpers::GetPlayerStateFromObject(AbilitySystemComponent->GetAvatarActor());
		// 	Message.TargetTags = *DamageEffectSpec.CapturedTargetTags.GetAggregatedTags();
		// 	//@TODO: Fill out context tags, and any non-ability-system source/instigator tags
		// 	//@TODO: Determine if it's an opposing team kill, self-own, team kill, etc...
		//
		// 	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
		// 	MessageSystem.BroadcastMessage(Message.Verb, Message);
		// }
		//
		// //@TODO: assist messages (could compute from damage dealt elsewhere)?
	}
#endif
	
}

void UCerberusHealthComponent::OnRep_DeathState(ECerberusDeathState OldDeathState)
{
	const ECerberusDeathState NewDeathState = DeathState;

	// Revert the death state for now since we rely on StartDeath and FinishDeath to change it.
	DeathState = OldDeathState;

	if (OldDeathState > NewDeathState)
	{
		// The server is trying to set us back but we've already predicted past the server state.
		UE_LOG(LogCerberus, Warning, TEXT("CerberusHealthComponent: Predicted past server death state [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		return;
	}

	if (OldDeathState == ECerberusDeathState::NotDead)
	{
		if (NewDeathState == ECerberusDeathState::DeathStarted)
		{
			StartDeath();
		} else if (NewDeathState == ECerberusDeathState::DeathFinished)
		{
			StartDeath();
			FinishDeath();
		} else
		{
			UE_LOG(LogCerberus, Error, TEXT("CerberusHealthComponent: Invalid death transition [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		}
	} else if (OldDeathState == ECerberusDeathState::DeathStarted)
	{
		if (NewDeathState == ECerberusDeathState::DeathFinished)
		{
			FinishDeath();
		} else
		{
			UE_LOG(LogCerberus, Error, TEXT("LyraHealthComponent: Invalid death transition [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		}
	}

	ensureMsgf((DeathState == NewDeathState), TEXT("CerberusHealthComponent: Death transition failed [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
}

void UCerberusHealthComponent::StartDeath()
{
	if (DeathState != ECerberusDeathState::NotDead)
	{
		return;
	}

	DeathState = ECerberusDeathState::DeathStarted;

	// @TODO SetLooseGameplayTagCount is not replicated. Should it be? OnRep_DeathState is a function that replicates death out so may not be needed
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(FCerberusGameplayTags::Get().Status_Death_Dying, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathStarted.Broadcast(Owner);
	Owner->ForceNetUpdate();
}

void UCerberusHealthComponent::FinishDeath()
{
	if (DeathState != ECerberusDeathState::DeathStarted)
	{
		return;
	}

	DeathState = ECerberusDeathState::DeathFinished;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(FCerberusGameplayTags::Get().Status_Death_Dead, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);
	
	OnDeathFinished.Broadcast(Owner);
	Owner->ForceNetUpdate();
}

void UCerberusHealthComponent::DamageSelfDestruct(bool bFellOutOfWorld)
{
	if ((DeathState == ECerberusDeathState::NotDead) && AbilitySystemComponent)
	{
		// @TODO Handle fell out of world death actions.
	}
}


