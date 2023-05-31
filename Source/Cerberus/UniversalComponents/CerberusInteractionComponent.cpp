// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusInteractionComponent.h"

#include "Cerberus/Character/CerberusCharacter.h"
#include "Cerberus/Player/CerberusPlayerController.h"
#include "Cerberus/Widgets/CerberusInteractionWidget.h"
#include "Net/UnrealNetwork.h"

UCerberusInteractionComponent::UCerberusInteractionComponent()
{
	SetComponentTickEnabled(false);
	SetIsReplicated(true);

	InteractionTime = 0.0f;
	InteractionDistance = 200.0f;
	NameText = FText::FromString("Interactable Object");
	ActionText = FText::FromString("Interact");
	bAllowMultipleInteractors = true;
	bIsInteractable = true;
	OutlineColorValue = 1;

	Space = EWidgetSpace::Screen;
	DrawSize = FIntPoint(600,100);
	bDrawAtDesiredSize = true;

	SetActive(true);
	SetHiddenInGame(true);
}

void UCerberusInteractionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCerberusInteractionComponent, bIsInteractable);
}

void UCerberusInteractionComponent::RefreshWidget()
{
	if (!bHiddenInGame && GetOwner()->GetNetMode() != NM_DedicatedServer)
	{
		// Make sure the widget is initialized, and that we are displaying the right values
		if (UCerberusInteractionWidget* InteractionWidget = Cast<UCerberusInteractionWidget>(GetUserWidgetObject()))
		{
			// Update the UI, will fire a BP event
			InteractionWidget->UpdateInteractionWidget(this);
		}
	}
}

void UCerberusInteractionComponent::BeginFocus(ACerberusCharacter* Character)
{
	if(!IsActive() || !GetOwner() || !Character || !bIsInteractable)
		return;

	OnBeginFocus.Broadcast(Character);
	
	if (Character->GetCerberusPlayerController()->IsLocalController())
	{
		SetHiddenInGame(false);
		if (OutlineColorValue > 0)
		{
			ToggleOutline(OutlineColorValue);
		}
	}
	
	RefreshWidget();
}

void UCerberusInteractionComponent::EndFocus(ACerberusCharacter* Character)
{
	OnEndFocus.Broadcast(Character);
	
	if (Character->GetCerberusPlayerController()->IsLocalController())
	{
		SetHiddenInGame(true);
		if (OutlineColorValue > 0)
		{
			ToggleOutline(0);
		}
	}
}

void UCerberusInteractionComponent::BeginInteract(ACerberusCharacter* Character)
{
	if (CanInteract(Character))
	{
		Interactors.AddUnique(Character);
		OnBeginInteract.Broadcast(Character);
	}
}

void UCerberusInteractionComponent::EndInteract(ACerberusCharacter* Character)
{
	Interactors.RemoveSingle(Character);
	OnEndInteract.Broadcast(Character);
}

void UCerberusInteractionComponent::Interact(ACerberusCharacter* Character)
{
	if (CanInteract(Character))
	{
		OnInteract.Broadcast(Character);
	}
}

float UCerberusInteractionComponent::GetInteractPercentage()
{
	 if (Interactors.IsValidIndex(0))
	 {
		 if (ACerberusCharacter* Interactor = Interactors[0])
		 {
			 return 1.f - FMath::Abs(Interactor->GetRemainingInteractTime() / InteractionTime);
		 }
	 }

	return 0.0f;
}

void UCerberusInteractionComponent::SetInteractableNameText(const FText& NewNameText)
{
	NameText = NewNameText;
	RefreshWidget();
}

void UCerberusInteractionComponent::SetInteractableActionText(const FText& NewActionText)
{
	ActionText = NewActionText;
	RefreshWidget();
}

void UCerberusInteractionComponent::Deactivate()
{
	Super::Deactivate();

	for (int i = Interactors.Num() - 1; i >= 0; --i)
	{
		if (ACerberusCharacter* Interactor = Interactors[i])
		{
			EndFocus(Interactor);
			EndInteract(Interactor);
		}
	}

	Interactors.Empty();
}

bool UCerberusInteractionComponent::CanInteract(ACerberusCharacter* Character) const
{
	const bool bPlayerAlreadyInteracting = !bAllowMultipleInteractors && Interactors.Num() >= 1;
	return !bPlayerAlreadyInteracting && IsActive() && GetOwner() != nullptr && Character != nullptr && bIsInteractable;
}

void UCerberusInteractionComponent::ToggleOutline(int32 Value) const
{
	for (auto& VisualComp : GetOwner()->GetComponents())
	{
		if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(VisualComp))
		{
			if (Value > 0)
				PrimitiveComponent->SetRenderCustomDepth(true);
			else
				PrimitiveComponent->SetRenderCustomDepth(false);
			
			PrimitiveComponent->SetCustomDepthStencilValue(Value);
		}
	}
}


#if WITH_EDITOR

void UCerberusInteractionComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// If a new pickup is selected in the property editor, change the mesh to reflect the new item being selected
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UCerberusInteractionComponent, OutlineColorValue))
	{
		
		if (OutlineColorValue > 0)
		{
			//Toggle on outline to preview colors in the editor
			ToggleOutline(OutlineColorValue);

			//After 2 seconds turn the properties off for use in-game
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(this, FName("ToggleOutline"), 0);
			
			
			if (GetOwner() && GetWorld())
			{
				FTimerHandle TimerHandle;
				GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 2.0f, false);
			}
			
		}
	}
}
#endif
