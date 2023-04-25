// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cerberus/UniversalComponents/CerberusEquipmentComponent.h"
#include "CerberusPreviewActor.generated.h"

/*
 * ACerberusPreviewActor
 *
 * An actor that can be spawned that will output its visual components to a render target.
 * This can be used for any type of UI preview, such as characters weapons and etc.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllSubMeshesUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSubMeshChanged, EEquipableSlot, Slot, USkeletalMeshComponent*, Mesh);

class USpringArmComponent;
UCLASS(Abstract, Blueprintable)
class CERBERUS_API ACerberusPreviewActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACerberusPreviewActor();
	
	// /** Takes in a map of all Equipped items to update the skeletal mesh in one pass. Will also modify the local EquippedItems Map*/
	// UFUNCTION(BlueprintCallable, Category="Cerberus|Preview")
	// void DrawPreview(UCerberusGearItem* Item);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cerberus|Preview")
	USkeletalMeshComponent* MasterMesh;
	
	UFUNCTION(BlueprintCallable, Category="Cerberus|Preview")
	void UpdateSubMeshMap(EEquipableSlot Slot, USkeletalMeshComponent* Mesh);

	UFUNCTION(BlueprintCallable, Category="Cerberus|Preview")
	void SetSubMeshMap(TMap<EEquipableSlot, USkeletalMeshComponent*> Meshes);

	
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FOnAllSubMeshesUpdated OnAllSubMeshesUpdated;

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FOnSubMeshChanged OnSubMeshChanged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cerberus|Preview")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cerberus|Preview")
	USceneCaptureComponent2D* RenderTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cerberus|Preview")
	TMap<EEquipableSlot, USkeletalMeshComponent*> SubMeshes;
	

private:
	/** Private function to draw all the skeletal meshes */
	void DrawPreview_Internal();
	
};
