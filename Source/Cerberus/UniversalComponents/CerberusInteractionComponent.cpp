// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusInteractionComponent.h"

#include "Cerberus/Character/CerberusCharacter.h"
#include "Cerberus/Widgets/CerberusInteractionWidget.h"

UCerberusInteractionComponent::UCerberusInteractionComponent()
{
	SetComponentTickEnabled(false);

	InteractionTime = 0.0f;
	InteractionDistance = 200.0f;
	NameText = FText::FromString("Interactable Object");
	ActionText = FText::FromString("Interact");
	bAllowMultipleInteractors = true;

	Space = EWidgetSpace::Screen;
	DrawSize = FIntPoint(600,100);
	bDrawAtDesiredSize = true;

	SetActive(true);
	SetHiddenInGame(true);
}

void UCerberusInteractionComponent::RefreshWidget()
{
	if (!bHiddenInGame && GetOwner()->GetNetMode() != NM_DedicatedServer)
	{
		// Make sure the widget is initialized, and that we are displaying the right values
		if (UCerberusInteractionWidget* InteractionWidget = Cast<UCerberusInteractionWidget>(GetUserWidgetObject()))
		{
			InteractionWidget->UpdateInteractionWidget(this);
		}
	}
}

void UCerberusInteractionComponent::BeginFocus(ACerberusCharacter* Character)
{
	if(!IsActive() || !GetOwner() || !Character)
		return;

	OnBeginFocus.Broadcast(Character);
	
	SetHiddenInGame(false);

	if (!GetOwner()->HasAuthority())
	{
		for (auto& VisualComp : GetOwner()->GetComponents())
		{
			if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(VisualComp))
			{
				if (PrimitiveComponent->ComponentHasTag("interactable.outline"))
				{
					PrimitiveComponent->SetRenderCustomDepth(true);
				}
				
			}
		}
	}
	
	RefreshWidget();
}

void UCerberusInteractionComponent::EndFocus(ACerberusCharacter* Character)
{
	if(!IsActive() || !GetOwner() || !Character)
		return;

	OnEndFocus.Broadcast(Character);

	SetHiddenInGame(true);

	if (!GetOwner()->HasAuthority())
	{
		for (auto& VisualComp : GetOwner()->GetComponents())
		{
			if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(VisualComp))
			{
				if (PrimitiveComponent->ComponentHasTag("interactable.outline"))
				{
					PrimitiveComponent->SetRenderCustomDepth(false);
				}
				
			}
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

	return 0;
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

void UCerberusInteractionComponent::SetInteractableActionType(const FText& NewActionType)
{
	ActionType = NewActionType;
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
	return !bPlayerAlreadyInteracting && IsActive() && GetOwner() != nullptr && Character != nullptr;
}
