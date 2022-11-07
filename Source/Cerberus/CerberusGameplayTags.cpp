#include "CerberusGameplayTags.h"

#include "CerberusLogChannels.h"
#include "GameplayTagsManager.h"

FCerberusGameplayTags FCerberusGameplayTags::GameplayTags;

void FCerberusGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	
	GameplayTags.AddAllTags(&Manager);

	// Notify manager that we are done adding native tags.
	Manager.DoneAddingNativeTags();
}

void FCerberusGameplayTags::AddAllTags(UGameplayTagsManager* Manager)
{
	AddTag(Ability_ActivateFail_IsDead, "Ability.ActivateFail.IsDead", "Ability failed to activate because its owner is dead.");
	AddTag(Ability_ActivateFail_Networking, "Ability.ActivateFail.Networking", "Ability failed to activate because it did not pass the network checks.");
	AddTag(Ability_ActivateFail_ActivationGroup, "Ability.ActivateFail.ActivationGroup", "Ability failed to activate because of its activation group.");

	
	AddTag(GameplayEvent_Death, "GameplayEvent.Death", "Event that fires on death. This event only fires on the server.");
	AddTag(GameplayEvent_Reset, "GameplayEvent.Reset", "Event that fires once a player reset is executed");
	AddTag(GameplayEvent_RequestReset, "GameplayEvent.RequestReset", "Event to request a player's pawn to be instantly replaced with a new one at a valid spawn location.");

	AddTag(SetByCaller_Damage, "SetByCaller.Damage", "SetByCaller tag used by damage gameplay events");
	AddTag(SetByCaller_Heal, "SetByCaller.Heal", "SetByCaller tag used by healing gameplay events");
	
	
	AddTag(Status_Death, "Status.Death", "Target has the death status");
	AddTag(Status_Death_Dying, "Status.Death.Dying", "Target has begun the death process");
	AddTag(Status_Death_Dead, "Status.Death.Dead", "Target has finished the death process");
	AddTag(Status_Pvp, "Status.Pvp", "Target is able to attack and be attacked by other players");
}

void FCerberusGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}

void FCerberusGameplayTags::AddMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, uint8 MovementMode)
{
	AddTag(OutTag, TagName,"Character movement mode tag.");
	GameplayTags.MovementModeTagMap.Add(MovementMode, OutTag);
}

void FCerberusGameplayTags::AddCustomMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, uint8 CustomMovementMode)
{
	AddTag(OutTag, TagName,"Character custom movement mode tag.");
	GameplayTags.CustomMovementModeTagMap.Add(CustomMovementMode, OutTag);
}

FGameplayTag FCerberusGameplayTags::FindTagByString(FString TagString, bool bMatchPartialString)
{
	const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

	if (!Tag.IsValid() && bMatchPartialString)
	{
		FGameplayTagContainer AllTags;
		Manager.RequestAllGameplayTags(AllTags, true);

		for (const FGameplayTag TestTag : AllTags)
		{
			if (TestTag.ToString().Contains(TagString))
			{
				UE_LOG(LogCerberus, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
				Tag = TestTag;
				break;
			}
		}
	}

	return Tag;
}
