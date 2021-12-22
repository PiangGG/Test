// Copyright Epic Games, Inc. All Rights Reserved.

#include "VSTStreamMediaSource.h"


UVSTStreamMediaSource::UVSTStreamMediaSource()
{

}

FName UVSTStreamMediaSource::GetDesiredPlayerName() const
{
	return FName(TEXT("VSTMedia"));
}

/* UMediaSource overrides
 *****************************************************************************/

FString UVSTStreamMediaSource::GetUrl() const
{

	return StreamUrl;
}

void UVSTStreamMediaSource::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
}
bool UVSTStreamMediaSource::Validate() const
{
	return StreamUrl.Contains(TEXT("://"));
}
