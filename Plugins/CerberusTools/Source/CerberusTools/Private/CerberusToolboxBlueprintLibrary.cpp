// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusToolboxBlueprintLibrary.h"

#include "Engine/SkeletalMeshSocket.h"
#if WITH_EDITOR
#include "HAL/PlatformApplicationMisc.h"
#endif


// Function prototypes
static FString FVectorToString(const FVector& Vector);

bool UCerberusToolboxBlueprintLibrary::DumpVirtualBones(USkeleton* Skeleton)
{
	// Check input argument
	if (!IsValid(Skeleton))
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"DumpVirtualBones\" with invalid skeleton."));
		return false;
	}
	
	if (Skeleton->GetVirtualBones().Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("\"%s\" does not contain any virtual bones."), *(Skeleton->GetFullName()));
		return false;
	}

	// Prepare string for virtual bones
	FString dumpString;
	if (Skeleton->GetVirtualBones().Num() > 1)
	{
		dumpString += "(";
	}

	uint32 count = 0;
	for (auto& VirtualBone : Skeleton->GetVirtualBones())
	{
		if (count > 0)
		{
			dumpString += ",";
		}

		dumpString += "(";

		dumpString += "VirtualBoneName=\"";
		dumpString += VirtualBone.VirtualBoneName.ToString();
		dumpString += "\",";

		dumpString += "SourceBoneName=\"";
		dumpString += VirtualBone.SourceBoneName.ToString();
		dumpString += "\",";

		dumpString += "TargetBoneName=\"";
		dumpString += VirtualBone.TargetBoneName.ToString();
		dumpString += "\",";

		dumpString += ")";

		count++;
	}

	if (Skeleton->GetVirtualBones().Num() > 1)
	{
		dumpString += ")";
	}
	// Dump virtual bones
	UE_LOG(LogTemp, Log, TEXT("%s"), *dumpString);

	// Copy bones to clipboard
#if WITH_EDITOR
	FPlatformApplicationMisc::ClipboardCopy(*dumpString);
#endif
	
	return true;
}

bool UCerberusToolboxBlueprintLibrary::MergeVirtualBones(USkeleton* SourceSkeleton, USkeleton* TargetSkeleton)
{
	// Check input argument
	if (!IsValid(SourceSkeleton))
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"MergeVirtualBones\" with invalid source skeleton."));
		return false;
	}

	// Check input argument
	if (!IsValid(TargetSkeleton))
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"MergeVirtualBones\" with invalid target skeleton."));
		return false;
	}
	
	if (SourceSkeleton->GetVirtualBones().Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("\"%s\" does not contain any virtual bones."), *(SourceSkeleton->GetFullName()));
		return false;
	}

	for (auto& VirtualBone : SourceSkeleton->GetVirtualBones())
	{
		AddVirtualBone(VirtualBone.VirtualBoneName, VirtualBone.SourceBoneName, VirtualBone.TargetBoneName, TargetSkeleton);
	}

	return true;

}


bool UCerberusToolboxBlueprintLibrary::AddVirtualBone(const FName& VirtualBoneName, const FName& SourceBoneName, const FName& TargetBoneName, USkeleton* Skeleton)
{
	
	
	// Check input argument
	if (!IsValid(Skeleton))
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"AddVirtualBone\" with invalid skeleton."));
		return false;
	}

	bool IsValidVirtualBone = true;
	IsValidVirtualBone &= VirtualBoneName != NAME_None;
	if (VirtualBoneName == NAME_None)
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"AddVirtualBone\" with invalid VirtualBoneName."));
	}

	IsValidVirtualBone &= SourceBoneName != NAME_None;
	if (SourceBoneName == NAME_None)
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"AddVirtualBone\" with invalid SourceBoneName."));
	}

	IsValidVirtualBone &= TargetBoneName != NAME_None;
	if (TargetBoneName == NAME_None)
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"TargetBoneName\" with invalid VirtualBoneName."));
	}

	if (!IsValidVirtualBone)
	{
		return false;
	}
	

	for (auto& VirtualBone : Skeleton->GetVirtualBones())
	{
		if (VirtualBone.VirtualBoneName == VirtualBoneName &&
			VirtualBone.SourceBoneName == SourceBoneName &&
			VirtualBone.TargetBoneName == TargetBoneName)
		{
			UE_LOG(LogTemp, Error, TEXT("virtual bone: %s, source = %s, target %s already exists in the skeleton %s"), *VirtualBoneName.ToString(), *SourceBoneName.ToString(), *TargetBoneName.ToString(), *(Skeleton->GetFullName()));
			return false;
		} 
	}
	
	FName NewVirtualBoneName = VirtualBoneName;
	if (!Skeleton->AddNewVirtualBone(SourceBoneName, TargetBoneName, NewVirtualBoneName))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to add virtual bone to %s"), *(Skeleton->GetFullName()));
		return false;
	}

	Skeleton->RenameVirtualBone(NewVirtualBoneName, VirtualBoneName);
	Skeleton->Modify();

	return true;
}

bool UCerberusToolboxBlueprintLibrary::DumpSockets(USkeleton* Skeleton)
{
	// Check input argument
	if (!IsValid(Skeleton))
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"DumpVirtualBones\" with invalid skeleton."));
		return false;
	}
	
	if (Skeleton->Sockets.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("\"%s\" does not contain any sockets."), *(Skeleton->GetFullName()));
		return false;
	}

	// Prepare string for sockets
	FString dumpString;
	if (Skeleton->Sockets.Num() > 1)
	{
		dumpString += "(";
	}

	uint32 count = 0;
	for (auto& Socket : Skeleton->Sockets)
	{
		if (count > 0)
		{
			dumpString += ",";
		}

		dumpString += "(";

		dumpString += "BoneName=\"";
		dumpString += Socket->BoneName.ToString();
		dumpString += "\",";

		dumpString += "SocketName=\"";
		dumpString += Socket->SocketName.ToString();
		dumpString += "\",";

		dumpString += "RelativeTransform=(Rotation=(";
		
		const auto Rotation = Socket->RelativeRotation.Quaternion();
		FString RotationString = "X=";
		RotationString += FString::SanitizeFloat(Rotation.X);
		RotationString += ",Y=";
		RotationString += FString::SanitizeFloat(Rotation.Y);
		RotationString += ",Z=";
		RotationString += FString::SanitizeFloat(Rotation.Z);
		RotationString += ",W=";
		RotationString += FString::SanitizeFloat(Rotation.W);
		dumpString += RotationString;

		dumpString += "),Translation=(";
		dumpString += FVectorToString(Socket->RelativeLocation);

		dumpString += "),Scale3D=(";
		dumpString += FVectorToString(Socket->RelativeScale);
		dumpString += ")))";

		count++;
	}

	if (Skeleton->Sockets.Num() > 1)
	{
		dumpString += ")";
	}
	// Dump sockets
	UE_LOG(LogTemp, Log, TEXT("%s"), *dumpString);

	// Copy sockets to clipboard
#if WITH_EDITOR
	FPlatformApplicationMisc::ClipboardCopy(*dumpString);
#endif
	
	return true;
}

bool UCerberusToolboxBlueprintLibrary::MergeSockets(USkeleton* SourceSkeleton, USkeleton* TargetSkeleton)
{
	// Check input argument
	if (!IsValid(SourceSkeleton))
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"MergeSockets\" with invalid source skeleton."));
		return false;
	}

	// Check input argument
	if (!IsValid(TargetSkeleton))
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"MergeSockets\" with invalid target skeleton."));
		return false;
	}
	
	if (SourceSkeleton->GetVirtualBones().Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("\"%s\" does not contain any sockets."), *(SourceSkeleton->GetFullName()));
		return false;
	}

	for (auto& Socket : SourceSkeleton->Sockets)
	{
		const FTransform SocketTransform = FTransform(
			Socket->RelativeRotation,
			Socket->RelativeLocation,
			Socket->RelativeScale);
		AddSockets(Socket->BoneName, Socket->SocketName, SocketTransform, TargetSkeleton);
	}

	return true;
	return true;
}

bool UCerberusToolboxBlueprintLibrary::HasSocket(const FName& SocketName, USkeleton* Skeleton)
{
	if (!IsValid(Skeleton))
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"HasSocket\" with invalid skeleton."));
		return true;
	}

	if (SocketName == NAME_None)
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"HasSocket\" with invalid socket name."));
		return true;
	}

	for (auto Socket : Skeleton->Sockets)
	{
		if (IsValid(Socket))
		{
			if (Socket->SocketName == SocketName)
			{
				return true;
			}
		}
	}
	
	return false;
}

bool UCerberusToolboxBlueprintLibrary::AddSockets(const FName& BoneName, const FName& SocketName,
                                                  const FTransform& RelativeTransform, USkeleton* Skeleton)
{
	
	// Check input argument
	if (!IsValid(Skeleton))
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"AddSockets\" with invalid skeleton."));
		return false;
	}

	bool IsValidSocket = true;
	IsValidSocket &= BoneName != NAME_None;
	if (BoneName == NAME_None)
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"AddSockets\" with invalid BoneName."));
	}

	IsValidSocket &= SocketName != NAME_None;
	if (SocketName == NAME_None)
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"AddSockets\" with invalid SocketName."));
	}

	if (!IsValidSocket)
	{
		return false;
	}

	if (!HasSocket(SocketName, Skeleton))
	{
		auto Socket = NewObject<USkeletalMeshSocket>(Skeleton);
		Socket->BoneName = BoneName;
		Socket->SocketName = SocketName;
		Socket->RelativeLocation = RelativeTransform.GetLocation();
		Socket->RelativeRotation = RelativeTransform.GetRotation().Rotator();
		Socket->RelativeScale = RelativeTransform.GetScale3D();
		Skeleton->Sockets.Add(Socket);
		
		Skeleton->Modify();
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"AddSockets\" HasSocket has returned true"));
		return false;
	}

	return true;
}

bool UCerberusToolboxBlueprintLibrary::DumpSkeletonCurveNames(USkeleton* Skeleton)
{
	// Check input argument
	if (!IsValid(Skeleton))
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"DumpSkeletonCurveNames\" with invalid skeleton."));
		return false;
	}

	auto CurveMapping = Skeleton->GetSmartNameContainer(USkeleton::AnimCurveMappingName);
	if (!CurveMapping)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get curve mapping in \"DumpSkeletonCurveNames\". Contact author of the plugin for more information."));
		return false;
	}

	TArray<FName> CurveNames;
	CurveMapping->Iterate([&CurveNames](const FSmartNameMappingIterator& Iterator)
		{
			FName CurveName;
			if (Iterator.GetName(CurveName))
			{
				CurveNames.Add(CurveName);
			} else
			{
				UE_LOG(LogTemp, Error, TEXT("An error occured in \"DumpSkeletonCurveNames\" while getting a curve name. Contact author of the plugin for more information."));
			}
		}
	);

	FString dumpString = "(";
	uint32 count = 0;
	for (auto CurveName : CurveNames)
	{
		if (count > 0)
		{
			dumpString += ",";
		}

		dumpString += "\"";
		dumpString += CurveName.ToString();
		dumpString += "\"";

		count++;
	}

	dumpString += ")";

	UE_LOG(LogTemp, Log, TEXT("%s"), *dumpString);
	
#if WITH_EDITOR
	FPlatformApplicationMisc::ClipboardCopy(*dumpString);
#endif
	return true;
}

bool UCerberusToolboxBlueprintLibrary::CheckForMissingCurveNames(const TArray<FName>& CurveNamesToCheck, USkeleton* Skeleton)
{
	// Check input argument
	if (!IsValid(Skeleton))
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"CheckForMissingCurveNames\" with invalid skeleton."));
		return false;
	}

	if (CurveNamesToCheck.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Called \"CheckForMissingCurveNames\" with no curve names to check"));
		return false;
	}

	auto CurveMapping = Skeleton->GetSmartNameContainer(USkeleton::AnimCurveMappingName);
	if (!CurveMapping)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get curve mapping in \"CheckForMissingCurveNames\". Contact author of the plugin for more information."));
		return false;
	}

	TArray<FName> SkeletonCurveNames;
	CurveMapping->Iterate([&SkeletonCurveNames](const FSmartNameMappingIterator& Iterator)
		{
			FName CurveName;
			if (Iterator.GetName(CurveName))
			{
				SkeletonCurveNames.Add(CurveName);
			} else
			{
				UE_LOG(LogTemp, Error, TEXT("An error occured in \"CheckForMissingCurveNames\" while getting a curve name. Contact author of the plugin for more information."));
			}
		}
	);

	bool HasNoMissingCurveNames = true;
	for (auto& CurveName : CurveNamesToCheck)
	{
		if (SkeletonCurveNames.Find(CurveName) == INDEX_NONE)
		{
			if (HasNoMissingCurveNames)
			{
				UE_LOG(LogTemp, Error, TEXT("The following curves are missing in the skeleton \"%s\": "), *(Skeleton->GetFullName()));
			}

			UE_LOG(LogTemp, Error, TEXT("  %s"), *CurveName.ToString());
			HasNoMissingCurveNames = false;
		}
	}
	return true;
}


FString FVectorToString(const FVector& Vector)
{
	FString str = "X=";
	str += FString::SanitizeFloat(Vector.X);
	str += ",Y=";
	str += FString::SanitizeFloat(Vector.Y);
	str += ",Z=";
	str += FString::SanitizeFloat(Vector.Z);

	return str;
}
