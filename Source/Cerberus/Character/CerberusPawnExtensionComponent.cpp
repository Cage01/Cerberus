// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusPawnExtensionComponent.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"

UCerberusPawnExtensionComponent::UCerberusPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	
	bPawnReadyToInitialize = false;
}


void UCerberusPawnExtensionComponent::HandleControllerChanged()
{
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
