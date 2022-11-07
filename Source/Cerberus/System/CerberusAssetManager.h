// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CerberusGameData.h"
#include "Cerberus/Character/CerberusPawnData.h"
#include "Engine/AssetManager.h"
#include "CerberusAssetManager.generated.h"

struct FCerberusBundles
{
	static const FName Equipped;
};

/**
 * UCerberusAssetManager
 *
 *	Game implementation of the asset manager that overrides functionality and stores game-specific types.
 *	It is expected that most games will want to override AssetManager as it provides a good place for game-specific loading logic.
 *	This class is used by setting 'AssetManagerClassName' in DefaultEngine.ini.
 */
UCLASS(Config = Game)
class CERBERUS_API UCerberusAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UCerberusAssetManager();

	/** Returns the AssetManager singleton object. */
	static UCerberusAssetManager& Get();

	void AddLoadedAsset(const UObject* Asset);
	

	/** Returns the subclass referenced by a TSoftClassPtr.  This will synchronously load the asset if it's not already loaded. */
	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	// Logs all assets currently loaded and tracked by the asset manager.
	static void DumpLoadedAssets();

	const UCerberusGameData& GetGameData();
	const UCerberusPawnData* GetDefaultPawnData() const;


protected:
	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);
	static bool ShouldLogAssetLoads();
	
	// Global game data asset to use.
	UPROPERTY(Config)
	TSoftObjectPtr<UCerberusGameData> CerberusGameDataPath;

	// Pawn data used when spawning player pawns if there isn't one set on the player state.
	UPROPERTY(Config)
	TSoftObjectPtr<UCerberusPawnData> DefaultPawnData;

	// Loaded version of the game data
	UPROPERTY(Transient)
	TMap<UClass*, UPrimaryDataAsset*> GameDataMap;
	
	// 	//~UAssetManager interface
	// 	virtual void StartInitialLoading() override;
	// #if WITH_EDITOR
	// 	virtual void PreBeginPIE(bool bStartSimulate) override;
	// #endif
	// 	//~End of UAssetManager interface

	UPrimaryDataAsset* LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath, FPrimaryAssetType PrimaryAssetType);

	
	template <typename GameDataClass>
	const GameDataClass& GetOrLoadTypedGameData(const TSoftObjectPtr<GameDataClass>& DataPath)
		{
			if (const UPrimaryDataAsset* const * pResult = GameDataMap.Find(GameDataClass::StaticClass()))
			{
				return *CastChecked<GameDataClass>(*pResult);
			}

			// Does a blocking load if needed
			return *CastChecked<const GameDataClass>(LoadGameDataOfClass(GameDataClass::StaticClass(), DataPath, GameDataClass::StaticClass()->GetFName()));
		}
	
private:
	
	// Assets loaded and tracked by the asset manager.
	UPROPERTY()
	TSet<const UObject*> LoadedAssets;

	// Used for a scope lock when modifying the list of load assets.
	FCriticalSection LoadedAssetsCritical;
};

template <typename AssetType>
AssetType* UCerberusAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template <typename AssetType>
TSubclassOf<AssetType> UCerberusAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}
