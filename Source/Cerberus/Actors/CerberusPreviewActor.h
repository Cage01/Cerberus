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

	/** The mesh that will be used for the Master/Leader Pose Component. Animations will reference this mesh when moving the other components. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cerberus|Preview")
	USkeletalMeshComponent* MasterMesh;

	/** Will update a single slot in the mesh map. If you have a reference to a USkeletalMeshComponent, call the function ->GetSkeletalMeshAsset() to then pass into this */
	UFUNCTION(BlueprintCallable, Category="Cerberus|Preview")
	void UpdateSubMeshMap(EEquipableSlot Slot, USkeletalMesh* NewMesh, FTransform MeshTransform);

	/** Will clear out the original submesh map and reset it with the values given */
	UFUNCTION(BlueprintCallable, Category="Cerberus|Preview")
	void SetSubMeshMap(TMap<EEquipableSlot, USkeletalMeshComponent*> Meshes);
	
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FOnAllSubMeshesUpdated OnAllSubMeshesUpdated;

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FOnSubMeshChanged OnSubMeshChanged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cerberus|Preview")
	FRotator DefaultRotation;

	UFUNCTION(BlueprintCallable, Category="Cerberus|Preview")
	void ResetPreviewRotation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cerberus|Preview")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cerberus|Preview")
	USceneCaptureComponent2D* RenderTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cerberus|Preview")
	TMap<EEquipableSlot, USkeletalMeshComponent*> SubMeshes;

private:
	/** Private function to find the correct Mesh via name. The names in this case are generated automatically based on the slot they fit into */
	USkeletalMeshComponent* FindMeshComponentByName(FName ComponentName);
	
	/** Private function to draw all the skeletal meshes */
	void DrawPreview_Internal(EEquipableSlot UpdatedSlot);
};
