// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusPickup.h"

#include "Cerberus/CerberusLogChannels.h"
#include "Cerberus/Character/CerberusCharacter.h"
#include "Cerberus/Inventory/CerberusInventoryComponent.h"
#include "Cerberus/Inventory/Items/CerberusItem.h"
#include "Cerberus/UniversalComponents/CerberusInteractionComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

// Sets default values
ACerberusPickup::ACerberusPickup()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	
	SetRootComponent(PickupMesh);

	InteractionComponent = CreateDefaultSubobject<UCerberusInteractionComponent>("PickupInteractionComponent");
	InteractionComponent->InteractionTime = 0.5f;
	InteractionComponent->InteractionDistance = 200.0f;
	InteractionComponent->NameText = FText::FromString("Pickup");
	InteractionComponent->ActionText = FText::FromString("Take");
	InteractionComponent->OnInteract.AddDynamic(this, &ACerberusPickup::OnTakePickup);
	InteractionComponent->OutlineColorValue = 255; //White
	InteractionComponent->SetupAttachment(PickupMesh);

	SetReplicates(true);
}

void ACerberusPickup::OnRep_Item()
{
	if (Item)
	{
		PickupMesh->SetStaticMesh(Item->StaticMesh);
		InteractionComponent->NameText = Item->ItemDisplayName;

		// Clients bind to this delegate in order to refresh the interaction widget if item quantity changes
		Item->OnItemModified.AddDynamic(this, &ACerberusPickup::OnItemModified);
	}

	//If any replicated properties on the item have changed, we refresh the widget
	InteractionComponent->RefreshWidget();	
}

void ACerberusPickup::OnItemModified()
{
	if (InteractionComponent)
	{
		InteractionComponent->RefreshWidget();
	}
}

void ACerberusPickup::InitializePickup(const TSubclassOf<UCerberusItem> ItemClass, const int32 Quantity)
{
	if (HasAuthority() && ItemClass && Quantity > 0)
	{
		Item = NewObject<UCerberusItem>(this, ItemClass);
		Item->SetQuantity(Quantity);

		OnRep_Item();
		Item->MarkDirtyForReplication();
	}
}

// Called when the game starts or when spawned
void ACerberusPickup::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && ItemTemplate && bNetStartup)
	{
		InitializePickup(ItemTemplate->GetClass(), ItemTemplate->GetQuantity());
	}

	if (!bNetStartup)
	{
		AlignWithGround();
	}

	if (Item)
	{
		Item->MarkDirtyForReplication();
	}
}

void ACerberusPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACerberusPickup, Item);
}

bool ACerberusPickup::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	//Check if the array of items needs to replicate
	if (Item && Channel->KeyNeedsToReplicate(Item->GetUniqueID(), Item->RepKey))
	{
		bWroteSomething |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
	}

	return bWroteSomething;
}
#if WITH_EDITOR
void ACerberusPickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	//If a new pickup is selected in the property editor, change the mesh to reflect the new item being selected
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ACerberusPickup, ItemTemplate))
	{
		if (ItemTemplate)
		{
			PickupMesh->SetStaticMesh(ItemTemplate->StaticMesh);
		}
	}
}
#endif

void ACerberusPickup::OnTakePickup(ACerberusCharacter* Taker)
{
	if (!Taker)
	{
		UE_LOG(LogCerberus, Warning, TEXT("Pickup was taken but player was not valid."))
		return;
	}
	
	if (HasAuthority() && !IsPendingKillPending() && Item)
	{
		if (UCerberusInventoryComponent* InventoryComponent = Taker->InventoryComponent)
		{
			const FItemAddResult AddResult = InventoryComponent->TryAddItem(Item);

			if (AddResult.ActualAmountGiven < Item->GetQuantity())
			{
				Item->SubtractQuantity(AddResult.ActualAmountGiven);
			} else if (AddResult.ActualAmountGiven >= Item->GetQuantity())
			{
				Destroy();
			}

			if (!AddResult.ErrorText.IsEmpty())
			{
				if (ACerberusCharacter* PC = Cast<ACerberusCharacter>(Taker->GetController()))
				{
					//PC->ClientShowNotification(AddResult.ErrorText);
				}
			}
		}
	}
}


