// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "VSTMediaTracks.h"
#include "VST.h"

//#include "MediaHelpers.h"


#define LOCTEXT_NAMESPACE "FVSTMediaTracks"



FVSTMediaTracks::FVSTMediaTracks()
{ 
	VSTMediaPlayer = nullptr;
}



void FVSTMediaTracks::Initialize(void* Player,FString& OutInfo)
{
	Shutdown();

	UE_LOG(LogVSTMedia, Verbose, TEXT("Tracks: %p: Initializing tracks"), this);

	VSTMediaPlayer = Player;

	FTrack Track;
	Track.DisplayName = FText::FromString(TEXT("VST player track"));
	Track.Id = 0;
	Track.Name= TEXT("VST player track");
	VideoTracks.Add(Track);
}


void FVSTMediaTracks::Shutdown()
{
	VSTMediaPlayer = nullptr;
}


bool FVSTMediaTracks::GetAudioTrackFormat(int32 TrackIndex, int32 FormatIndex, FMediaAudioTrackFormat& OutFormat) const
{
	if (!AudioTracks.IsValidIndex(TrackIndex) || (FormatIndex != 0))
	{
		return false;
	}

	// @todo gmp: fix audio format
	OutFormat.BitsPerSample = 0;
	OutFormat.NumChannels = 2;
	OutFormat.SampleRate = 44100;
	OutFormat.TypeName = TEXT("PCM");

	return true;
}


int32 FVSTMediaTracks::GetNumTracks(EMediaTrackType TrackType) const
{
	switch (TrackType)
	{
	case EMediaTrackType::Audio:
		return AudioTracks.Num();

	case EMediaTrackType::Caption:
		return CaptionTracks.Num();

	case EMediaTrackType::Video:
		return VideoTracks.Num();

	default:
		return 0;
	}
}


int32 FVSTMediaTracks::GetNumTrackFormats(EMediaTrackType TrackType, int32 TrackIndex) const
{
	return ((TrackIndex >= 0) && (TrackIndex < GetNumTracks(TrackType))) ? 1 : 0;
}


int32 FVSTMediaTracks::GetSelectedTrack(EMediaTrackType TrackType) const
{
	switch (TrackType)
	{
	case EMediaTrackType::Audio:
		return INDEX_NONE;
		break;

	case EMediaTrackType::Caption:
		return INDEX_NONE;
		break;

	case EMediaTrackType::Video:
		return 0;
		break;
	}
	return INDEX_NONE;
}


FText FVSTMediaTracks::GetTrackDisplayName(EMediaTrackType TrackType, int32 TrackIndex) const
{
	switch (TrackType)
	{
	case EMediaTrackType::Audio:
		if (AudioTracks.IsValidIndex(TrackIndex))
		{
			return AudioTracks[TrackIndex].DisplayName;
		}
		break;

	case EMediaTrackType::Caption:
		if (CaptionTracks.IsValidIndex(TrackIndex))
		{
			return CaptionTracks[TrackIndex].DisplayName;
		}
		break;

	case EMediaTrackType::Video:
		if (VideoTracks.IsValidIndex(TrackIndex))
		{
			return VideoTracks[TrackIndex].DisplayName;
		}

	default:
		break;
	}

	return FText::GetEmpty();
}


int32 FVSTMediaTracks::GetTrackFormat(EMediaTrackType TrackType, int32 TrackIndex) const
{
	return (GetSelectedTrack(TrackType) != INDEX_NONE) ? 0 : INDEX_NONE;
}


FString FVSTMediaTracks::GetTrackLanguage(EMediaTrackType TrackType, int32 TrackIndex) const
{
	return TEXT("und"); // libvlc currently doesn't provide language codes
}


FString FVSTMediaTracks::GetTrackName(EMediaTrackType TrackType, int32 TrackIndex) const
{
	switch (TrackType)
	{
	case EMediaTrackType::Audio:
		if (AudioTracks.IsValidIndex(TrackIndex))
		{
			return AudioTracks[TrackIndex].Name;
		}
		break;

	case EMediaTrackType::Caption:
		if (CaptionTracks.IsValidIndex(TrackIndex))
		{
			return CaptionTracks[TrackIndex].Name;
		}
		break;

	case EMediaTrackType::Video:
		if (VideoTracks.IsValidIndex(TrackIndex))
		{
			return VideoTracks[TrackIndex].Name;
		}

	default:
		break;
	}

	return FString();
}


bool FVSTMediaTracks::GetVideoTrackFormat(int32 TrackIndex, int32 FormatIndex, FMediaVideoTrackFormat& OutFormat) const
{

	OutFormat.Dim = FIntPoint(720,576);
	OutFormat.FrameRate = 25;
	OutFormat.FrameRates = TRange<float>(OutFormat.FrameRate);
	OutFormat.TypeName = TEXT("Default");
	
	return true;
}


bool FVSTMediaTracks::SelectTrack(EMediaTrackType TrackType, int32 TrackIndex)
{
	

	return true;
}


bool FVSTMediaTracks::SetTrackFormat(EMediaTrackType TrackType, int32 TrackIndex, int32 FormatIndex)
{
	switch (TrackType)
	{
	case EMediaTrackType::Audio:
		return AudioTracks.IsValidIndex(TrackIndex);

	case EMediaTrackType::Caption:
		return CaptionTracks.IsValidIndex(TrackIndex);

	case EMediaTrackType::Video:
		return VideoTracks.IsValidIndex(TrackIndex);

	default:
		return false;
	}
		

}


#undef LOCTEXT_NAMESPACE
