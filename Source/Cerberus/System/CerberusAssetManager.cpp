// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusAssetManager.h"

#include "Cerberus/CerberusLogChannels.h"

const FName FCerberusBundles::Equipped("Equipped");

UCerberusAssetManager::UCerberusAssetManager()
{
}

UCerberusAssetManager& UCerberusAssetManager::Get()
{
	check(GEngine);

	if (UCerberusAssetManager* Singleton = Cast<UCerberusAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogCerberus, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to CerberusAssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<UCerberusAssetManager>();
}

UObject* UCerberusAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		// Use LoadObject if asset manager isn't ready yet.
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void UCerberusAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
}

bool UCerberusAssetManager::ShouldLogAssetLoads()
{
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}
