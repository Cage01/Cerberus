// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PoseableMeshComponent.h"
#include "CerberusPoseableMeshComponent.generated.h"

/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class CERBERUSTOOLS_API UCerberusPoseableMeshComponent : public UPoseableMeshComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Components|PoseableMesh")
	void SetBoneLocalTransformByName(const FName& BoneName, const FTransform& InTransform);

	UFUNCTION(BlueprintCallable, Category="Components|PoseableMesh")
	void UpdatePose();
	
};
