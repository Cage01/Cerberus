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
				"CoreUObject", 
				"Engine", 
				"InputCore", 
				"GameplayAbilities", 
				"GameplayTags", 
				"GameplayTasks",
				"AIModule",
				"ModularGameplay",
				"ModularGameplayActors"
			});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{

			});
	}
}
