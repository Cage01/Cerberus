// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cerberus/AbilitySystem/Attributes/CerberusHealthSet.h"
#include "Components/GameFrameworkComponent.h"
#include "CerberusHealthComponent.generated.h"

struct FGameplayEffectSpec;
struct FOnAttributeChangeData;
class UCerberusAbilitySystemComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCerberusHealth_DeathEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FCerberusHealth_AttributeChanged, UCerberusHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);


/**
 * ECerberusDeathState
 *
 *	Defines current state of death.
 */
UENUM(BlueprintType)
enum class ECerberusDeathState : uint8
{
	NotDead = 0,
	DeathStarted,
	DeathFinished
};



/**
 * UCerberusHealthComponent
 *
 *	An Actor component used to handle anything related to health.
 *	
 *	Any object in the game that you want to have a health attribute and use all functionality related to health should have this component
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class CERBERUS_API UCerberusHealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:

	UCerberusHealthComponent(const FObjectInitializer& ObjectInitializer);

	/** A static function that will attempt to return the health component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Cerberus|Health")
	static UCerberusHealthComponent* FindHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCerberusHealthComponent>() : nullptr); }

	/** Initialize the component using an ability system component. */
	UFUNCTION(BlueprintCallable, Category = "Cerberus|Health")
	void InitializeWithAbilitySystem(UCerberusAbilitySystemComponent* InASC);

	/** Uninitialize the component, clearing any references to the ability system. */
	UFUNCTION(BlueprintCallable, Category = "Cerberus|Health")
	void UninitializeFromAbilitySystem();

	/** Returns the current health value. */
	UFUNCTION(BlueprintCallable, Category = "Cerberus|Health")
	float GetHealth() const;

	/** Returns the current maximum health value. */
	UFUNCTION(BlueprintCallable, Category = "Cerberus|Health")
	float GetMaxHealth() const;

	/** Returns the current health in the range [0.0, 1.0]. To be used to update UI elements such as progress bars*/
	UFUNCTION(BlueprintCallable, Category = "Cerberus|Health")
	float GetHealthNormalized() const;

	UFUNCTION(BlueprintCallable, Category = "Cerberus|Health")
	ECerberusDeathState GetDeathState() const { return DeathState; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Cerberus|Health", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsDeadOrDying() const { return (DeathState > ECerberusDeathState::NotDead); }

	/** Begins the death sequence for the owner. */
	virtual void StartDeath();

	/** Ends the death sequence for the owner. */
	virtual void FinishDeath();

	/** Applies enough damage to kill the owner. */
	virtual void DamageSelfDestruct(bool bFellOutOfWorld = false);

public:

	/** Delegate fired when the health value has changed. */
	UPROPERTY(BlueprintAssignable)
	FCerberusHealth_AttributeChanged OnHealthChanged;

	/** Delegate fired when the max health value has changed. */
	UPROPERTY(BlueprintAssignable)
	FCerberusHealth_AttributeChanged OnMaxHealthChanged;

	/** Delegate fired when the death sequence has started. */
	UPROPERTY(BlueprintAssignable)
	FCerberusHealth_DeathEvent OnDeathStarted;

	/** Delegate fired when the death sequence has finished. */
	UPROPERTY(BlueprintAssignable)
	FCerberusHealth_DeathEvent OnDeathFinished;

protected:

	virtual void OnUnregister() override;

	void ClearGameplayTags();

	virtual void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void HandleMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude);

	UFUNCTION()
	virtual void OnRep_DeathState(ECerberusDeathState OldDeathState);

protected:

	// Ability system used by this component.
	UPROPERTY()
	UCerberusAbilitySystemComponent* AbilitySystemComponent;

	// Health set used by this component.
	UPROPERTY()
	const UCerberusHealthSet* HealthSet;

	// Replicated state used to handle dying.
	UPROPERTY(ReplicatedUsing = OnRep_DeathState)
	ECerberusDeathState DeathState;
};
