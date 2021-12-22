// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IMediaAudioSample.h"
#include "IMediaTextureSample.h"
#include "VST.h"

class FMediaSamples;
class FVSTMediaAudioSamplePool;
class FVSTMediaTextureSamplePool;
class IMediaOptions;
class IMediaAudioSink;
class IMediaOverlaySink;
class IMediaSamples;
class IMediaTextureSink;




/**
 * Handles VLC callbacks.
 */
class FVSTMediaCallbacks
{
public:

	/** Default constructor. */
	FVSTMediaCallbacks();

	/** Virtual destructor. */
	~FVSTMediaCallbacks();

public:

	/**
	 * Get the output media samples.
	 *
	 * @return Media samples interface.
	 */
	IMediaSamples& GetSamples();

	/**
	 * Initialize the handler for the specified media player.
	 *
	 * @param InPlayer The media player that owns this handler.
	 */
	void Initialize(void* Player);

	/**
	 * Set the player's current time.
	 *
	 * @param Time The player's play time.
	 */
	void SetCurrentTime(FTimespan Time)
	{
		CurrentTime = Time;
	}

	/** Shut down the callback handler. */
	void Shutdown();

private:

	/** Handles buffer unlock callbacks from VLC. */
	static void StaticVideoFrameCallback(void* UserData, RGBAFrameData* FrameData);

private:

	/** Current number of channels in audio samples( accessed by VLC thread only). */
	uint32 AudioChannels;

	/** Current audio sample format (accessed by VLC thread only). */
	EMediaAudioSampleFormat AudioSampleFormat;

	/** Audio sample object pool. */
	FVSTMediaAudioSamplePool* AudioSamplePool;

	/** Current audio sample rate (accessed by VLC thread only). */
	uint32 AudioSampleRate;

	/** Size of a single audio sample (in bytes). */
	SIZE_T AudioSampleSize;

	/** The player's current time. */
	FTimespan CurrentTime;



	/** The output media samples. */
	FMediaSamples* Samples;

	/** Current video buffer dimensions (accessed by VLC thread only; may be larger than VideoOutputDim). */
	FIntPoint VideoBufferDim;

	/** Number of bytes per row of video pixels. */
	uint32 VideoBufferStride;

	/** Current duration of video frames. */
	FTimespan VideoFrameDuration;

	/** Current video output dimensions (accessed by VLC thread only). */
	FIntPoint VideoOutputDim;

	/** Play time of the previous frame. */
	FTimespan VideoPreviousTime;

	/** Current video sample format (accessed by VLC thread only). */
	EMediaTextureSampleFormat VideoSampleFormat;

	/** Video sample object pool. */
	FVSTMediaTextureSamplePool* VideoSamplePool;

	void* VSTMediaPlayer;

};