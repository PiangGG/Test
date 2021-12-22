// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "VSTMediaPlayer.h"
#include "VSTMediaPrivate.h"

#include "IMediaEventSink.h"
#include "IMediaOptions.h"
#include "Misc/FileHelper.h"
#include "Serialization/ArrayReader.h"

#include "VSTMediaUtils.h"

/* FVSTMediaPlayer structors
 *****************************************************************************/

FVSTMediaPlayer::FVSTMediaPlayer(IMediaEventSink& InEventSink)
	: CurrentRate(0.0f)
	, CurrentTime(FTimespan::Zero())
	, EventSink(InEventSink)
	, ShouldLoop(false)
	, VSTMediaPlayer(nullptr)
{ }

FVSTMediaPlayer::~FVSTMediaPlayer()
{
	Close();
}

bool FVSTMediaPlayer::InitializePlayer()
{
	VSTMediaPlayer = MediaSource.GetMedia();

	if (VSTMediaPlayer == nullptr)
	{
		UE_LOG(LogVSTMedia, Warning, TEXT("Failed to initialize media player"));
		return false;
	}

	FVST::BindEventCB(VSTMediaPlayer, this, &FVSTMediaPlayer::StaticEventCallback);

	// initialize player
	CurrentRate = 0.0f;
	CurrentTime = FTimespan::Zero();

	EventSink.ReceiveMediaEvent(EMediaEvent::MediaOpened);

	return true;
}

/* IMediaControls interface
 *****************************************************************************/

bool FVSTMediaPlayer::CanControl(EMediaControl Control) const
{
	return false;
}

FTimespan FVSTMediaPlayer::GetDuration() const
{
	return MediaSource.GetDuration();
}

float FVSTMediaPlayer::GetRate() const
{
	return CurrentRate;
}

EMediaState FVSTMediaPlayer::GetState() const
{
	if (VSTMediaPlayer == nullptr)
	{
		return EMediaState::Closed;
	}

	EStateCode State = FVST::GetPlayerState(VSTMediaPlayer);

	switch (State)
	{
	case EStateCode::State_NONE:
	case EStateCode::State_BeginOpenInput:
	case EStateCode::State_OpenInputSuccess:
	case EStateCode::State_FindStreamInfoSuccess:
		return EMediaState::Preparing;
	case EStateCode::State_Playing:
		return EMediaState::Playing;
	case EStateCode::State_OpenInputFailed:
		return EMediaState::Error;
	}

	return EMediaState::Error; // should never get here
}

EMediaStatus FVSTMediaPlayer::GetStatus() const
{
	return (GetState() == EMediaState::Preparing) ? EMediaStatus::Buffering : EMediaStatus::None;
}

TRangeSet<float> FVSTMediaPlayer::GetSupportedRates(EMediaRateThinning Thinning) const
{
	TRangeSet<float> Result;

	if (Thinning == EMediaRateThinning::Thinned)
	{
		Result.Add(TRange<float>::Inclusive(0.0f, 10.0f));
	}
	else
	{
		Result.Add(TRange<float>::Inclusive(0.0f, 1.0f));
	}

	return Result;
}

FTimespan FVSTMediaPlayer::GetTime() const
{
	return CurrentTime;
}

bool FVSTMediaPlayer::IsLooping() const
{
	return ShouldLoop;
}

bool FVSTMediaPlayer::Seek(const FTimespan& Time)
{
	return false;
}

bool FVSTMediaPlayer::SetLooping(bool Looping)
{
	ShouldLoop = Looping;
	return true;
}

bool FVSTMediaPlayer::SetRate(float Rate)
{
	return false;
}

void FVSTMediaPlayer::StaticEventCallback(void* UserData, EEventCode Code)
{
	if (UserData == nullptr)
	{
		return;
	}

	if (UserData != nullptr)
	{
		((FVSTMediaPlayer*)UserData)->Events.Enqueue(Code);
	}
}

/* IMediaPlayer interface
 *****************************************************************************/

void FVSTMediaPlayer::Close()
{
	if (VSTMediaPlayer == nullptr)
	{
		return;
	}

	// detach callback handlers
	Callbacks.Shutdown();
	Tracks.Shutdown();
	View.Shutdown();

	//FVST::BindEventCB(VSTMediaPlayer, nullptr,nullptr);
	FVST::Close(VSTMediaPlayer);
	VSTMediaPlayer = nullptr;
	// reset fields
	CurrentRate = 0.0f;
	CurrentTime = FTimespan::Zero();
	MediaSource.Close();
	Info.Empty();

	// notify listeners
	EventSink.ReceiveMediaEvent(EMediaEvent::TracksChanged);
	EventSink.ReceiveMediaEvent(EMediaEvent::MediaClosed);
}

IMediaCache& FVSTMediaPlayer::GetCache()
{
	return *this;
}

IMediaControls& FVSTMediaPlayer::GetControls()
{
	return *this;
}

FString FVSTMediaPlayer::GetInfo() const
{
	return Info;
}

FGuid FVSTMediaPlayer::GetPlayerPluginGUID() const
{
	return FGuid();
}

FName FVSTMediaPlayer::GetPlayerName() const
{
	static FName PlayerName(TEXT("VSTMedia"));
	return PlayerName;
}

IMediaSamples& FVSTMediaPlayer::GetSamples()
{
	return Callbacks.GetSamples();
}

FString FVSTMediaPlayer::GetStats() const
{
	return FString();
}

IMediaTracks& FVSTMediaPlayer::GetTracks()
{
	return Tracks;
}

FString FVSTMediaPlayer::GetUrl() const
{
	return MediaSource.GetCurrentUrl();
}

IMediaView& FVSTMediaPlayer::GetView()
{
	return View;
}

bool FVSTMediaPlayer::Open(const FString& Url, const IMediaOptions* Options)
{
	Close();

	if (Url.IsEmpty())
	{
		return false;
	}
	MediaSource.OpenUrl(Url);
	if (!MediaSource.OpenUrl(Url))
	{
		return false;
	}

	return InitializePlayer();
}

bool FVSTMediaPlayer::Open(const TSharedRef<FArchive, ESPMode::ThreadSafe>& Archive, const FString& OriginalUrl, const IMediaOptions* /*Options*/)
{
	Close();

	return InitializePlayer();
}

void FVSTMediaPlayer::TickInput(FTimespan DeltaTime, FTimespan /*Timecode*/)
{
	if (VSTMediaPlayer == nullptr)
	{
		return;
	}

	// process events
	EEventCode Event;

	while (Events.Dequeue(Event))
	{
		switch (Event)
		{
		case Event_OpenInputFailed:
		{
			int32 ErrorCode = -1;
			const char* errorInfo = FVST::GetSDKErrorInfo(VSTMediaPlayer, ErrorCode);
			//Close();
		}
		break;
		case Event_StartDecode:

			Tracks.Initialize(VSTMediaPlayer, Info);
			Callbacks.Initialize(VSTMediaPlayer);
			View.Initialize();
			EventSink.ReceiveMediaEvent(EMediaEvent::TracksChanged);
			break;
		case Event_Ended:
		case Event_Error:

			FVST::Close(VSTMediaPlayer);
			// end hack

			Callbacks.GetSamples().FlushSamples();
			EventSink.ReceiveMediaEvent(EMediaEvent::PlaybackEndReached);

			break;
		case Event_Playing:
			EventSink.ReceiveMediaEvent(EMediaEvent::PlaybackResumed);
			break;
		case Event_ReadFrameError:
			UE_LOG(LogVSTMedia, Warning, TEXT("Failed read Frame "));
			break;
		}
	}

	EStateCode State = FVST::GetPlayerState(VSTMediaPlayer);

	if (State == EStateCode::State_Playing)
	{
		CurrentRate = 1.0f;
		CurrentTime += DeltaTime * 1;
	}
	else
	{
		CurrentRate = 0.0f;
	}

	Callbacks.SetCurrentTime(CurrentTime);
}