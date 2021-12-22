// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VSTTypes.h"


typedef void(*FVSTSDKInitSDK)();
typedef void(*FVSTSDKReleaseSDK)();

typedef void(*FVSTSDKNewPlayer)(void**);
typedef void(*FVSTSDKRemovePlayer)(void*);

typedef void(*FVSTSDKSetVideoFmt)(void*,int);
typedef void(*FVSTSDKSetPlayerParameter)(void*, CommonParameter);
typedef void(*FVSTSDKOpenUrl)(void*,const char*);
typedef void(*FVSTSDKClose)(void*);

typedef EStateCode(*FVSTSDKGetPlayerState)(void*);
typedef int(*FVSTSDKGetPlayerFPS)(void*);

typedef void(*FVSTSDKBindFrameDataCB)(void*,void*, MediaFrameCallBack);
typedef void(*FVSTSDKBindEventCB)(void*, void*, EventCallBack);

typedef const char* (*FVSTSDKGetSDKErrorInfo)(void*, int32& );