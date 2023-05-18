// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Defines all equipable slots in the game. Even for weapons and other gear items
 */
UENUM(BlueprintType)
enum class EEquipableSlot: uint8
{
	//Root mesh slot
	ROOT UMETA(DisplayName = "Root"),
	
	//Gear specific
	GIS_HELMET UMETA(DisplayName = "Helmet"),
	GIS_CHEST UMETA(DisplayName = "Chest"),
	GIS_SHOULDERS UMETA(DisplayName = "Shoulders"),
	GIS_HANDS UMETA(DisplayName = "Hands"),
	GIS_FEET UMETA(DisplayName = "Feet"),
	GIS_LEGS UMETA(DisplayName = "Legs"),
	GIS_BACK UMETA(DisplayName = "Back"),
	GIS_PRIMARY_WEAPON UMETA(DisplayName = "Primary Weapon"),
	GIS_SECONDARY_WEAPON UMETA(DisplayName = "Secondary Weapon"),
	GIS_THROWABLE UMETA(DisplayName = "Throwable"),
	
	//Weapon specific
	WIS_BARREL UMETA(DisplayName = "Weapon Barrel"),
	WIS_MAGAZINE UMETA(DisplayName = "Weapon Magazine"),

	
	//End - Helps to iterate through the slots
	NUM UMETA(Hidden),

	//Helpers (Do not include in the iteration count)
	NONE UMETA(Hidden),
	ALL UMETA(Hidden)
	
};
