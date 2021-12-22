// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BaseMediaSource.h"
#include "Containers/UnrealString.h"
#include "UObject/ObjectMacros.h"

#include "VSTStreamMediaSource.generated.h"


UCLASS(BlueprintType)
class VST_VIDEO_API UVSTStreamMediaSource
	: public UBaseMediaSource
{
	GENERATED_BODY()

public:
	UVSTStreamMediaSource();
	/** The URL to the media stream to be played. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Stream, AssetRegistrySearchable)
	FString StreamUrl;
	virtual void Serialize(FArchive& Ar) override;
public:
	virtual FName GetDesiredPlayerName() const override;

	virtual FString GetUrl() const override;
	virtual bool Validate() const override;
};
