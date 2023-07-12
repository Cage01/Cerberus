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
				"Serialization",
				"Engine", 
				"InputCore",
				"GameplayAbilities", 
				"GameplayTags", 
				"GameplayTasks",
				"AIModule",
				"UMG"
			});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
				"NetCore", 
				"ALSV4_CPP",
				"CerberusTools",
				"EnhancedInput"
			});
	}
}
