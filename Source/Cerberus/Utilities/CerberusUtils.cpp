// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusUtils.h"

bool UCerberusUtils::IsListenServerHost(UWorld* World)
{
	if (World->GetNetMode() == NM_ListenServer)
	{
		const APlayerState* PlayerState = World->GetFirstPlayerController()->PlayerState;
		return PlayerState && !PlayerState->IsABot();
	}
	
	return false;
}

TArray<uint8> UCerberusUtils::SerializeObject(UObject* Target)
{
	TArray<uint8> bytes;
	// FMemoryReader memoryWriter(bytes, true);
	// FObjectAndNameAsStringProxyArchive ar(memoryWriter, false);

	FObjectWriter memoryWriter(Target, bytes);
	Target->Serialize(memoryWriter);
	return bytes;
}

UObject* UCerberusUtils::DeserializeObject(AActor* Owner, TSubclassOf<UObject> TargetClass, TArray<uint8> bytes)
{
	UObject* Object = NewObject<UObject>(Owner, TargetClass);
	// FMemoryReader memoryReader(bytes, true);
	// FObjectAndNameAsStringProxyArchive ar(memoryReader, false);

	FObjectReader memoryReader(Object, bytes);
	Object->Serialize(memoryReader);

	return Object;
}
