// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"

/**  */
class VST_VIDEO_API FVideoUIStyle
{
public:

	static void Initialize();

	static void Shutdown();

	/** reloads textures used by slate renderer */
	static void ReloadTextures();

	/** @return The Slate style set for the UMG Style */
	static const ISlateStyle& Get();

	static FName GetStyleSetName();


private:
	static TSharedPtr<class FSlateStyleSet > VideoUIStyle;
};

class FVST_VideoModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
