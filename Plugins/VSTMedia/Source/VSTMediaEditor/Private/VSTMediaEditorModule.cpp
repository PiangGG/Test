// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
//#include "LicenseInterface.h"
//#include "LicenseManagerEditor.h"
#include "VSTStreamMediaSource.h"
#include "VideoPlayer.h"

/**
 * Implements the VlcMediaEditor module.
 */
class FVSTMediaManager //: public ILicenseInterface
{
	FVSTMediaManager(const FVSTMediaManager&) {}
	FVSTMediaManager& operator=(const FVSTMediaManager&) {}
	~FVSTMediaManager()
	{
		//FLicenseManager::Get()->UnRegisterLicensedObject(this);
	}
	FVSTMediaManager()
	{
		TArray<UClass*> RegisteredClassNames;
		RegisteredClassNames.Add(UVSTStreamMediaSource::StaticClass());
		RegisteredClassNames.Add(UVideoPlayer::StaticClass());
		//FLicenseManager::Get()->RegisterLicensingUObject(this, RegisteredClassNames);
	}
public:
	static FVSTMediaManager& Get()
	{
		static FVSTMediaManager Singleton;
		return Singleton;
	}
};

class FVSTMediaEditorModule
	: public IModuleInterface
{
public:

	//~ IModuleInterface interface

	virtual void StartupModule() override 
	{ 
		FVSTMediaManager& VSTMediaManager = FVSTMediaManager::Get();
	}
	virtual void ShutdownModule() override
	{ 

	}
};




IMPLEMENT_MODULE(FVSTMediaEditorModule, VSTMediaEditor);
