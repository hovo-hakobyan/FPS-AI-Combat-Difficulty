// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AI_Accuracy : ModuleRules
{
	public AI_Accuracy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule", "GameplayTasks", "Niagara", "UMG", "RigLogicLib", "SlateCore" });
	}
}
