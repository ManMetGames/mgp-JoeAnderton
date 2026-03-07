// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FT_ReloadingAndAmmo : ModuleRules
{
	public FT_ReloadingAndAmmo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
	}
}
