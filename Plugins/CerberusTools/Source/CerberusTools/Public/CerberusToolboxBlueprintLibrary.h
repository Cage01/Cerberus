// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CerberusToolboxBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CERBERUSTOOLS_API UCerberusToolboxBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Virtual Bones
	UFUNCTION(BlueprintCallable, Category="Toolbox")
	static bool DumpVirtualBones(USkeleton* Skeleton);

	UFUNCTION(BlueprintCallable, Category="Toolbox")
	static bool MergeVirtualBones(USkeleton* SourceSkeleton, USkeleton* TargetSkeleton);

	UFUNCTION(BlueprintCallable, Category="Toolbox")
	static bool AddVirtualBone(const FName& VirtualBoneName, const FName& SourceBoneName, const FName& TargetBoneName, USkeleton* Skeleton);

	// Sockets
	UFUNCTION(BlueprintCallable, Category="Toolbox")
	static bool DumpSockets(USkeleton* Skeleton);

	UFUNCTION(BlueprintCallable, Category="Toolbox")
	static bool MergeSockets(USkeleton* SourceSkeleton, USkeleton* TargetSkeleton);
	
	UFUNCTION(BlueprintCallable, Category="Toolbox")
	static bool HasSocket(const FName& SocketName, USkeleton* Skeleton);

	UFUNCTION(BlueprintCallable, Category="Toolbox")
	static bool AddSockets(const FName& BoneName, const FName& SocketName, const FTransform& RelativeTransform, USkeleton* Skeleton);

	// Skeleton Curves
	UFUNCTION(BlueprintCallable, Category="Toolbox")
	static bool DumpSkeletonCurveNames(USkeleton* Skeleton);

	UFUNCTION(BlueprintCallable, Category="Toolbox")
	static bool CheckForMissingCurveNames(const TArray<FName>& CurveNamesToCheck, USkeleton* Skeleton);
};
