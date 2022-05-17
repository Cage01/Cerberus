// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CerberusAbilitySet.generated.h"

class UCerberusAbilitySystemComponent;
class UCerberusGameplayAbility;
class UGameplayEffect;

/**
 * FCerberusAbilitySet_GameplayAbility
 *
 *	Data used by the ability set to grant gameplay abilities.
 */
USTRUCT(BlueprintType)
struct FCerberusAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCerberusGameplayAbility> Ability = nullptr;

	// Level of ability to grant.
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
	
};

/**
 * FCerberusAbilitySet_GameplayEffect
 *
 *	Data used by the ability set to grant gameplay effects.
 */
USTRUCT(BlueprintType)
struct FCerberusAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

/**
 * FCerberusAbilitySet_AttributeSet
 *
 *	Data used by the ability set to grant attribute sets.
 */
USTRUCT(BlueprintType)
struct FCerberusAbilitySet_AttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;
};

USTRUCT(BlueprintType)
struct FCerberusAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	/* Removes ability from ability system. Must be from an authoritative source */
	void TakeFromAbilitySystem(UCerberusAbilitySystemComponent* CerberusASC);

protected:
	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	// Pointers to the granted attribute sets
	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};

/**
 * UCerberusAbilitySet
 *
 * Non-mutable data asset used to grant gameplay abilities and gameplay effects.
 */
UCLASS(BlueprintType, Const)
class UCerberusAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCerberusAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* Grants the ability set to the specified ability system component.
	* The returned handles can be used later to take away anything that was granted.
	*		Notes:
	*		- Must be from an authoritative source
	*/
	void GiveToAbilitySystem(UCerberusAbilitySystemComponent* CerberusASC, FCerberusAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:
	/* Gameplay abilities to grant when this ability set is granted. */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<FCerberusAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	/* Gameplay effects to grant when this ability set is granted. */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta=(TitleProperty=GameplayEffect))
	TArray<FCerberusAbilitySet_GameplayEffect> GrantedGameplayEffects;

	/* Attribute sets to grant when this ability set is granted. */
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta=(TitleProperty=AttributeSet))
	TArray<FCerberusAbilitySet_AttributeSet> GrantedAttributes;
	
};
