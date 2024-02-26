// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class unreal_5_1_three_c : ModuleRules
{
	public unreal_5_1_three_c(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "HeadMountedDisplay", "EnhancedInput", "Sockets", "Networking", "SlateCore" });
	}
}
