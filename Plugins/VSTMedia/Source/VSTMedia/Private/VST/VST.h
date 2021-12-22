// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VSTTypes.h"
#include "VSTImports.h"



class FVST
{
public:

	static const uint32 MaxPlanes = 5;

public:

	static FString GetPluginDir();
	static bool Initialize();
	static void Shutdown();

public:
	static FVSTSDKInitSDK  InitSDK;
	static FVSTSDKReleaseSDK ReleaseSDK;
	static FVSTSDKNewPlayer NewPlayer;
	static FVSTSDKRemovePlayer RemovePlayer;
	static FVSTSDKSetVideoFmt SetVideoFmt;
	static FVSTSDKSetPlayerParameter SetPlayerParameter;
	static FVSTSDKOpenUrl OpenUrl;
	static FVSTSDKClose Close;
	static FVSTSDKGetPlayerState GetPlayerState;
	static FVSTSDKGetPlayerFPS   GetPlayerFPS;
	static FVSTSDKBindFrameDataCB BindFrameDataCB;
	static FVSTSDKBindEventCB BindEventCB;

	static FVSTSDKGetSDKErrorInfo GetSDKErrorInfo;

	
public:

	static int64 Delay(int64 Timestamp)
	{
		return Timestamp ;
	}

protected:

	static void FreeDependency(void*& Handle);
	static bool LoadDependency(const FString& Dir, const FString& Name, void*& Handle);

private:

	static void* CoreHandle;
	static FString PluginDir;
};