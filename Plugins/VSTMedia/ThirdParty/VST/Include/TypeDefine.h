#pragma once


enum EEventCode
{
	Event_NONE = 0x000001,
	Event_OpenInputFailed ,
	Event_StartDecode,
	Event_Playing,
	Event_Ended,
	Event_ReadFrameError,
	Event_Error,
};

enum EStateCode
{
	State_NONE = 0x000001,
	State_BeginOpenInput,
	State_OpenInputSuccess,
	State_OpenInputFailed,
	State_FindStreamInfoSuccess,
	State_FindStreamInfoFailed,
	State_Playing,
};


enum EPixFmtType 
{
	PixFmtType_BGRA,
	PixFmtType_YUV,
};

enum EStreamProtocolType
{
	EProtocol_RTSP,
	EProtocol_RTMP,
	EProtocol_Unkown,
};


struct RGBAFrameData
{
	unsigned char* pBuff;
	int      nWidth;
	int      nHeight;
	int      nBuffSize;
	EPixFmtType Fmt;
};

struct CommonParameter 
{
	bool  bNoBuffer;
	int Timeout;
	int AnalyzedurationTime;
	int MaxDelayTime;
	int   ProbeSize;
	bool  bUseTcp;

	CommonParameter() 
	{
		bNoBuffer = true;
		Timeout = 20000000;
		AnalyzedurationTime = 1000000;
		MaxDelayTime = 500000;
		ProbeSize = 1 * 1024 * 1024;
		bUseTcp = false;
	}
};



typedef void( * MediaFrameCallBack)(void* UserData,RGBAFrameData* Data);
typedef void( * EventCallBack)(void* UserData,EEventCode Code);
