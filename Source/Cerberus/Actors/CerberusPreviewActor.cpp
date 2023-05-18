// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusPreviewActor.h"

#include "Cerberus/Enums/EquipableSlot.h"
#include "Cerberus/Items/CerberusGearItem.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACerberusPreviewActor::ACerberusPreviewActor()
{
	//Setting defaults
	DefaultRotation = FRotator(0.f, -90.f, 0.f);
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MasterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Master"));
	const FTransform MeshTransform = FTransform(
		DefaultRotation,
		FVector(0.f, 0.f, -100.f),
		FVector(1.f));
	MasterMesh->SetRelativeTransform(MeshTransform);
	MasterMesh->SetupAttachment(RootComponent);


	UEnum* Slots = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEquipableSlot"), true);

	for (int32 EnumIndex = 0; EnumIndex < Slots->NumEnums(); ++EnumIndex)
	{
		EEquipableSlot Slot = static_cast<EEquipableSlot>(Slots->GetValueByIndex(EnumIndex));
		//EEquipableSlot Slot = EnumValue.GetValue();

		// Skip helper values
		if (Slot == EEquipableSlot::ALL || Slot == EEquipableSlot::NONE)
			continue;

		// Create SkeletalMeshComponent and attach it to the Master
		const FName SlotName = FName(*UEnum::GetValueAsString(Slot));
		USkeletalMeshComponent* MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(SlotName);
		AddOwnedComponent(MeshComp);
		MeshComp->SetupAttachment(MasterMesh);
		MeshComp->SetLeaderPoseComponent(MasterMesh);
		SubMeshes.Add(Slot, MeshComp);
		
	}

	// Acts as the camera for the preview actor. Will handle rotation and etc
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(MasterMesh, NAME_None);
	const FTransform ArmTransform = FTransform(
	DefaultRotation,
	FVector(0.f, 0.f, 80.f),
	FVector(1.f));
	SpringArm->SetRelativeTransform(ArmTransform);

	RenderTarget = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("RenderTarget"));
	RenderTarget->SetupAttachment(SpringArm, NAME_None);
}

void ACerberusPreviewActor::UpdateSubMeshMap(EEquipableSlot Slot, USkeletalMesh* NewMesh, FTransform MeshTransform)
{
	if (NewMesh)
	{
		USkeletalMeshComponent* MeshComp = *SubMeshes.Find(Slot);
		MeshComp->SetSkeletalMeshAsset(NewMesh);
		MeshComp->SetRelativeTransform(MeshTransform);
		DrawPreview_Internal(Slot);
		OnSubMeshChanged.Broadcast(Slot, MeshComp);
	}
}

void ACerberusPreviewActor::SetSubMeshMap(TMap<EEquipableSlot, USkeletalMeshComponent*> Meshes)
{
	SubMeshes.Empty();
	SubMeshes = Meshes;
	
	DrawPreview_Internal(EEquipableSlot::ALL);
	OnAllSubMeshesUpdated.Broadcast();
}

//
// void ACerberusPreviewActor::DrawPreview(UCerberusGearItem* Item)
// {
//
// }

// Called when the game starts or when spawned
void ACerberusPreviewActor::BeginPlay()
{
	Super::BeginPlay();


}

void ACerberusPreviewActor::ResetPreviewRotation()
{
	SpringArm->SetRelativeRotation(DefaultRotation);
}

USkeletalMeshComponent* ACerberusPreviewActor::FindMeshComponentByName(FName ComponentName)
{
	if (ComponentName.IsValid())
	{
		TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
		GetComponents(SkeletalMeshComponents);
	
		for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
		{
			if (SkeletalMeshComponent->GetFName() == ComponentName)
			{
				return SkeletalMeshComponent;
			}
		}
	}

	return nullptr;
}

void ACerberusPreviewActor::DrawPreview_Internal(EEquipableSlot UpdatedSlot) 
{
	// If nothing was updated return
	if (UpdatedSlot == EEquipableSlot::NONE)
		return;

	// If all the slots were updated, iterate through and update each
	if (UpdatedSlot == EEquipableSlot::ALL)
	{
		UEnum* Slots = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEquipableSlot"), true);
		for (int32 EnumIndex = 0; EnumIndex < static_cast<int32>(EEquipableSlot::NUM); ++EnumIndex)
		{
			EEquipableSlot Slot = static_cast<EEquipableSlot>(Slots->GetValueByIndex(EnumIndex));

			//Skips the root mesh, shouldn't hopefully need to be updated
			if (Slot == EEquipableSlot::ROOT)
				continue;
			
			const FName SlotName = FName(*UEnum::GetValueAsString(Slot));

			// Update the actual owned component with the new mesh
			if (USkeletalMeshComponent* OwnedComponent = FindMeshComponentByName(SlotName))
			{
				if(SubMeshes.Contains(Slot))
				{
					const USkeletalMeshComponent* MeshComponent = *SubMeshes.Find(Slot);
					OwnedComponent->SetSkeletalMeshAsset(MeshComponent->GetSkeletalMeshAsset());
					OwnedComponent->SetRelativeTransform(MeshComponent->GetRelativeTransform());
				}
			}
		}

		return;
	}

	// Otherwise a specific slot was updated
	const FName SlotName = FName(*UEnum::GetValueAsString(UpdatedSlot));

	// Update the actual owned component with the new mesh
	if (USkeletalMeshComponent* OwnedComponent = FindMeshComponentByName(SlotName))
	{
		const USkeletalMeshComponent* MeshComponent = *SubMeshes.Find(UpdatedSlot);
		OwnedComponent->SetSkeletalMeshAsset(MeshComponent->GetSkeletalMeshAsset());
		OwnedComponent->SetRelativeTransform(MeshComponent->GetRelativeTransform());
	}
	
}

