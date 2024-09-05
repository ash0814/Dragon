// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Dragon : ModuleRules
{
	public Dragon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "NavigationSystem", "AIModule" });
	}
}
