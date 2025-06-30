// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class PracticeGame : ModuleRules
{
	public PracticeGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        //bUseUnity = false;

		PublicDependencyModuleNames.AddRange(new string[] 
        {
            "Core",
            "CoreUObject", 
            "Engine", 
            "InputCore", 
            "EnhancedInput", 
            "UMG", 
            "Niagara",
            "AIModule",
            "NavigationSystem",
            "LevelSequence",
            "MovieScene",
            "MediaAssets"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {  });
       
        PublicIncludePaths.AddRange(new string[] { Path.Combine(EngineDirectory, "PlugIns/FX/Niagara/Source") });
        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
