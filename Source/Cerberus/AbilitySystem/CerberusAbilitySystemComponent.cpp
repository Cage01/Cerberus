// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusAbilitySystemComponent.h"

#include "CerberusAbilityTagRelationshipMapping.h"
#include "Cerberus/CerberusLogChannels.h"
#include "Cerberus/System/CerberusAssetManager.h"
#include "Cerberus/System/CerberusGameData.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked");


UCerberusAbilitySystemComponent::UCerberusAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	InputHeldSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();

	FMemory::Memset(ActivationGroupCounts, 0, sizeof(ActivationGroupCounts));
}

void UCerberusAbilitySystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//@TODO Lyra unregisters the ASC from the world itself which inherits the WorldSubsystem class. Doesnt seem applicable here.
}

void UCerberusAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	check(ActorInfo);
	check(InOwnerActor);

	const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);

	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bHasNewPawnAvatar)
	{
		// Notify all abilities tht a new pawn avatar has been set
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			UCerberusGameplayAbility* CerberusAbilityCDO = Cast<UCerberusGameplayAbility>(AbilitySpec.Ability);

			if (CerberusAbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
			{
				TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
				for (UGameplayAbility* AbilityInstance : Instances)
				{
					//Send out an event that can be picked up in blueprints to activate off of
					UCerberusGameplayAbility* CerberusAbilityInstance = CastChecked<UCerberusGameplayAbility>(AbilityInstance);
					CerberusAbilityInstance->OnPawnAvatarSet();
				}
			} else
			{
				CerberusAbilityCDO->OnPawnAvatarSet();
			}
		}

		//@TODO Lyra registers with the global ability system here

		TryActivateAbilitiesOnSpawn();
	}
}


void UCerberusAbilitySystemComponent::TryActivateAbilitiesOnSpawn()
{
	ABILITYLIST_SCOPE_LOCK()
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		const UCerberusGameplayAbility* CerberusAbilityCDO = Cast<UCerberusGameplayAbility>(AbilitySpec.Ability);
		CerberusAbilityCDO->TryActivateAbilityOnSpawn(AbilityActorInfo.Get(), AbilitySpec);
	}
}

void UCerberusAbilitySystemComponent::CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility)
{
	ABILITYLIST_SCOPE_LOCK()
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (!AbilitySpec.IsActive())
		{
			continue;
		}

		UCerberusGameplayAbility* CerberusAbilityCDO = Cast<UCerberusGameplayAbility>(AbilitySpec.Ability);
		TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
		if (CerberusAbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
		{
			// Cancel all the spawned instances, not the CDO.
			for (UGameplayAbility* AbilityInstance : Instances)
			{
				UCerberusGameplayAbility* CerberusAbilityInstance = CastChecked<UCerberusGameplayAbility>(AbilityInstance);
				if (ShouldCancelFunc(CerberusAbilityInstance, AbilitySpec.Handle))
				{
					if (CerberusAbilityInstance->CanBeCanceled())
					{
						CerberusAbilityInstance->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(), CerberusAbilityInstance->GetCurrentActivationInfo(), bReplicateCancelAbility);
					} else
					{
						UE_LOG(LogCerberusAbilitySystem, Error, TEXT("CancelAbilitiesByFunc: Can't cancel ability [%s] because CanBeCanceled is false."), *CerberusAbilityInstance->GetName() )
					}
				}
			}
		} else
		{
			// Cancel the non-instanced ability CDO.
			if (ShouldCancelFunc(CerberusAbilityCDO, AbilitySpec.Handle))
			{
				// Non-instanced abilities can always be cancelled 
				check(CerberusAbilityCDO->CanBeCanceled())
				CerberusAbilityCDO->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(), CerberusAbilityCDO->GetCurrentActivationInfo(), bReplicateCancelAbility);
			}
			
		}
	}
}

void UCerberusAbilitySystemComponent::CancelInputActivatedAbilities(bool bReplicateCancelAbility)
{
	TShouldCancelAbilityFunc ShouldCancelFunc = [this](const UCerberusGameplayAbility* CerberusAbility, FGameplayAbilitySpecHandle Handle)
	{
		const ECerberusAbilityActivationPolicy ActivationPolicy = CerberusAbility->GetActivationPolicy();
		return ((ActivationPolicy == ECerberusAbilityActivationPolicy::OnInputTriggered) || (ActivationPolicy == ECerberusAbilityActivationPolicy::WhileInputActive));
	};

	CancelAbilitiesByFunc(ShouldCancelFunc, bReplicateCancelAbility);
}

void UCerberusAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputPress ability task works.
	if (Spec.IsActive())
	{
		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UCerberusAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputRelease ability task works.
	if (Spec.IsActive())
	{
		// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UCerberusAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UCerberusAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UCerberusAbilitySystemComponent::ProcessAbilityInput(float DeltaTime)
{
	if (HasMatchingGameplayTag(TAG_Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}

	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();


	//
	// Process all abilities that activate when the input is held.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UCerberusGameplayAbility* CerberusAbilityCDO = CastChecked<UCerberusGameplayAbility>(AbilitySpec->Ability);

				if (CerberusAbilityCDO->GetActivationPolicy() == ECerberusAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}	
	}

	//
	// Process all abilities that activate when the input is pressed.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputPressed(*AbilitySpec);
				} else
				{
					const UCerberusGameplayAbility* CerberusAbilityCDO = CastChecked<UCerberusGameplayAbility>(AbilitySpec->Ability);

					if (CerberusAbilityCDO->GetActivationPolicy() == ECerberusAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	//
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	//
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	//
	// Process all abilities that had their input released this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	//
	// Clear the cached ability handles.
	//
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UCerberusAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UCerberusAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability)
{
	Super::NotifyAbilityActivated(Handle, Ability);

	UCerberusGameplayAbility* CerberusAbility = CastChecked<UCerberusGameplayAbility>(Ability);
	AddAbilityToActivationGroup(CerberusAbility->GetActivationGroup(), CerberusAbility);
}

void UCerberusAbilitySystemComponent::NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	Super::NotifyAbilityFailed(Handle, Ability, FailureReason);

	if (APawn* Avatar = Cast<APawn>(GetAvatarActor()))
	{
		if (!Avatar->IsLocallyControlled() && Ability->IsSupportedForNetworking())
		{
			ClientNotifyAbilityFailed(Ability, FailureReason);
		}
	}

	HandleAbilityFailed(Ability, FailureReason);
}

void UCerberusAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled)
{
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);

	UCerberusGameplayAbility* CerberusAbility = CastChecked<UCerberusGameplayAbility>(Ability);
	RemoveAbilityFromActivationGroup(CerberusAbility->GetActivationGroup(), CerberusAbility);
}

void UCerberusAbilitySystemComponent::ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags)
{
	FGameplayTagContainer ModifiedBlockTags = BlockTags;
	FGameplayTagContainer ModifiedCancelTags = CancelTags;

	if (TagRelationshipMapping)
	{
		TagRelationshipMapping->GetAbilityTagsToBlockAndCancel(AbilityTags, &ModifiedBlockTags, &ModifiedCancelTags);
	}
	
	Super::ApplyAbilityBlockAndCancelTags(AbilityTags, RequestingAbility, bEnableBlockTags, BlockTags, bExecuteCancelTags, CancelTags);

	//@TODO: Apply any special logic like blocking input or movement
}

void UCerberusAbilitySystemComponent::HandleChangeAbilityCanBeCanceled(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bCanBeCanceled)
{
	Super::HandleChangeAbilityCanBeCanceled(AbilityTags, RequestingAbility, bCanBeCanceled);

	//@TODO: Apply any special logic like blocking input or movement
}

void UCerberusAbilitySystemComponent::GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const
{
	if (TagRelationshipMapping)
	{
		TagRelationshipMapping->GetRequiredAndBlockedActivationTags(AbilityTags, &OutActivationRequired, &OutActivationBlocked);
	}
}

void UCerberusAbilitySystemComponent::SetTagRelationshipMapping(UCerberusAbilityTagRelationshipMapping* NewMapping)
{
	TagRelationshipMapping = NewMapping;
}


void UCerberusAbilitySystemComponent::ClientNotifyAbilityFailed_Implementation(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	HandleAbilityFailed(Ability, FailureReason);
}

void UCerberusAbilitySystemComponent::HandleAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	if (const UCerberusGameplayAbility* CerberusAbility = Cast<UCerberusGameplayAbility>(Ability))
	{
		CerberusAbility->OnAbilityFailedToActivate(FailureReason);
	}
}


bool UCerberusAbilitySystemComponent::IsActivationGroupBlocked(ECerberusAbilityActivationGroup Group) const
{
	bool bBlocked = false;

	switch (Group)
	{
	case ECerberusAbilityActivationGroup::Independent:
		// Independent abilities are never blocked
		bBlocked = false;
		break;
	case ECerberusAbilityActivationGroup::Exclusive_Replaceable:
	case ECerberusAbilityActivationGroup::Exclusive_Blocking:
		// Exclusive abilities can activate if nothing is blocking
		bBlocked = (ActivationGroupCounts[(uint8)ECerberusAbilityActivationGroup::Exclusive_Blocking] > 0);
		break;

	default:
		checkf(false, TEXT("IsActivationGroupBlocked: Invalid ActivationGroup [%d]\n"), (uint8)Group);
		break;
	}

	return bBlocked;
}

void UCerberusAbilitySystemComponent::AddAbilityToActivationGroup(ECerberusAbilityActivationGroup Group, UCerberusGameplayAbility* CerberusAbility)
{
	check(CerberusAbility);
	check(ActivationGroupCounts[(uint8)Group] < INT32_MAX);

	ActivationGroupCounts[(uint8)Group]++;

	const bool bReplicateCancelAbility = false;

	switch (Group)
	{
	case ECerberusAbilityActivationGroup::Independent:
		// Independent abilities do not cancel any other abilities.
		break;
	case ECerberusAbilityActivationGroup::Exclusive_Replaceable:
	case ECerberusAbilityActivationGroup::Exclusive_Blocking:
		CancelActivationGroupAbilities(ECerberusAbilityActivationGroup::Exclusive_Replaceable, CerberusAbility, bReplicateCancelAbility);
		break;
	default:
		checkf(false, TEXT("ddAbilityToActivationGroup: Invalid ActivationGroup [%d]\n"), (uint8)Group);
		break;
	}

	const int32 ExclusiveCount = ActivationGroupCounts[(uint8)ECerberusAbilityActivationGroup::Exclusive_Replaceable] + ActivationGroupCounts[(uint8)ECerberusAbilityActivationGroup::Exclusive_Blocking];
	if (!ensure(ExclusiveCount <= 1))
	{
		UE_LOG(LogCerberusAbilitySystem, Error, TEXT("AddAbilityToActivationGroup: Multiple exclusive abilities are running."))
	}
}

void UCerberusAbilitySystemComponent::RemoveAbilityFromActivationGroup(ECerberusAbilityActivationGroup Group, UCerberusGameplayAbility* CerberusAbility)
{
	check(CerberusAbility)
	check(ActivationGroupCounts[(uint8)Group] > 0);

	ActivationGroupCounts[(uint8)Group]--;
}

void UCerberusAbilitySystemComponent::CancelActivationGroupAbilities(ECerberusAbilityActivationGroup Group, UCerberusGameplayAbility* IgnoreCerberusAbility, bool bReplicateCancelAbility)
{
	TShouldCancelAbilityFunc ShouldCancelFunc = [this, Group, IgnoreCerberusAbility](const UCerberusGameplayAbility* CerberusAbility, FGameplayAbilitySpecHandle Handle)
	{
		return ((CerberusAbility->GetActivationGroup() == Group) && (CerberusAbility != IgnoreCerberusAbility));
	};

	CancelAbilitiesByFunc(ShouldCancelFunc, bReplicateCancelAbility);
}

void UCerberusAbilitySystemComponent::AddDynamicTagGameplayEffect(const FGameplayTag& Tag)
{
	//@TODO create asset manager
	//const TSubclassOf<UGameplayEffect> DynamicTagGE = UCerberusAssetManager::GetSubclassOf(UCerberusGameData::Get())

	//const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec()
}

void UCerberusAbilitySystemComponent::RemoveDynamicTagGameplayEffect(const FGameplayTag& Tag)
{
	//@TODO Create asset manager
}

void UCerberusAbilitySystemComponent::GetAbilityTargetData(const FGameplayAbilitySpecHandle AbilityHandle, FGameplayAbilityActivationInfo ActivationInfo, FGameplayAbilityTargetDataHandle& OutTargetDataHandle)
{
	TSharedPtr<FAbilityReplicatedDataCache> ReplicatedData = AbilityTargetDataMap.Find(FGameplayAbilitySpecHandleAndPredictionKey(AbilityHandle, ActivationInfo.GetActivationPredictionKey()));
	if (ReplicatedData.IsValid())
	{
		OutTargetDataHandle = ReplicatedData->TargetData;
	}
}
