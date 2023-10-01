// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserModule.h"
#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Modules/ModuleManager.h"
#include "UObject/SavePackage.h"

#define LOCTEXT_NAMESPACE "FCerberusToolsModule"


class FCerberusToolsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	

/** Code to handle exporting any asset thumbnail to a 2d Texture. */
private:
	FDelegateHandle ContentBrowserExtenderDelegateHandle;

	void AddContentBrowserContextMenuExtender()
	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
		TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuAssetExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();

		CBMenuAssetExtenderDelegates.Add(FContentBrowserMenuExtender_SelectedAssets::CreateStatic(&OnExtendContentBrowserAssetSelectionMenu));
		ContentBrowserExtenderDelegateHandle = CBMenuAssetExtenderDelegates.Last().GetHandle();
	}

	void RemoveContentBrowserContextMenuExtender() const
	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
		TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
		CBMenuExtenderDelegates.RemoveAll([this](const FContentBrowserMenuExtender_SelectedAssets& Delegate) { return Delegate.GetHandle() == ContentBrowserExtenderDelegateHandle; });
	}
	
	static TSharedRef<FExtender> OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets)
	{
		TSharedRef<FExtender> Extender = MakeShared<FExtender>();
		Extender->AddMenuExtension(
			"CommonAssetActions",
			EExtensionHook::After,
			nullptr,
			FMenuExtensionDelegate::CreateStatic(&ExecuteSaveThumbnailAsTexture, SelectedAssets)
		);
		return Extender;
	}
	
	static void ExecuteSaveThumbnailAsTexture(FMenuBuilder& MenuBuilder, const TArray<FAssetData> SelectedAssets)
	{
		// A mix of https://forums.unrealengine.com/t/copy-asset-thumbnail-to-new-texture2d/138054/4
		// and https://isaratech.com/save-a-procedurally-generated-texture-as-a-new-asset/
		// and https://arrowinmyknee.com/2020/08/28/asset-right-click-menu-in-ue4/
		MenuBuilder.BeginSection("Cerberus Tools", LOCTEXT("ASSET_CONTEXT", "Cerberus Tools"));
		{
			// Add Menu Entry Here
			MenuBuilder.AddMenuEntry(
			LOCTEXT("ButtonName", "Export Thumbnail to Texture"),
			LOCTEXT("Button ToolTip", "Will export asset's thumbnail and put it in the Content/Development/ProceduralTextures/ folder"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateLambda([SelectedAssets]()
			{
				for(const FAssetData& Asset : SelectedAssets)
				{
					int32 pathSeparatorIdx;
					FAssetData obj = Asset;
					FString GamePath = obj.GetAsset()->GetPathName();
					FString AssetName;
					int32 pathEnd;
					if (GamePath.FindLastChar('/', pathEnd)) {
						++pathEnd;
						AssetName += GamePath;
						AssetName.RightChopInline(pathEnd);
						int32 extensionIdx;
						if (AssetName.FindChar('.', extensionIdx)) {
							AssetName.LeftInline(extensionIdx);
						}
						GamePath.LeftInline(pathEnd);
						FString Suffix = "T_";
						FString NameWithSuffix = Suffix + AssetName;
						AssetName = NameWithSuffix;
					}
					else {
						AssetName = "T_Thumbnail";
					}
					
					if (AssetName.FindChar('/', pathSeparatorIdx)) {
						// TextureName should not have any path separators in it
						return;
					}

					FObjectThumbnail* thumb = ThumbnailTools::GenerateThumbnailForObjectToSaveToDisk(obj.GetAsset());
					if (!thumb) {
						return;
					}

					FString PackageName = TEXT("/Game/Development/ProceduralTextures/");
					if (!PackageName.EndsWith("/")) {
						PackageName += "/";
					}
					PackageName += AssetName;

					UPackage* Package = CreatePackage(*PackageName);
					Package->FullyLoad();

					UTexture2D* NewTexture = NewObject<UTexture2D>(Package, *AssetName, RF_Public | RF_Standalone | RF_MarkAsRootSet);
					NewTexture->AddToRoot();
					FTexturePlatformData* platformData = new FTexturePlatformData();
					platformData->SizeX = thumb->GetImageWidth();
					platformData->SizeY = thumb->GetImageHeight();
					//platformData->NumSlices = 1;
					platformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;
					NewTexture->SetPlatformData(platformData);

					FTexture2DMipMap* Mip = new FTexture2DMipMap();
					platformData->Mips.Add(Mip);
					Mip->SizeX = thumb->GetImageWidth();
					Mip->SizeY = thumb->GetImageHeight();

					Mip->BulkData.Lock(LOCK_READ_WRITE);
					uint8* TextureData = (uint8*)Mip->BulkData.Realloc(thumb->GetUncompressedImageData().Num() * 4);
					FMemory::Memcpy(TextureData, thumb->GetUncompressedImageData().GetData(), thumb->GetUncompressedImageData().Num());
					Mip->BulkData.Unlock();

					NewTexture->Source.Init(thumb->GetImageWidth(), thumb->GetImageHeight(), 1, 1, ETextureSourceFormat::TSF_BGRA8, thumb->GetUncompressedImageData().GetData());
					NewTexture->LODGroup = TEXTUREGROUP_UI;
					NewTexture->UpdateResource();
					Package->MarkPackageDirty();
					Package->FullyLoad();
					FAssetRegistryModule::AssetCreated(NewTexture);

					FSavePackageArgs SaveArgs;
					SaveArgs.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
					SaveArgs.SaveFlags = SAVE_NoError;
					SaveArgs.bForceByteSwapping = true;
					FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
					UPackage::SavePackage(Package, NewTexture, *PackageFileName, SaveArgs);
				}
			})),
	NAME_None,
	EUserInterfaceActionType::Button);
		}
		MenuBuilder.EndSection();
	}


};
#undef LOCTEXT_NAMESPACE


