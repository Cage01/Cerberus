// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusAbilitySet.h"

#include "CerberusAbilitySystemComponent.h"
#include "Cerberus/CerberusLogChannels.h"

void FCerberusAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FCerberusAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FCerberusAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	GrantedAttributeSets.Add(Set);
}

void FCerberusAbilitySet_GrantedHandles::TakeFromAbilitySystem(UCerberusAbilitySystemComponent* CerberusASC)
{
	check(CerberusASC)

	if (!CerberusASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
			CerberusASC->ClearAbility(Handle);
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		CerberusASC->GetSpawnedAttributes_Mutable().Remove(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

UCerberusAbilitySet::UCerberusAbilitySet(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UCerberusAbilitySet::GiveToAbilitySystem(UCerberusAbilitySystemComponent* CerberusASC, FCerberusAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(CerberusASC)

	if (!CerberusASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	// Grant the gameplay abilities.
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); AbilityIndex++)
	{
		const FCerberusAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogCerberusAbilitySystem, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UCerberusGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UCerberusGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		// Keeping track of all given abilities in the struct. The Spec Handle has all the information about said ability.
		const FGameplayAbilitySpecHandle AbilitySpecHandle = CerberusASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}


	// Grant the gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); EffectIndex++)
	{
		const FCerberusAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogCerberusAbilitySystem, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid."), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = CerberusASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, CerberusASC->MakeEffectContext());

		// Keeping track of all given effects in the struct. GameplayEffectHandle contains all relevant information about the effect.
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	// Grant the attribute sets.
	for (int32 AttributeIndex = 0; AttributeIndex < GrantedAttributes.Num(); AttributeIndex++)
	{
		const FCerberusAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[AttributeIndex];

		if (!IsValid(SetToGrant.AttributeSet))
		{
			UE_LOG(LogCerberusAbilitySystem, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid."), AttributeIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* NewSet = NewObject<UAttributeSet>(CerberusASC->GetOwner(), SetToGrant.AttributeSet);
		CerberusASC->AddAttributeSetSubobject(NewSet);
		
		// Keeping track of all given attribute sets in the struct.
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}
}
