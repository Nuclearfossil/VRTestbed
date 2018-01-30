// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class TestBed : ModuleRules
{
	public TestBed(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
        // PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });                

        // Increase build time?
        // MinFilesUsingPrecompiledHeaderOverride = 1;
        // bFasterWithoutUnity = true;

		// Remember to disable this once we want to ship.
		// OptimizeCode = CodeOptimization.Never;
	}
}
