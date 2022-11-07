// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusGameData.h"

#include "CerberusAssetManager.h"

UCerberusGameData::UCerberusGameData()
{
	
}

const UCerberusGameData& UCerberusGameData::UCerberusGameData::Get()
{
	return UCerberusAssetManager::Get().GetGameData();
}
