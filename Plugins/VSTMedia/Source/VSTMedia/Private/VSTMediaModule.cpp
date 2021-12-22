// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "IVSTMediaModule.h"
#include "VSTMediaPrivate.h"

#include "HAL/FileManager.h"
#include "Misc/OutputDeviceFile.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"
#include "UObject/Class.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/WeakObjectPtr.h"

#include "VST.h"
#include "VSTMediaPlayer.h"


DEFINE_LOG_CATEGORY(LogVSTMedia);

#define LOCTEXT_NAMESPACE "FVSTMediaModule"


/**
 * Implements the VSTMedia module.
 */
class FVSTMediaModule
	: public IVSTMediaModule
{
public:

	/** Default constructor. */
	FVSTMediaModule()
		: Initialized(false)
	{ }

public:

	//~ IVSTMediaModule interface

	virtual TSharedPtr<IMediaPlayer, ESPMode::ThreadSafe> CreatePlayer(IMediaEventSink& EventSink) override
	{
		if (!Initialized)
		{
			return nullptr;
		}

		return MakeShared<FVSTMediaPlayer, ESPMode::ThreadSafe>(EventSink);
	}

public:

	//~ IModuleInterface interface

	virtual void StartupModule() override
	{
		// initialize LibVLC
		if (!FVST::Initialize())
		{
			UE_LOG(LogVSTMedia, Error, TEXT("Failed to initialize VSTSDK"));
			return;
		}
		
	
		Initialized = true;
	}

	virtual void ShutdownModule() override
	{
		if (!Initialized)
		{
			return;
		}

		Initialized = false;

	

		// shut down VSTSDK
		FVST::Shutdown();
	}

private:


private:

	/** Whether the module has been initialized. */
	bool Initialized;

};


IMPLEMENT_MODULE(FVSTMediaModule, VSTMedia);


#undef LOCTEXT_NAMESPACE
