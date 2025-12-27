#ifndef	__AJPLAYER_20171111_H
#define __AJPLAYER_20171111_H

#include "AjLibDef.h"
//

#ifdef AJPLAYER_EXPORTS
#define AJPLAYER_API __declspec(dllexport)
#define MAX_VIDEO_MASK_AREA 4
typedef struct
{
	int xPos;
	int yPos;
	int width;
	int height;
}MASK_AREA_ENTRY;

typedef struct
{
	MASK_AREA_ENTRY mainStreamMaskList[MAX_VIDEO_MASK_AREA];
	MASK_AREA_ENTRY subStreamMaskList[MAX_VIDEO_MASK_AREA];
}VideoMaskConfig;

#define MAX_VIDEO_ROI_AREA 4
typedef struct
{
	int xPos;
	int yPos;
	int width;
	int height;
}ROI_AREA_ENTRY;

typedef struct
{
	int enable;
	ROI_AREA_ENTRY roi[MAX_VIDEO_ROI_AREA];
}VideoROI;

typedef struct _AJ_POINT_S
{
	int x;
	int y;
}AJ_POINT_S;

typedef struct _AJ_SIZE_S
{
	int width;
	int height;
}AJ_SIZE_S;

#define MAX_POLYGON_POINT_CNT 10
typedef struct _Polygon
{
	AJ_POINT_S points[MAX_POLYGON_POINT_CNT];
	int count;
}Polygon__;
#else
#define AJPLAYER_API __declspec(dllimport)
#if USE_LIBNETSDK
#include "sdk_define.h"
#else
#include "data_struct.h"
#endif
#if _DEBUG
#pragma comment(lib, "AjPlayer.lib")
#else
#pragma comment(lib, "AjPlayer.lib")
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN64
#define AJPLAYER_HANDLE uintptr_t
#else
#define AJPLAYER_HANDLE LONG
#endif

	typedef struct
	{
		long nWidth;
		long nHeight;
		long nStamp;
		long nType;
		long nFrameRate;
		long bIsVideo;
		int  nLinseSize[4];
	}FRAME_INFO_AJ;

	typedef struct
	{
		double fX;
		double fY;
	} POINT_F;

#define VG_MAX_LINE_NUMBER 1
	typedef struct
	{
		BOOL bEnable;
		POINT point_start;
		POINT point_end;

		POINT_F point_left;	//ґ№Ц±ПЯЧу¶Лµг
		POINT_F point_right; //ґ№Ц±ПЯУТ¶Лµг

		POINT_F arrow_point_left_point_left;	//ґ№Ц±ПЯЧујэН·Чу¶ЛµгО»ЦГ
		POINT_F arrow_point_left_point_right;//ґ№Ц±ПЯЧујэН·ЧуУТµгО»ЦГ

		POINT_F arrow_point_right_point_left;//ґ№Ц±ПЯУТјэН·Чу¶ЛµгО»ЦГ
		POINT_F arrow_point_right_point_right;//ґ№Ц±ПЯУТјэН·ЧуУТµгО»ЦГ

		BOOL bDrawDirection;
		BOOL bDrawLeftArrow;
		BOOL bDrawRightArrow;
	}LineStruct;

	typedef struct
	{
		LineStruct data[VG_MAX_LINE_NUMBER];
	}VgLineStruct;

	typedef int (CALLBACK* fDecCallBackFunction)(AJPLAYER_HANDLE nPort, char* pBuf, long nSize, FRAME_INFO_AJ* pFrameInfo, void* pUser, long nReserved2);

	typedef int (CALLBACK* fQRcodeCallBackFunction)(AJPLAYER_HANDLE nPort, char* pBuf, void* pUser);

	/********************************************************
	№¦ДЬЈєіхКј»ЇГЅМеІҐ·ЕївЈ¬ФЪїЄКјК№УГГЅМеІҐ·ЕївЦ®З°±ШРлЗТЅцµчУГТ»ґОЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_Init();

	/********************************************************
	№¦ДЬЈєКН·ЕЛщУРІҐ·ЕЖчЧКФґЈ¬Т»°гФЪІ»ФЩРиТЄК№УГІҐ·ЕЖчµДК±єтµчУГЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_ReleaseAll();
	/************************************************************************
	№¦ДЬЈєЙиЦГYUVКэѕЭ»Шµч
	************************************************************************/
	AJPLAYER_API int 	AJ_PLAYER_SetDecCallBack(AJPLAYER_HANDLE nPort, fDecCallBackFunction func, void* pUser);
	/********************************************************
	№¦ДЬЈєіхКј»ЇІҐ·ЕЖчґ°їЪЎЈ
	hWndЈєІҐ·ЕЖчґ°їЪѕд±ъЎЈ
	·µ»ШЦµЈє>0±нКѕіЙ№¦Ј¬·µ»ШІҐ·ЕЖч±кѕд±ъЎЈ
			<=0 ·µ»ШґнОуВлЎЈ
	*********************************************************/
	AJPLAYER_API AJPLAYER_HANDLE  AJ_PLAYER_CreatePlayer(HWND hWnd);

	/********************************************************
	№¦ДЬЈєЙиЦГИЛБіДЈРНµДВ·ѕ¶(±ШРлФЪAJ_PLAYER_SetupDecoder Ц®З°µчУГ)
	ІОКэЈє
	appPathЈєИЛБіДЈРНµДВ·ѕ¶ЎЈ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦
		  -1 ±нКѕК§°ЬЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_SetFaceDetectModelPath(char* appPath);

	/********************************************************
	№¦ДЬЈєїЄЖфИЛБіјмІв
	ІОКэЈє
	·µ»ШЦµЈє0 ±нКѕіЙ№¦
		  -1 ±нКѕК§°ЬЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_SetFaceDetect(AJPLAYER_HANDLE nPort, BOOL bOpen);

	/********************************************************
	№¦ДЬЈє№Ш±ХИЛБіјмІв
	ІОКэЈє
	·µ»ШЦµЈє0 ±нКѕіЙ№¦
		  -1 ±нКѕК§°ЬЎЈ
	*********************************************************/
	AJPLAYER_API BOOL  AJ_PLAYER_GetFaceDetect(AJPLAYER_HANDLE nPort);
	/********************************************************
	№¦ДЬЈєЙиЦГЅвВлДЈКЅ
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	nModeЈєІОїјAJDecodeType
	·µ»ШЦµЈє0 ±нКѕіЙ№¦Ј¬·сФт·µ»ШґнОуВлЎЈ

	*********************************************************/
	AJPLAYER_API int AJ_PLAYER_SetDecodeMode(AJPLAYER_HANDLE nPort, int nMode);
	/********************************************************
	№¦ДЬЈєЙиЦГІҐ·ЕДЈКЅ
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	nModeЈєІОїјAJPLAYER_PLAYMODE
	·µ»ШЦµЈє0 ±нКѕіЙ№¦Ј¬·сФт·µ»ШґнОуВлЎЈ

	*********************************************************/

	AJPLAYER_API int AJ_PLAYER_SetPlayMode(AJPLAYER_HANDLE nPort, int nMode);

	/********************************************************
	№¦ДЬЈєЙиЦГІҐ·ЕДЈКЅ
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	·µ»ШЦµЈєІОїјAJPLAYER_PLAYMODEЈ¬·сФт·µ»ШґнОуВлЎЈ

	*********************************************************/

	AJPLAYER_API int AJ_PLAYER_GetPlayMode(AJPLAYER_HANDLE nPort);

	/********************************************************
	№¦ДЬЈє»сИЎЅвВлДЈКЅ
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	i
	·µ»ШЦµЈє>=0 ·µ»ШЅвВлДЈКЅЈ¬·сФт·µ»ШґнОуВлЎЈ

	*********************************************************/
	AJPLAYER_API int AJ_PLAYER_GetDecodeMode(AJPLAYER_HANDLE nPort);

	/********************************************************
	№¦ДЬЈєЙиЦГЅвВлІОКэ.
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	isVideo:ІҐ·ЕЖчАаРНЈ¬1--КУЖµЎў0--ТфЖµЎЈ
	pParam : ГЅМеІОКэЈ¬
			 µ±isAudio = 0К±
			 ЗлКдИлtypedef struct __VIDEO_STREAM_PARAM
			 {
			 char codec[256];
			 int width;
			 int height;
			 int colorbits;
			 int framerate;
			 int bitrate;
			 char vol_data[256];
			 int vol_length;
			 }VIDEO_PARAM;
			 µ±isAudio = 1К±
			 ЗлКдИлtypedef struct  __AUDIO_STREAM_PARAM
			 {
			 char codec[256];
			 int samplerate;
			 int bitspersample;
			 int channels;
			 int framerate;
			 int bitrate;
			 }AUDIO_PARAM;
	pSizeЈєГЅМеІОКэµДі¤¶ИЎЈ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦Ј¬·сФт·µ»ШґнОуВлЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_SetupDecoder(AJPLAYER_HANDLE nPort, int isVideo, PBYTE pParam, DWORD pSize);

	/*********************************************************
	№¦ДЬЈєґ«ИлГЅМеКэѕЭЎЈ
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	isVideoЈєКэѕЭАаРНЈє0-ТфЖµЈ¬1-КУЖµЎЈ
	pBufferЈєГЅМеКэѕЭЎЈ
	nSizeЈєКэѕЭґуРЎЎЈ
	pExtraЈє¶оНвКэѕЭ Т»°гК№УГПВГжµДЅб№№Ме
		MPEG4_VIDEO_FRAME_EXTRA
		G711_AUDIO_FRAME_EXTRA
	ExtraLengthЈє¶оНвКэѕЭі¤¶ИЎЈ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦Ј¬·сФт·µ»ШґнОуВлЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_DecodeFrame(AJPLAYER_HANDLE nPort, int isVideo, BYTE* pBuffer, DWORD nSize, BYTE* pExtra, DWORD ExtraLength);

	/*********************************************************
	№¦ДЬЈє¶БИЎХэФЪІҐ·ЕµДЅвВлєуµДКУЖµКэѕЭ(YV12ёсКЅ)Ј¬Ц»УРХэФЪІҐ·ЕК±ІЕФКРнµчУГ
	ІОКэЈєpFrameBuf ИЎіцµДКэѕЭЎЈ
		  pFrameBufSize КэѕЭі¤¶И
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦Ј¬·сФт·µ»ШґнОуВлЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_ReadOneFrame(AJPLAYER_HANDLE nPort, BYTE* pFrameBuf, LONG* pFrameBufSize);

	/*********************************************************
	№¦ДЬЈєїмЛЩЗеАнІҐ·ЕЖчЎЈ
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦Ј¬·сФт·µ»ШґнОуВлЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_ReleaseWithoutWait(AJPLAYER_HANDLE nPort);

	/*********************************************************
	№¦ДЬЈє№Ш±ХЅвВлЖчЎЈ
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	isVideoЈєКэѕЭАаРНЈє0-ТфЖµЈ¬1-КУЖµ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦Ј¬·сФт·µ»ШґнОуВлЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_CloseDecoder(AJPLAYER_HANDLE nPort, int isVideo);

	/*********************************************************
	№¦ДЬЈєґЭ»ЩІҐ·ЕЖчЎЈ
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦Ј¬·сФт·µ»ШґнОуВлЎЈ
	ЧўТвЈєТЄЙѕіэІҐ·ЕЖч ТтПИµчУГAJ_PLAYER_CloseDecoderЈЁЈ©Ј¬ЅЁТйИнјюНЛіцК±К№УГAJ_PLAYER_ReleaseAllЈЁЈ©
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_DeletePlayer(AJPLAYER_HANDLE nPort);

	/*********************************************************
	№¦ДЬЈєЛўРВІҐ·ЕЖчЈЁЈ©ЎЈ
	ІОКэЈє
	valueЈє1ЈєГ»УРКэѕЭКдИлµДК±єт»№»бПФКѕЧоєуТ»ЦЎ»­ГжЈ»
		   0ЈєГ»УРКэѕЭКдИлµДК±єтЦ±ЅУПФКѕєЪЖБЎЈ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦Ј¬·сФт·µ»ШґнОуВлЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_RefreshSurface(int value);

	/*********************************************************
	№¦ДЬЈєІҐ·ЕКУЖµ
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	Value 0-НЈЦ№ІҐ·ЕЈ¬1-їЄКјІҐ·ЕЎЈ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦Ј¬·сФт·µ»ШґнОуВлЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_SetPlayOn(AJPLAYER_HANDLE nPort, LONG Value);

	/*********************************************************
	№¦ДЬЈєІҐ·ЕТфЖµЈЁУРЗТЦ»ДЬїЄЖфТ»ёцІҐ·ЕЖчµДТфЖµЈ©
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦Ј¬·сФт·µ»ШґнОуВлЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_SetAudioOn(AJPLAYER_HANDLE nPort);

	/*********************************************************
	№¦ДЬЈєНЈЦ№ТфЖµ
	ІОКэЈє
	·µ»ШЦµЈє0 ±нКѕіЙ№¦Ј¬·сФт·µ»ШґнОуВлЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_SetAudioOff();

	/*********************************************************
	№¦ДЬЈєЙиЦГТфБї
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	nVolume: ТфБї
	·µ»ШЦµЈє0 ±нКѕіЙ№¦Ј¬·сФт·µ»ШґнОуВлЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_SetVolume(AJPLAYER_HANDLE nPort, LONG nVolume);

	/*********************************************************
	№¦ДЬЈє»сИЎТфБї
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	pVolume ТфБї
	·µ»ШЦµЈє0 ±нКѕіЙ№¦ЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_GetVolume(AJPLAYER_HANDLE nPort, LONG* pVolume);

	/*********************************************************
	№¦ДЬЈєЙиЦГёЯЗеК±КЗ·сИ«ЖБПФКѕ
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	bIsFullFillЈєКЗ·сИ«ЖБПФКѕЎЈ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦ЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_SetFullFillStatus(AJPLAYER_HANDLE nPort, LONG bIsFullFill);

	/*********************************************************
	№¦ДЬЈєИЎµГёЯЗеК±КЗ·сИ«ЖБПФКѕ
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	·µ»ШЦµЈє0 І»КЗИ«ЖБПФКѕЎЈ
			1 КЗИ«ЖБПФКѕЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_GetFullFillStatus(AJPLAYER_HANDLE nPort);

	/*********************************************************
	№¦ДЬЈєґ«ИлКу±к¶ЇЧч
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	·µ»ШЦµЈє0
	ЧўТвІ»ТЄґ«Л«»чКВјю
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_InputMouseEvent(AJPLAYER_HANDLE nPort, LONG nMsgType, WPARAM wp, LPARAM lp);

	/*********************************************************
	№¦ДЬЈєЙиЦГµзЧУ·ЕґуµДЧґМ¬
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦ЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_SetZoomRectOn(AJPLAYER_HANDLE nPort, LONG nType);

	/*********************************************************
	№¦ДЬЈє»сИЎµзЧУ·ЕґуµДЧґМ¬
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦ЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_GetZoomRectStatus(AJPLAYER_HANDLE nPort);
	/*********************************************************
	№¦ДЬЈєЙиЦГКЗ·сЅшРРТЖ¶ЇХмІв»­їт
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	bOpenЈєКЗ·сїЄЖф
	·µ»ШЦµЈє0 ±нКѕіЙ№¦ЎЈ
	*********************************************************/
	AJPLAYER_API int AJ_PLAYER_SetMotionDetectConfigOn(AJPLAYER_HANDLE nPort, LONG bOpen);

	/*********************************************************
	№¦ДЬЈєЙиЦГТЖ¶ЇХмІв»­їтКэѕЭ
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	xBlocksЈєКЗ·сїЄЖф
	yBlocksЈєКЗ·сїЄЖф
	pConfigStringЈєКЗ·сїЄЖф
	·µ»ШЦµЈє0 ±нКѕіЙ№¦ЎЈ
	*********************************************************/
	AJPLAYER_API int AJ_PLAYER_SetMotionDetectConfig(AJPLAYER_HANDLE nPort, LONG xBlocks, LONG yBlocks, char* pConfigString);
	/*********************************************************
	№¦ДЬЈє»сИЎТЖ¶ЇХмІв»­їтКэѕЭ
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	pConfigStringЈєКЗ·сїЄЖф
	·µ»ШЦµЈє0 ±нКѕіЙ№¦ЎЈ
	*********************************************************/
	AJPLAYER_API int AJ_PLAYER_GetMotionDetectConfigString(AJPLAYER_HANDLE nPort, char* pConfigString);
	///*********************************************************
	//№¦ДЬЈє»сИЎТЖ¶ЇХмІв»­їтКэѕЭ
	//ІОКэЈє
	//nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	//pConfigStringЈєКЗ·сїЄЖф
	//·µ»ШЦµЈє0 ±нКѕіЙ№¦ЎЈ
	//*********************************************************/
	AJPLAYER_API int AJ_PLAYER_SetPolygonModeFalse(AJPLAYER_HANDLE nPort);
	///*********************************************************
	//№¦ДЬЈє»сИЎТЖ¶ЇХмІв»­їтКэѕЭ
	//ІОКэЈє
	//nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	//pConfigStringЈєКЗ·сїЄЖф
	//·µ»ШЦµЈє0 ±нКѕіЙ№¦ЎЈ
	//*********************************************************/
	AJPLAYER_API int AJ_PLAYER_SetPolygonConfig(AJPLAYER_HANDLE nPort, Polygon__& config);
	///*********************************************************
	//№¦ДЬЈє»сИЎТЖ¶ЇХмІв»­їтКэѕЭ
	//ІОКэЈє
	//nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	//pConfigStringЈєКЗ·сїЄЖф
	//·µ»ШЦµЈє0 ±нКѕіЙ№¦ЎЈ
	//*********************************************************/
	AJPLAYER_API int AJ_PLAYER_GetPolygonConfig(AJPLAYER_HANDLE nPort, Polygon__& config);

	AJPLAYER_API int AJ_PLAYER_SetVideoMaskConfig(AJPLAYER_HANDLE nPort, VideoMaskConfig& config);
	AJPLAYER_API int AJ_PLAYER_GetVideoMaskConfig(AJPLAYER_HANDLE nPort, VideoMaskConfig& config);
	AJPLAYER_API int AJ_PLAYER_DisableVideoMaskMouseEvent(AJPLAYER_HANDLE nPort);

	AJPLAYER_API int AJ_PLAYER_SetLineConfig(AJPLAYER_HANDLE nPort, VgLineStruct& config);
	AJPLAYER_API int AJ_PLAYER_GetLineConfig(AJPLAYER_HANDLE nPort, VgLineStruct& config);
	AJPLAYER_API int AJ_PLAYER_SetLineConfigFalse(AJPLAYER_HANDLE nPort);
	/*********************************************************
	№¦ДЬЈє//Ѕ«µ±З°ХэФЪІҐ·ЕµДНј±ЈґжіЙОДјюЎЈ
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	FileFormat: ОДјюёсКЅЎЈ
	0 YUVКэѕЭЎЈ
	1 jpgНјЖ¬ЎЈ
	2 BMPНјЖ¬ЎЈ
	ЈЁ0-2 sDirName±нКѕДїВј ЧФ¶ЇЙъіЙОДјюГыЎЈЈ©
	3 JPGНјЖ¬ sDirName ±нКѕОДјюГы

	StoragePath ЈЁFileFormat ОЄ0-2К±Ј©ОЄґжґўДїВјЈ¬ЈЁFileFormat ОЄ3К±Ј©ОЄґжґўОДјюГыЎЈ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦ЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_SnapShot(AJPLAYER_HANDLE nPort, LONG FileFormat, char* sDirName);

	/*********************************************************
	№¦ДЬЈєІҐ·ЕКУЖµ
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦ЎЈ
	*********************************************************/
	//AJPLAYER_API int  AJ_PLAYER_SetAlarmStatus(AJPLAYER_HANDLE nPort, LONG alramCode, LONG duration);

	/*********************************************************
	№¦ДЬЈєЙиЦГВјПсЧґМ¬
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦ЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_SetRecordStatus(AJPLAYER_HANDLE nPort, LONG value);

	/*********************************************************
	№¦ДЬЈєЙиЦГ¶ФЅІЧґМ¬
	ІОКэЈє
	nPortЈєІҐ·ЕЖчѕд±ъЎЈ
	·µ»ШЦµЈє0 ±нКѕіЙ№¦ЎЈ
	*********************************************************/
	AJPLAYER_API int  AJ_PLAYER_SetTalkStatus(AJPLAYER_HANDLE nPort, LONG value);

#ifdef __cplusplus
}
#endif
#endif
