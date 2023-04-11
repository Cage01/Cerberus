// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusNetworkUtils.h"

bool UCerberusNetworkUtils::IsListenServerHost(UWorld* World)
{
	if (World->GetNetMode() == NM_ListenServer)
	{
		const APlayerState* PlayerState = World->GetFirstPlayerController()->PlayerState;
		return PlayerState && !PlayerState->IsABot();
	}
	
	return false;
}
