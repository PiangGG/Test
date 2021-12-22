// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "VST.h"
#include "VSTMediaPrivate.h"

#include "HAL/PlatformProcess.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"


#define VST_DEFINE(Func) \
	FVSTSDK##Func FVST::Func = nullptr;

#define VST_IMPORT(Name, Func) \
	Func = (FVSTSDK##Func)FPlatformProcess::GetDllExport(CoreHandle, TEXT(#Name)); \
	if (Func == nullptr) \
	{ \
		UE_LOG(LogVSTMedia, Warning, TEXT("Failed to import VLC function %s"), TEXT(#Name)); \
		Shutdown(); \
		return false; \
	}


/* Static initialization
 *****************************************************************************/

void* FVST::CoreHandle = nullptr;

FString FVST::PluginDir;

VST_DEFINE(InitSDK);
VST_DEFINE(ReleaseSDK);
VST_DEFINE(NewPlayer);
VST_DEFINE(RemovePlayer);
VST_DEFINE(SetVideoFmt);
VST_DEFINE(SetPlayerParameter);
VST_DEFINE(OpenUrl);
VST_DEFINE(Close);
VST_DEFINE(GetPlayerState);
VST_DEFINE(GetPlayerFPS);

VST_DEFINE(BindFrameDataCB);
VST_DEFINE(BindEventCB);
VST_DEFINE(GetSDKErrorInfo);


/* FVST static functions
 *****************************************************************************/

FString FVST::GetPluginDir()
{
	return PluginDir;
}


bool FVST::Initialize()
{
	// determine directory paths
	const FString BaseDir = IPluginManager::Get().FindPlugin("VSTMedia")->GetBaseDir();
	const FString VSTDir = FPaths::Combine(*BaseDir, TEXT("ThirdParty"), TEXT("VST"),TEXT("Dll"));


#if PLATFORM_WINDOWS
	#if PLATFORM_64BITS
		const FString LibDir = FPaths::Combine(*VSTDir, TEXT("Win64"));
	#endif
#endif

	// load required libraries in the correct order
	if (!LoadDependency(LibDir, TEXT("VSTMedia"), CoreHandle))
	{
		return false;
	}


	PluginDir = FPaths::ConvertRelativePathToFull(FPaths::Combine(*LibDir, TEXT("plugins")));
    
	VST_IMPORT(InitSDK, InitSDK);
	VST_IMPORT(ReleaseSDK, ReleaseSDK);
	VST_IMPORT(NewPlayer, NewPlayer);
	VST_IMPORT(RemovePlayer, RemovePlayer);
	VST_IMPORT(SetVideoFmt, SetVideoFmt);
	VST_IMPORT(SetPlayerParameter, SetPlayerParameter);
	VST_IMPORT(OpenUrl, OpenUrl);
	VST_IMPORT(Close, Close);
	VST_IMPORT(GetPlayerState, GetPlayerState);
	VST_IMPORT(GetPlayerFPS, GetPlayerFPS);
	
	VST_IMPORT(BindFrameDataCB, BindFrameDataCB);
	VST_IMPORT(BindEventCB, BindEventCB);

	VST_IMPORT(GetSDKErrorInfo, GetSDKErrorInfo);
	InitSDK();
	return true;
}


void FVST::Shutdown()
{
	PluginDir.Empty();
	ReleaseSDK();
	FreeDependency(CoreHandle);
}


/* FVST static functions
 *****************************************************************************/

void FVST::FreeDependency(void*& Handle)
{
	if (Handle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(Handle);
		Handle = nullptr;
	}
}


bool FVST::LoadDependency(const FString& Dir, const FString& Name, void*& Handle)
{
	FString Lib = Name + TEXT(".") + FPlatformProcess::GetModuleExtension();
	FString Path = Dir.IsEmpty() ? *Lib : FPaths::Combine(*Dir, *Lib);

	Handle = FPlatformProcess::GetDllHandle(*Path);

	if (Handle == nullptr)
	{
		UE_LOG(LogVSTMedia, Warning, TEXT("Failed to load required library %s. Plug-in will not be functional."), *Lib);
		return false;
	}

	return true;
}
