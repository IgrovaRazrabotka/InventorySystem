// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.


//In this C# file the important addition is "UMG", "Slate", "SlateCore" in the PublicDependencyModuleNames field.
//In order to use this system You must have these modules included as well.
using UnrealBuildTool;

public class IncomeSystem : ModuleRules
{
	public IncomeSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "Slate", "SlateCore", "AIModule" });
	}
}
