// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreTypes.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

#include "VSTMediaSettings.generated.h"



/**
 * Settings for the VSTMedia plug-in.
 */
UCLASS(config=Game, defaultconfig, meta = (DisplayName = "VST Media Settings"))
class VSTMEDIAFACTORY_API UVSTMediaSettings
	: public UObject
{
	GENERATED_BODY()

public:

	/** Default constructor. */
	UVSTMediaSettings();

public:
	/** 是否无需缓存*/
	UPROPERTY(config,EditAnywhere, Category = "VSTMediaSetting")
	bool bNoBuffer = true;
	/**超时时间 单位 毫秒*/
	UPROPERTY(config, EditAnywhere, Category = "VSTMediaSetting")
	int32 Timeout = 20000000;
	/**分析时间 单位 毫秒*/
	UPROPERTY(config, EditAnywhere, Category = "VSTMediaSetting")
	int32 Analyzedurationtime = 1000000;
	/**rtsp 最大延迟时间 单位 毫秒*/
	UPROPERTY(config, EditAnywhere, Category = "VSTMediaSetting")
	int32 RtspMaxDelayTime = 500000;

	/**probesize  分析缓存大小 默认1M*/
	UPROPERTY(config, EditAnywhere, Category = "VSTMediaSetting")
	int32 ProbeSize = 1*1024*1024;
	/** rtsp 传输协议*/
	UPROPERTY(config, EditAnywhere, Category = "VSTMediaSetting")
	bool bUseTCP = true;
};
