// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusPawnExtensionComponent.h"

#include "Cerberus/CerberusLogChannels.h"
#include "Cerberus/AbilitySystem/CerberusAbilitySystemComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"

UCerberusPawnExtensionComponent::UCerberusPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	//PawnData = nullptr;
	AbilitySystemComponent = nullptr;
	bPawnReadyToInitialize = false;
}

void UCerberusPawnExtensionComponent::InitializeAbilitySystem(UCerberusAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	// check(InASC);
	// check(InOwnerActor);
	//
	// if (AbilitySystemComponent == InASC)
	// {
	// 	// The ability system component hasn't changed.
	// 	return;
	// }
	//
	// if (AbilitySystemComponent)
	// {
	// 	// Clean up the old ability system component.
	// 	UninitializeAbilitySystem();
	// }
	//
	// APawn* Pawn = GetPawnChecked<APawn>();
	// AActor* ExistingAvatar = InASC->GetAvatarActor();
	//
	// UE_LOG(LogCerberus, Verbose, TEXT("Setting up ASC [%s] on pawn [%s] owner [%s], existing [%s] "), *GetNameSafe(InASC), *GetNameSafe(Pawn), *GetNameSafe(InOwnerActor), *GetNameSafe(ExistingAvatar));
	//
	// if ((ExistingAvatar != nullptr) && (ExistingAvatar != Pawn))
	// {
	// 	UE_LOG(LogCerberus, Log, TEXT("Existing avatar (authority=%d)"), ExistingAvatar->HasAuthority() ? 1 : 0);
	//
	// 	// There is already a pawn acting as the ASC's avatar, so we need to kick it out
	// 	// This can happen on clients if they're lagged: their new pawn is spawned + possessed before the dead one is removed
	// 	ensure(!ExistingAvatar->HasAuthority());
	//
	// 	if (UCerberusPawnExtensionComponent* OtherExtensionComponent = FindPawnExtensionComponent(ExistingAvatar))
	// 	{
	// 		OtherExtensionComponent->UninitializeAbilitySystem();
	// 	}
	// }
	//
	// AbilitySystemComponent = InASC;
	// AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);
	//
	// // if (ensure(PawnData))
	// // {
	// // 	InASC->SetTagRelationshipMapping(PawnData->TagRelationshipMapping);
	// // }

	OnAbilitySystemInitialized.Broadcast();
}

void UCerberusPawnExtensionComponent::UninitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		AbilitySystemComponent->CancelAbilities(nullptr, nullptr);
		//AbilitySystemComponent->ClearAbilityInput();
		AbilitySystemComponent->RemoveAllGameplayCues();

		if (AbilitySystemComponent->GetOwnerActor() != nullptr)
		{
			AbilitySystemComponent->SetAvatarActor(nullptr);
		}
		else
		{
			// If the ASC doesn't have a valid owner, we need to clear *all* actor info, not just the avatar pairing
			AbilitySystemComponent->ClearActorInfo();
		}

		OnAbilitySystemUninitialized.Broadcast();
	}

	AbilitySystemComponent = nullptr;
}

void UCerberusPawnExtensionComponent::HandleControllerChanged()
{
	// if (AbilitySystemComponent && (AbilitySystemComponent->GetAvatarActor() == GetPawnChecked<APawn>()))
	// {
	// 	ensure(AbilitySystemComponent->AbilityActorInfo->OwnerActor == AbilitySystemComponent->GetOwnerActor());
	// 	if (AbilitySystemComponent->GetOwnerActor() == nullptr)
	// 	{
	// 		UninitializeAbilitySystem();
	// 	}
	// 	else
	// 	{
	// 		AbilitySystemComponent->RefreshAbilityActorInfo();
	// 	}
	// }

	CheckPawnReadyToInitialize();
}

void UCerberusPawnExtensionComponent::HandlePlayerStateReplicated()
{
	CheckPawnReadyToInitialize();
}

void UCerberusPawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckPawnReadyToInitialize();
}

bool UCerberusPawnExtensionComponent::CheckPawnReadyToInitialize()
{
	// if (bPawnReadyToInitialize)
	// {
	// 	return true;
	// }
	//
	// // Pawn data is required.
	// // if (!PawnData)
	// // {
	// // 	return false;
	// // }
	//
	// APawn* Pawn = GetPawnChecked<APawn>();
	//
	// const bool bHasAuthority = Pawn->HasAuthority();
	// const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
	//
	// if (bHasAuthority || bIsLocallyControlled)
	// {
	// 	// Check for being possessed by a controller.
	// 	if (!GetController<AController>())
	// 	{
	// 		return false;
	// 	}
	// }
	//
	// // Allow pawn components to have requirements.
	// TArray<UActorComponent*> InteractableComponents = Pawn->GetComponentsByInterface(UCerberusReadyInterface::StaticClass());
	// for (UActorComponent* InteractableComponent : InteractableComponents)
	// {
	// 	const ICerberusReadyInterface* Ready = CastChecked<ICerberusReadyInterface>(InteractableComponent);
	// 	if (!Ready->IsPawnComponentReadyToInitialize())
	// 	{
	// 		return false;
	// 	}
	// }
	//
	// // Pawn is ready to initialize.
	// bPawnReadyToInitialize = true;
	// OnPawnReadyToInitialize.Broadcast();
	// BP_OnPawnReadyToInitialize.Broadcast();

	return true;
}

void UCerberusPawnExtensionComponent::OnPawnReadyToInitialize_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnPawnReadyToInitialize.IsBoundToObject(Delegate.GetUObject()))
	{
		OnPawnReadyToInitialize.Add(Delegate);
	}

	if (bPawnReadyToInitialize)
	{
		Delegate.Execute();
	}
}

void UCerberusPawnExtensionComponent::OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemInitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemInitialized.Add(Delegate);
	}

	if (AbilitySystemComponent)
	{
		Delegate.Execute();
	}
}

void UCerberusPawnExtensionComponent::OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemUninitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemUninitialized.Add(Delegate);
	}
}

void UCerberusPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();
	//
	// const APawn* Pawn = GetPawn<APawn>();
	// ensureAlwaysMsgf((Pawn != nullptr), TEXT("LyraPawnExtensionComponent on [%s] can only be added to Pawn actors."), *GetNameSafe(GetOwner()));
	//
	// TArray<UActorComponent*> PawnExtensionComponents;
	// Pawn->GetComponents(UCerberusPawnExtensionComponent::StaticClass(), PawnExtensionComponents);
	// ensureAlwaysMsgf((PawnExtensionComponents.Num() == 1), TEXT("Only one LyraPawnExtensionComponent should exist on [%s]."), *GetNameSafe(GetOwner()));

}

// void UCerberusPawnExtensionComponent::OnRep_PawnData()
// {
// }
