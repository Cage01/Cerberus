// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CerberusEquipmentComponent.generated.h"


class ACerberusPreviewActor;
class UCerberusGearItem;
UENUM(BlueprintType)
enum class EEquipableSlot: uint8
{
	//Character Specific
	EIS_HEAD UMETA(DisplayName = "Head"),
	
	//Gear specific
	GIS_HELMET UMETA(DisplayName = "Helmet"),
	GIS_CHEST UMETA(DisplayName = "Chest"),
	GIS_LEGS UMETA(DisplayName = "Legs"),
	GIS_BACK UMETA(DisplayName = "Back"),
	GIS_PRIMARY_WEAPON UMETA(DisplayName = "Primary Weapon"),
	GIS_SECONDARY_WEAPON UMETA(DisplayName = "Secondary Weapon"),
	GIS_THROWABLE UMETA(DisplayName = "Throwable"),
	
	//Weapon specific
	WIS_BARREL UMETA(DisplayName = "Weapon Barrel"),
	WIS_MAGAZINE UMETA(DisplayName = "Weapon Magazine")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquippedItemsChanged, const EEquipableSlot, Slot, const UCerberusEquipableItem*, Item);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CERBERUS_API UCerberusEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCerberusEquipmentComponent();

	/** Returns the Inventory component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Cerberus|Equipment")
	static UCerberusEquipmentComponent* FindEquipmentComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCerberusEquipmentComponent>() : nullptr); }

	//TODO: Im thinking the replication is necessary for inspecting other players equipment. But be ready to remove if not
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

	UPROPERTY(BlueprintAssignable, Category = "Cerberus|Equipment")
	FOnEquippedItemsChanged OnEquippedItemsChanged;

	/** The mesh to have displayed if we dont have an item equipped to a slot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cerberus|Mesh")
	TMap<EEquipableSlot, USkeletalMesh*> DefaultMeshes;

	/** A Map of the equipped item objects within this component. (Key = Equipment Slot) (Value = UCerberusEquipableItem) */
	UPROPERTY(BlueprintReadOnly, Category="Cerberus|Equipment")
	TMap<EEquipableSlot, UCerberusEquipableItem*> EquippedItems;
	
	/** Defines the Gear that will be shown on the actor in each slot */
	UPROPERTY(BlueprintReadOnly, Category="Cerberus|Mesh")
	TMap<EEquipableSlot, USkeletalMeshComponent*> EquippedMeshes;
	
	/** Finds and returns the SkeletalMeshComp that is being held in the specified equipment slot*/
	UFUNCTION(BlueprintPure)
	USkeletalMeshComponent* FindSlotSkeletalMeshComponent(EEquipableSlot Slot);

	/** This must be defined by the actor that owns this component - Current reasoning is that Actors will have to be generated for things like weapons.
	 * So when that Actor is spawned it will a default value for the preview actor to associate with it. Which is easier to modify for a designer.
	 * That value will then be passed to this component.
	 */
	UPROPERTY(BlueprintReadOnly, Category="Cerberus|Preview")
	ACerberusPreviewActor* PreviewActor;

	bool EquipItem(UCerberusEquipableItem* Item);
	bool UnEquipItem(UCerberusEquipableItem* Item);

	void UpdateSkeletalMesh(UCerberusGearItem* Item);
	void ResetSkeletalMesh(EEquipableSlot Slot);

	// Might be used for weapons ?
	// bool UpdatSocketMesh(UCerberusGearItem* Item);
	// bool ResetSocketMesh(UCerberusGearItem* Item);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	int32 ReplicatedItemsKey;
		
};
