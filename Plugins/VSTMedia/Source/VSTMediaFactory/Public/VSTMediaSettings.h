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
	/** �Ƿ����軺��*/
	UPROPERTY(config,EditAnywhere, Category = "VSTMediaSetting")
	bool bNoBuffer = true;
	/**��ʱʱ�� ��λ ����*/
	UPROPERTY(config, EditAnywhere, Category = "VSTMediaSetting")
	int32 Timeout = 20000000;
	/**����ʱ�� ��λ ����*/
	UPROPERTY(config, EditAnywhere, Category = "VSTMediaSetting")
	int32 Analyzedurationtime = 1000000;
	/**rtsp ����ӳ�ʱ�� ��λ ����*/
	UPROPERTY(config, EditAnywhere, Category = "VSTMediaSetting")
	int32 RtspMaxDelayTime = 500000;

	/**probesize  ���������С Ĭ��1M*/
	UPROPERTY(config, EditAnywhere, Category = "VSTMediaSetting")
	int32 ProbeSize = 1*1024*1024;
	/** rtsp ����Э��*/
	UPROPERTY(config, EditAnywhere, Category = "VSTMediaSetting")
	bool bUseTCP = true;
};
