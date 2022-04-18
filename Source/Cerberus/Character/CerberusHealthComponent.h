// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "CerberusHealthComponent.generated.h"


class UCerberusAbilitySystemComponent;
class UCerberusHealthSet;
struct FGameplayEffectSpec;
struct FOnAttributeChangeData;

/*
 * ECerberusDeathState
 *
 *  Defines the current state of death
 */
UENUM(BlueprintType)
enum class ECerberusDeathState : uint8
{
	NotDead = 0,
	DeathStarted,
	DeathFinished
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))

class CERBERUS_API UCerberusHealthComponent : public UGameFrameworkComponent
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FCerberusHealth_AttributeChanged,  UCerberusHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCerberusHealth_DeathEvent, AActor*, OwningActor);
	
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCerberusHealthComponent(const FObjectInitializer& ObjectInitializer);

	// Returns the health component if one exists on the specified actor
	UFUNCTION(BlueprintPure, Category="Cerberus|Health")
	static UCerberusHealthComponent* FindHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCerberusHealthComponent>() : nullptr); }

	UFUNCTION(BlueprintCallable, Category="Cerberus|Health")
	void InitializeWithAbilitySystem(UCerberusAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category="Cerberus|Health")
	void UninitialieFromAbilitySystem();
	
	// Returns current health
	UFUNCTION(BlueprintCallable, Category="Cerberus|Health")
	float GetHealth() const;

	// Returns the maximum health value
	UFUNCTION(BlueprintCallable, Category="Cerberus|Health")
	float GetMaxHealth() const;

	// Returns current health between 0.0 and 1.0
	UFUNCTION(BlueprintCallable, Category="Cerberus|Health")
	float GetHealthNormalized() const;

	UFUNCTION(BlueprintCallable, Category="Cerberus|Health")
	ECerberusDeathState GetDeathState() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = TEXT_FALSE, Category="Cerberus|Health", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsDeadOrDying() const { return (DeathState > ECerberusDeathState::NotDead); }

	// Starts the death sequence for the owner
	virtual void StartDeath();

	// Ends the death sequence for the owner
	virtual void FinishDeath();

	// Applies enough damage to kill the owner
	virtual void DamageSelfDestruct(bool bFellOutOfWorld = false);

public:
	// Delegate fired when health value has changed
	UPROPERTY(BlueprintAssignable)
	FCerberusHealth_AttributeChanged OnHealthChanged;
	// Delegate fired when max health value has changed
	UPROPERTY(BlueprintAssignable)
	FCerberusHealth_AttributeChanged OnMaxHealthChanged;
	// Delegate fired when death instance has started
	UPROPERTY(BlueprintAssignable)
	FCerberusHealth_DeathEvent OnDeathStarted;
	// Delegate fired when death instance has finished
	UPROPERTY(BlueprintAssignable)
	FCerberusHealth_DeathEvent OnDeathFinished;
	
protected:

	virtual void OnUnregister() override;

	void ClearGameplayTags();

	// @TODO Do these need to be here? They only send out notifications that values have changed. When the ability set does the same thing.
	virtual void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void HandleMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude);

	UFUNCTION()
	virtual void OnRep_DeathState(ECerberusDeathState OldDeathState);

	
protected:
	UPROPERTY()
	UCerberusAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	const UCerberusHealthSet* HealthSet;

	UPROPERTY(ReplicatedUsing = OnRep_DeathState)
	ECerberusDeathState DeathState;
};
