// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "VSTMediaSource.h"
#include "VSTMediaPrivate.h"

#include "VST.h"
#include <windows.h>





FVSTMediaSource::FVSTMediaSource()
{

}

FTimespan FVSTMediaSource::GetDuration() const
{
	int64 Duration = 0;

	if (Duration < 0)
	{
		return FTimespan::Zero();
	}

	return FTimespan(Duration * ETimespan::TicksPerMillisecond);
}

void TCHAR_to_char(const TCHAR* tchar, char*& _char)
{
	int iLength;
	//获取字节长度   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//分配目标缓存
	_char = new char[iLength];
	memset(_char, 0, iLength);
	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}
bool FVSTMediaSource::OpenUrl(const FString& Url)
{
	FVST::NewPlayer(&VSTMediaPlayer);
	if (VSTMediaPlayer == nullptr)
	{
		return false;
	}
	CurrentUrl = Url;
	FVST::SetVideoFmt(VSTMediaPlayer, EPixFmtType::PixFmtType_BGRA);

	const UVSTMediaSettings* Setting = GetDefault<UVSTMediaSettings>();
	CommonParameter Parameter;
	Parameter.bNoBuffer = Setting->bNoBuffer;
	Parameter.AnalyzedurationTime = Setting->Analyzedurationtime;
	Parameter.bUseTcp = Setting->bUseTCP;
	Parameter.MaxDelayTime = Setting->RtspMaxDelayTime;
	Parameter.ProbeSize = Setting->ProbeSize;
	Parameter.Timeout = Setting->Timeout;
	FVST::SetPlayerParameter(VSTMediaPlayer, Parameter);

	char* cstr_Url=nullptr;
	TCHAR_to_char(*Url, cstr_Url);
	const char* cstr_Url2 = cstr_Url;
	FVST::OpenUrl(VSTMediaPlayer, cstr_Url2);
	if (cstr_Url)
	{
		delete cstr_Url;
		cstr_Url = nullptr;
	}
	return true;
}


void FVSTMediaSource::Close()
{
	if (VSTMediaPlayer != nullptr)
	{
		FVST::RemovePlayer(VSTMediaPlayer);

		VSTMediaPlayer = nullptr;
	}

	Data.Reset();
	CurrentUrl.Reset();
}



