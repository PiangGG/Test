// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
    public class VST_Video : ModuleRules
    {
        public VST_Video(ReadOnlyTargetRules Target) : base(Target)
        {

            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

            bEnableUndefinedIdentifierWarnings = false;
            
            PublicDependencyModuleNames.AddRange(new string[] { 
                "Core", 
                "CoreUObject", 
                "Engine", 
                "InputCore", 
                "MediaAssets",
                "UMG",
                "Slate", 
                "SlateCore"
            });

        }
    }
}
