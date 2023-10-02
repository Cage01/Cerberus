// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusLootableActor.h"

#include "Cerberus/Character/CerberusCharacter.h"
#include "Cerberus/Structs/CerberusLootTableRow.h"
#include "Cerberus/UniversalComponents/CerberusInteractionComponent.h"
#include "Cerberus/UniversalComponents/CerberusInventoryComponent.h"

#define LOCTEXT_NAMESPACE "LootActor"
// Sets default values
ACerberusLootableActor::ACerberusLootableActor()
{
	LootContainerMesh = CreateDefaultSubobject<UStaticMeshComponent>("LootContainerMesh");
	SetRootComponent(LootContainerMesh);

	LootInteraction = CreateDefaultSubobject<UCerberusInteractionComponent>("LootInteraction");
	LootInteraction->SetInteractableActionText(LOCTEXT("LootActorText", "Loot"));
	LootInteraction->SetInteractableNameText(LOCTEXT("LootActorName", "Container"));
	LootInteraction->SetupAttachment(GetRootComponent());

	Inventory = CreateDefaultSubobject<UCerberusInventoryComponent>("Inventory");
	Inventory->SetCapacity(20);

	LootRolls = FIntPoint(2, 8);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ACerberusLootableActor::BeginPlay()
{
	Super::BeginPlay();

	//Will call ACerberusLootableActor::OnInteract when the InteractionComponent fires the OnInteract event
	LootInteraction->OnInteract.AddDynamic(this, &ACerberusLootableActor::OnInteract);

	//Server will determine what items are placed within this actor
	if (HasAuthority() && LootTable)
	{
		TArray<FLootTableRow*> SpawnItems;
		LootTable->GetAllRows("", SpawnItems);

		int32 Rolls = FMath::RandRange(LootRolls.GetMin(), LootRolls.GetMax());

		for (int32 i = 0; i < Rolls; i++)
		{
			const FLootTableRow* LootRow = SpawnItems[FMath::RandRange(0, SpawnItems.Num()-1)];
			ensure(LootRow);

			//TODO: Check if this is right. It seems like a larger number would mean lower chances currently
			float ProbablilityRoll = FMath::FRandRange(0.f, 1.f);
			if (ProbablilityRoll > LootRow->Probability)
			{
				if (LootRow && LootRow->Items.Num())
				{
					for (auto& ItemClass : LootRow->Items)
					{
						//TODO: PostEditPropertyChange to not allow this value to go below 1 in the editor
						// Will set a random quantity based on the value given
						const int32 Quantity = FMath::RandRange(1, LootRow->MaxQuantity);
						Inventory->TryAddItemFromClass(ItemClass, Quantity);
					}
				}
			}

			// while (ProbablilityRoll > LootRow->Probability)
			// {
			// 	LootRow = SpawnItems[FMath::RandRange(0, SpawnItems.Num()-1)];
			// 	ProbablilityRoll = FMath::FRandRange(0.f, 1.f);
			// }


		}
	}
}

void ACerberusLootableActor::OnInteract(ACerberusCharacter* Character)
{
	if (Character)
	{
		Character->SetLootSource(Inventory);

		UCerberusInventoryComponent* CharacterInventory = Character->InventoryComponent;
		//Attempt to take items from the inventory
		for (const auto& Item : Inventory->GetItems())
		{
			//Add the item to the looting characters inventory, and remove the same amount looted from this inventory
			FItemAddResult Result = CharacterInventory->TryAddItem(Item);
			if (Result.ActualAmountGiven > 0)
			{
				Inventory->ConsumeItem(Item, Result.ActualAmountGiven);
			}
		}

		ensure(Inventory->GetItems().Num() >= 0);
		//Checks number of items left in this inventory. If there are none, disable interactivity
		if (Inventory->GetItems().Num() == 0)
		{
			//The interaction check on the owning character will see this value and decide to hide the UI.
			//When trying to manually hide the UI here by ending focus, there was no way to get the player client to perform the action from this actor.
			LootInteraction->SetIsInteractable(false);
			
		}
	}
	
}

#undef LOCTEXT_NAMESPACE