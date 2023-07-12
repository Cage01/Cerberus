// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusPoseableMeshComponent.h"

void UCerberusPoseableMeshComponent::SetBoneLocalTransformByName(const FName& BoneName, const FTransform& InTransform)
{
	if (!GetSkinnedAsset() || !RequiredBones.IsValid())
	{
		return;
	}

	int32 BoneIndex = GetBoneIndex(BoneName);
	if (BoneIndex >= 0 && BoneIndex < BoneSpaceTransforms.Num())
	{
		BoneSpaceTransforms[BoneIndex] = InTransform;
		MarkRefreshTransformDirty();
	}
}

void UCerberusPoseableMeshComponent::UpdatePose()
{
	if (!GetSkinnedAsset())
	{
		return;
	}

	if (GetNumComponentSpaceTransforms() == 0)
	{
		return;
	}

	FillComponentSpaceTransforms();
	FinalizeBoneTransform();

	UpdateChildTransforms();
	UpdateBounds();
	MarkRenderTransformDirty();
	MarkRenderDynamicDataDirty();
}
