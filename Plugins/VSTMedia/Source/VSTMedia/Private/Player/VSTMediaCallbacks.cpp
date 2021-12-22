// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "VSTMediaCallbacks.h"
#include "VSTMediaPrivate.h"

#include "IMediaAudioSample.h"
#include "IMediaOptions.h"
#include "IMediaTextureSample.h"
#include "MediaSamples.h"


#include "VSTMediaAudioSample.h"
#include "VSTMediaTextureSample.h"


/* FVSTMediaOutput structors
 *****************************************************************************/

FVSTMediaCallbacks::FVSTMediaCallbacks()
	: AudioChannels(0)
	, AudioSampleFormat(EMediaAudioSampleFormat::Int16)
	, AudioSamplePool(new FVSTMediaAudioSamplePool)
	, AudioSampleRate(0)
	, AudioSampleSize(0)
	, CurrentTime(FTimespan::Zero())
	, Samples(new FMediaSamples)
	, VideoBufferDim(FIntPoint::ZeroValue)
	, VideoBufferStride(0)
	, VideoFrameDuration(FTimespan::Zero())
	, VideoOutputDim(FIntPoint::ZeroValue)
	, VideoPreviousTime(FTimespan::MinValue())
	, VideoSampleFormat(EMediaTextureSampleFormat::CharAYUV)
	, VideoSamplePool(new FVSTMediaTextureSamplePool)
	, VSTMediaPlayer(nullptr)
{ }


FVSTMediaCallbacks::~FVSTMediaCallbacks()
{
	Shutdown();

	
	delete AudioSamplePool;
	AudioSamplePool = nullptr;

	delete Samples;
	Samples = nullptr;

	delete VideoSamplePool;
	VideoSamplePool = nullptr;
}


/* FVSTMediaOutput interface
 *****************************************************************************/

IMediaSamples& FVSTMediaCallbacks::GetSamples()
{
	return *Samples;
}


void FVSTMediaCallbacks::Initialize(void* Player)
{
	
	Shutdown();
	VSTMediaPlayer = Player;

	FVST::BindFrameDataCB(VSTMediaPlayer,this, &FVSTMediaCallbacks::StaticVideoFrameCallback);

}


void FVSTMediaCallbacks::Shutdown()
{
	if (VSTMediaPlayer)
	{
		//FVST::BindFrameDataCB(VSTMediaPlayer,nullptr,nullptr);
	}
	VSTMediaPlayer = nullptr;
	AudioSamplePool->Reset();
	VideoSamplePool->Reset();

	CurrentTime = FTimespan::Zero();
	
}


void FVSTMediaCallbacks::StaticVideoFrameCallback(void* UserData, RGBAFrameData* FrameData)
{
	FVSTMediaCallbacks* Callbacks = (FVSTMediaCallbacks*)UserData;

	if (nullptr == Callbacks) 
	{
		return;
	}
	Callbacks->VideoOutputDim.X = FrameData->nWidth;
	Callbacks->VideoOutputDim.Y = FrameData->nHeight;

	Callbacks->VideoBufferDim = FIntPoint(FrameData->nWidth, FrameData->nHeight);

	switch (FrameData->Fmt)
	{
	case PixFmtType_YUV:
		Callbacks->VideoSampleFormat = EMediaTextureSampleFormat::CharAYUV;
		Callbacks->VideoBufferStride =FrameData->nWidth*3/2;
		break;
	case PixFmtType_BGRA:
		Callbacks->VideoSampleFormat = EMediaTextureSampleFormat::CharBGRA;
		Callbacks->VideoBufferStride = FrameData->nWidth * 4;
		break;
	
	}
	int32 FrameRate = FVST::GetPlayerFPS(Callbacks->VSTMediaPlayer);
	Callbacks->VideoFrameDuration = FTimespan::FromSeconds(1.0 / (FrameRate==0?25:FrameRate));
	auto VideoSample = Callbacks->VideoSamplePool->Acquire();

	if (VideoSample == nullptr)
	{
		
		return ;
	}

	if (!VideoSample->Initialize(
		Callbacks->VideoBufferDim,
		Callbacks->VideoOutputDim,
		Callbacks->VideoSampleFormat,
		Callbacks->VideoBufferStride,
		Callbacks->VideoFrameDuration))
	{
		
		return ;
	}
	VideoSample->SetTime(Callbacks->CurrentTime);
	void* Buffer = VideoSample->GetMutableBuffer();
	FMemory::Memcpy(Buffer, (void*)FrameData->pBuff, FrameData->nBuffSize);
	Callbacks->Samples->AddVideo(Callbacks->VideoSamplePool->ToShared(VideoSample));
}

/* FVSTMediaOutput static functions
*****************************************************************************/

