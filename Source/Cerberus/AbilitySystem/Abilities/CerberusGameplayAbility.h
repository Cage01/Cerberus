// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CerberusGameplayAbility.generated.h"

class ULyraAbilitySystemComponent;
class ACerberusCharacter;
class ACerberusPlayerController;
class UCerberusAbilityCost;


/**
 * ECerberusAbilityActivationPolicy
 *
 *	Defines how an ability is meant to activate.
 */
UENUM(BlueprintType)
enum class ECerberusAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};

/**
 * ECerberusAbilityActivationGroup
 *
 *	Defines how an ability activates in relation to other abilities.
 */
UENUM(BlueprintType)
enum class ECerberusAbilityActivationGroup : uint8
{
	// Ability runs independently of all other abilities.
	Independent,

	// Ability is canceled and replaced by other exclusive abilities.
	Exclusive_Replaceable,

	// Ability blocks all other exclusive abilities from activating.
	Exclusive_Blocking,

	MAX	UMETA(Hidden)
};

/** Failure reason that can be used to play an animation montage when a failure occurs */
USTRUCT(BlueprintType)
struct FCerberusAbilityMontageFailureMessage
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite)
	APlayerController* PlayerController = nullptr;

	// All the reasons why this ability has failed
	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer FailureTags;

	UPROPERTY(BlueprintReadWrite)
	UAnimMontage* FailureMontage = nullptr;
};

/**
 * UCerberusGameplayAbility
 *
 * The base gameplay ability class used by the project.
 */
UCLASS(Abstract, HideCategories=(Input), meta=(ShortTooltip= "The base ability class used by the project"))
class CERBERUS_API UCerberusGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	friend class UCerberusAbilitySystemComponent;

public:
	UCerberusGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Cerberus|Ability")
	UCerberusAbilitySystemComponent* GetCerberusAbilitySystemComponentFromActorInfo() const;
	
	UFUNCTION(BlueprintCallable, Category="Cerberus|Ability")
	ACerberusPlayerController* GetCerberusPlayerControllerFromActorInfo() const;
	
	UFUNCTION(BlueprintCallable, Category="Cerberus|Ability")
	AController* GetControllerFromActorInfo() const;
	
	UFUNCTION(BlueprintCallable, Category="Cerberus|Ability")
	ACerberusCharacter* GetCerberusCharacterFromActorInfo() const;

	ECerberusAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	ECerberusAbilityActivationGroup GetActivationGroup() const { return ActivationGroup; }

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

	// Returns true if the requested activation group is a valid transition.
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category="Cerberus|Ability", meta=(ExpandBoolAsExecs = "ReturnValue"))
	bool CanChangeActivationGroup(ECerberusAbilityActivationGroup NewGroup) const;

	// Tries to change the activation group. Returns true if it successfully changed.
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category="Cerberus|Ability", meta=(ExpandBoolAsExecs = "ReturnValue"))
	bool ChangeActivationGroup(ECerberusAbilityActivationGroup NewGroup) const;

	//@TODO I think I may want this functionality, because I wanted the camera to zoom out further when weapon is sheathed 
	// // Tries to change the activation group.  Returns true if it successfully changed.
	// UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Lyra|Ability", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	// bool ChangeActivationGroup(ELyraAbilityActivationGroup NewGroup);
	//
	// // Sets the ability's camera mode.
	// UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
	// void SetCameraMode(TSubclassOf<ULyraCameraMode> CameraMode);
	
	void OnAbilityFailedToActivate(const FGameplayTagContainer& FailureReason) const
	{
		NativeOnAbilityFailedToActivate(FailureReason);
		ScriptOnAbilityFailedToActivate(FailureReason);
	}
	
protected:

	// Called when the ability fails to activate
	virtual void NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;

	// Called when the ability fails to activate
	UFUNCTION(BlueprintImplementableEvent)
	void ScriptOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;


	//~UGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void SetCanBeCanceled(bool bCanBeCanceled) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const override;
	virtual void ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec, FGameplayAbilitySpec* AbilitySpec) const override;
	virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const override;
	//~End of UGameplayAbility interface


	virtual void OnPawnAvatarSet();

	//virtual void GetAbilitySource(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, float& OutSourceLevel, const ICerberusAbilitySourceInterface*& OutAbilitySource, AActor*& OutEffectCauser) const;
	
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName="OnAbilityAdded")
	void K2_OnAbilityAdded();

	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName="OnAbilityRemoved")
	void K2_OnAbilityRemoved();

	UFUNCTION(BlueprintImplementableEvent, Category=Ability, DisplayName="OnPawnAvatarSet")
	void K2_OnPawnAvatarSet();

	
protected:
	// Defines how the ability is meant to activate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cerberus|Ability Activation")
	ECerberusAbilityActivationPolicy ActivationPolicy;

	// Defines the relationship between this ability activating and other abilities activating
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cerberus|Ability Activation")
	ECerberusAbilityActivationGroup ActivationGroup;

	// Additional costs that must be paid to activate this ability
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Costs)
	TArray<TObjectPtr<UCerberusAbilityCost>> AdditionalCosts;

	
	// Map of failure tags to simple error messages
	UPROPERTY(EditDefaultsOnly, Category = "Advanced")
	TMap<FGameplayTag, FText> FailureTagToUserFacingMessages;

	// Map of failure tags to anim montages that should be played with them
	UPROPERTY(EditDefaultsOnly, Category = "Advanced")
	TMap<FGameplayTag, UAnimMontage*> FailureTagToAnimMontage;


	// // Current camera mode set by the ability.
	// TSubclassOf<ULyraCameraMode> ActiveCameraMode;
};
