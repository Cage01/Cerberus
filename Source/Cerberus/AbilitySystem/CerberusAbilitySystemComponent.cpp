// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusAbilitySystemComponent.h"

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
	}
}

void UCerberusAbilitySystemComponent::CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility)
{
}

void UCerberusAbilitySystemComponent::CancelInputActivatedAbilities(bool bReplicateCancelAbility)
{
}

void UCerberusAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
}

void UCerberusAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
}

void UCerberusAbilitySystemComponent::ProcessAbilityInput(float DeltaTime)
{
}

void UCerberusAbilitySystemComponent::ClearAbilityInput()
{
}

bool UCerberusAbilitySystemComponent::IsActivationGroupBlocked(ECerberusAbilityActivationGroup Group) const
{
	return false; //tmp
}

void UCerberusAbilitySystemComponent::AddAbilityToActivationGroup(ECerberusAbilityActivationGroup Group, UCerberusGameplayAbility* CerberusAbility)
{
}

void UCerberusAbilitySystemComponent::RemoveAbilityFromActivationGroup(ECerberusAbilityActivationGroup Group, UCerberusGameplayAbility* CerberusAbility)
{
}

void UCerberusAbilitySystemComponent::CancelActivationGroupAbilities(ECerberusAbilityActivationGroup Group, UCerberusGameplayAbility* IgnoreCerberusAbility, bool bReplicateCancelAbility)
{
}

void UCerberusAbilitySystemComponent::AddDynamicTagGameplayEffect(const FGameplayTag& Tag)
{
}

void UCerberusAbilitySystemComponent::RemoveDynamicTagGameplayEffect(const FGameplayTag& Tag)
{
}

void UCerberusAbilitySystemComponent::SetTagRelationshipMapping(UCerberusAbilityTagRelationshipMapping* NewMapping)
{
}

void UCerberusAbilitySystemComponent::GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const
{
}

void UCerberusAbilitySystemComponent::TryActivateAbilitiesOnSpawn()
{
}

void UCerberusAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);
}

void UCerberusAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);
}

void UCerberusAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability)
{
	Super::NotifyAbilityActivated(Handle, Ability);
}

void UCerberusAbilitySystemComponent::NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	Super::NotifyAbilityFailed(Handle, Ability, FailureReason);
}

void UCerberusAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled)
{
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);
}

void UCerberusAbilitySystemComponent::ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags)
{
	Super::ApplyAbilityBlockAndCancelTags(AbilityTags, RequestingAbility, bEnableBlockTags, BlockTags,
	                                      bExecuteCancelTags, CancelTags);
}

void UCerberusAbilitySystemComponent::HandleChangeAbilityCanBeCanceled(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bCanBeCanceled)
{
	Super::HandleChangeAbilityCanBeCanceled(AbilityTags, RequestingAbility, bCanBeCanceled);
}

void UCerberusAbilitySystemComponent::ClientNotifyAbilityFailed_Implementation(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	HandleAbilityFailed(Ability, FailureReason);
}

void UCerberusAbilitySystemComponent::HandleAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
}
