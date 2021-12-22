// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class VSTMediaEditor : ModuleRules
	{
		public VSTMediaEditor(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
			bEnableUndefinedIdentifierWarnings = false;
			PrivateDependencyModuleNames.AddRange(
				new string[] {
					"Core",
					"CoreUObject",
					"MediaAssets",
					"UnrealEd",
					"VST_Video"
				});

			PrivateIncludePaths.AddRange(
				new string[] {
					"VSTMediaEditor/Private",
				});
		}
	}
}
