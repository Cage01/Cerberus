#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;
/**
 * FCerberusGameplayTags
 *
 *	Singleton containing native gameplay tags.
 */
struct FCerberusGameplayTags
{
public:
	static const FCerberusGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeTags();

	static FGameplayTag FindTagByString(FString TagString, bool bMatchPartialString = false);
	
public:

	FGameplayTag Ability_ActivateFail_IsDead;
	FGameplayTag Ability_ActivateFail_Networking;
	FGameplayTag Ability_ActivateFail_ActivationGroup;
	
	FGameplayTag GameplayEvent_Death;
	FGameplayTag GameplayEvent_Reset;
	FGameplayTag GameplayEvent_RequestReset;


	FGameplayTag SetByCaller_Damage;
	FGameplayTag SetByCaller_Heal;

	FGameplayTag Status_Death;
	FGameplayTag Status_Death_Dying;
	FGameplayTag Status_Death_Dead;
	FGameplayTag Status_Pvp;

	

	// Movement tags may become useful because I may move inputs and actions to GAS
	TMap<uint8, FGameplayTag> MovementModeTagMap;
	TMap<uint8, FGameplayTag> CustomMovementModeTagMap;
	
protected:
	void AddAllTags(UGameplayTagsManager* Manager);
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, uint8 MovementMode);
	void AddCustomMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, uint8 CustomMovementMode);


private:
	static FCerberusGameplayTags GameplayTags;
};
