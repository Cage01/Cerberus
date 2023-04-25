// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusPreviewActor.h"

#include "Cerberus/Items/CerberusGearItem.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACerberusPreviewActor::ACerberusPreviewActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MasterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Master"));
	const FTransform MeshTransform = FTransform(
		FRotator(0.f, -90.f, 0.f),
		FVector(0.f, 0.f, -100.f),
		FVector(1.f));
	MasterMesh->SetRelativeTransform(MeshTransform);
	MasterMesh->SetupAttachment(RootComponent);


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(MasterMesh, NAME_None);
	const FTransform ArmTransform = FTransform(
	FRotator(0.f, -90.f, 0.f),
	FVector(0.f, 0.f, 80.f),
	FVector(1.f));
	SpringArm->SetRelativeTransform(ArmTransform);

	RenderTarget = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("RenderTarget"));
	RenderTarget->SetupAttachment(SpringArm, NAME_None);
}

void ACerberusPreviewActor::UpdateSubMeshMap(EEquipableSlot Slot, USkeletalMeshComponent* Mesh)
{
	if (Mesh)
	{
		SubMeshes.Add(Slot, Mesh);
		OnSubMeshChanged.Broadcast(Slot, Mesh);
	}
}

void ACerberusPreviewActor::SetSubMeshMap(TMap<EEquipableSlot, USkeletalMeshComponent*> Meshes)
{
	SubMeshes.Empty();
	SubMeshes = Meshes;
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

void ACerberusPreviewActor::DrawPreview_Internal()
{

}

