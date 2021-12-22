// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using System.IO;

namespace UnrealBuildTool.Rules
{
	using System.IO;

	public class VSTMedia : ModuleRules
	{
		public VSTMedia(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
			bEnableUndefinedIdentifierWarnings = false;
			DynamicallyLoadedModuleNames.AddRange(
				new string[] {
					"Media",
				});

			PrivateDependencyModuleNames.AddRange(
				new string[] {
					"Core",
					"CoreUObject",
					"MediaUtils",
                    "MediaAssets",
					"Projects",
					"RenderCore",
					"VSTMediaFactory",
				});

			PrivateIncludePathModuleNames.AddRange(
				new string[] {
					"Media",
				});

			PrivateIncludePaths.AddRange(
				new string[] {
					"VSTMedia/Private",
					"VSTMedia/Private/Player",
					"VSTMedia/Private/Shared",
					"VSTMedia/Private/VST",
				});

            // add VLC libraries
            string BaseDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", ".."));
            string VSTDllDirectory = Path.Combine(BaseDirectory, "ThirdParty", "VST", "Dll", Target.Platform.ToString());
            string VSTIncludeDirectory = Path.Combine(BaseDirectory, "ThirdParty", "VST", "Include");

            if (Target.Platform == UnrealTargetPlatform.Win64)
            {
                PublicSystemIncludePaths.Add(VSTIncludeDirectory);
                if (Directory.Exists(VSTDllDirectory))
                {
                    foreach (string Plugin in Directory.EnumerateFiles(VSTDllDirectory, "*.*", SearchOption.AllDirectories))
                    {
                        RuntimeDependencies.Add(Path.Combine(VSTDllDirectory, Plugin));
                    }
                }
            }

        }

    }
}
