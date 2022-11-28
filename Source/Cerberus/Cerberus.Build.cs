// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Cerberus : ModuleRules
{
	public Cerberus(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", 
				"CoreOnline",
				"CoreUObject", 
				"Engine", 
				"InputCore",
				"GameplayAbilities", 
				"GameplayTags", 
				"GameplayTasks",
				"AIModule",
				"ModularGameplay",
				"ModularGameplayActors",
				"GameFeatures",
				"UMG"
			});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
				"NetCore"
			});
	}
}
