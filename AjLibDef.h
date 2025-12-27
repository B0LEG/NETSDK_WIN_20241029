/*******************************************************
ґЛОДјюЅ«¶ЁТеТ»Р©°ІјСНюКУѕІМ¬їв№«№І¶ЁТе
*******************************************************/

#ifndef __AJLIBDEF__20170222__H__
#define __AJLIBDEF__20170222__H__

typedef struct
{
	LONG FrameType;
	DOUBLE TimeStamp;
}MPEG4_VIDEO_FRAME_EXTRA;

typedef struct
{
	LONG FrameType;
	DOUBLE TimeStamp;
}G711_AUDIO_FRAME_EXTRA;

typedef struct __VIDEO_STREAM_PARAM
{
	char codec[256];
	int width;
	int height;
	int colorbits;
	int framerate;
	int bitrate;
	char vol_data[256];
	int vol_length;
}VIDEO_STREAM_PARAM;

typedef struct  __AUDIO_STREAM_PARAM
{
	char codec[256];
	int samplerate;
	int bitspersample;
	int channels;
	int framerate;
	int bitrate;
}AUDIO_STREAM_PARAM;

//typedef struct  __STREAM_AV_PARAM
//{
//	unsigned char	ProtocolName[32];
//	short  bHaveVideo;//0 ±нКѕГ»УРКУЖµІОКэ
//	short  bHaveAudio;//0 ±нКѕГ»УРТфЖµІОКэ
//	VIDEO_STREAM_PARAM videoParam;//КУЖµІОКэ
//	AUDIO_STREAM_PARAM audioParam;//ТфЖµІОКэ
//	char		szUrlInfo[512];//ЧўТві¤¶И
//}STREAM_AV_PARAM;

enum AJPLAYER_ERROR_CODE
{
	ERR_PLY_INIT_ERROR = -999999,																	//LibївіхКј»ЇґнОу
	ERR_PLY_CHANNELID_ERROR,																		//ІҐ·ЕЖчѕд±ъґнОуЎЈ
	ERR_PLY_MEDIAPARAM_ERROR,																	//ГЅМеІОКэґнОу
	ERR_PLY_DECODEPARAM_ERROR,																//ЅвВлІОКэґнОу
	ERR_PLY_MEDIAPARAMCHANGED_ERROR,											//ГЅМеІОКэТСёД±д
	ERR_PLY_DECODERINITFAILED_ERROR,													//ЅвВлЖчіхКј»ЇК§°Ь
	ERR_PLY_NOPLAY_ERROR,
	ERR_PLY_INITFACEMODELPATH_ERROR,												//ИЛБіК¶±рµДїв ДЈРНВ·ѕ¶ґнОу
	ERR_POINTER_ISNULL,
};

enum AJPLAYER_PLAYMODE
{
	PLAYMODE_REALTIME,			//КµК±ДЈКЅ
	PLAYMODE_SMOOTH,			//Бчі©ДЈКЅ
};

/* ЅвВлДЈКЅ */
typedef enum
{
	DECODEMODE_SW = 0,				// ИнЅв
	DECODEMODE_HW,				// УІЅвВлїЅ±ґДЈКЅ
	DECODEMODE_HW_FAST,			// УІЅвВлЦ±ЅУПФКѕДЈКЅ
}AJDecodeType;
#endif
