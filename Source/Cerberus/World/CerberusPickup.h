// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CerberusPickup.generated.h"

class UCerberusInteractionComponent;
class ACerberusCharacter;
class UCerberusItem;
UCLASS(ClassGroup = (Items), Blueprintable, Abstract)
class CERBERUS_API ACerberusPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACerberusPickup();

	void InitializePickup(const TSubclassOf<UCerberusItem> ItemClass, const int32 Quantity);

	/**If the item gets dropped, it will fire this event, which will help match the alignment of the object to the terrain in the world*/
	UFUNCTION(BlueprintImplementableEvent)
	void AlignWithGround();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UCerberusItem* ItemTemplate;

	UFUNCTION()
	void OnRep_Item();

protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, ReplicatedUsing=OnRep_Item)
	UCerberusItem* Item;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* PickupMesh;
	
	/**If some property on the item has modified (ie ammo on the ground was picked up, but the player only grabbed 20 out of its available 40. We bind this event to OnItemModified and refresh the UI.*/
	UFUNCTION()
	void OnItemModified();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION()
	void OnTakePickup(ACerberusCharacter* Taker);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCerberusInteractionComponent* InteractionComponent;
};
