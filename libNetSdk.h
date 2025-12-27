//
// Created by Alvin on 2018/5/8.
//

#ifndef LIBNETSDK_LIBNETSDK_H
#define LIBNETSDK_LIBNETSDK_H
#include "sdk_define.h"
#include "cmd_def.h"

#ifdef LibNetSDK_EXPORTS
#define LIB_NETSDK_API __declspec(dllexport)
#else
#ifdef libNetSdk_IS_LIB
#define LIB_NETSDK_API
#else
#define LIB_NETSDK_API
#if _DEBUG
#pragma comment(lib, "libNetSdk.lib")
#else
#pragma comment(lib, "libNetSdk.lib")
#endif

#endif
#endif

#define MAX_IPC_SERIALNUMBER	32
#define MAX_PLATFORM_ID			32
#define MAX_IPC_P2PID_LEN		128

typedef struct _IPC_ENTRY_AJ
{
	char				ipc_sn[MAX_IPC_SERIALNUMBER];
	char				platform_id[MAX_PLATFORM_ID];
	char				p2p_id[MAX_IPC_P2PID_LEN];
	char				deviceType[32];
	char				version[128];
	char				title[256];
	char				oem_sn[MAX_IPC_SERIALNUMBER];
	char				uuid[MAX_IPC_SERIALNUMBER];
	UserConfig			userCfg;
	MediaStreamConfig	streamCfg;
	LANConfig			lanCfg;
	int					p2pStatus;
	int					runnedtime;
	int					bChanged;
	int					ChannelNum;
	int					factoryMode;
}IPC_ENTRY_AJ;

#ifdef __cplusplus
extern "C" {
#endif
	//回调函数原型
	typedef long(CALLBACK* SearchIPCCallBack)(long nEventCode, long index, IPC_ENTRY_AJ* pResponse, void* pUser);
	typedef long(CALLBACK* FactoryCallBack)(long nEventCode, char* szDevSn, long index, long LedMode, long IrcutMode, void* pUser);
	typedef long(CALLBACK* StatusEventCallBack)(LIB_NETSDK_HANDLE lUserID, long nStateCode, char* pResponse, void* pUser);
	typedef long(CALLBACK* AUXResponseCallBack)(LIB_NETSDK_HANDLE lUserID, long lChannels, long nType, char* pResponse, void* pUser);
	typedef long(CALLBACK* fRealDataCallBack)(LIB_NETSDK_HANDLE lRealHandle, long nChannel, long dwDataType, char* pBuffer, long dwBufSize, LPFRAME_EXTDATA  pExtData);
	typedef	long(CALLBACK* fCaptureAudioCallBack)(unsigned char* pData, long dwBufSize, void* pUser);
	typedef long(CALLBACK* SerialDataCallBack) (LIB_NETSDK_HANDLE lUserID, unsigned char* pRecvDataBuffer, long dwBufSize, void* pUser);
	//sdk初始化和清理
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Init();
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Cleanup();
	//sdk设置回调
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SetSearchStatusCallBack(SearchIPCCallBack fcallBack, void* pUser);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SetStatusEventCallBack(StatusEventCallBack fStatusEventCallBack, void* pUser);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SetRealPlayStatusEventCallBack(StatusEventCallBack fStatusEventCallBack, void* pUser);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SetAUXResponseCallBack(AUXResponseCallBack fAUXCallBack, void* pUser);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SetReplayDataCallBack(fRealDataCallBack cbReplayDataCallBack, void* dwUser);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SetFactoryCallBack(FactoryCallBack cbFactoryCallBack, void* dwUser);
	//设备搜索
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_StartSearchIPC(char* identifier = "");
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_StopSearchIPC();
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SendSearchMsg();
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetSearchIPCCount();
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetIPCInfo(long index, IPC_ENTRY_AJ* pIPCInfo);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_ModifyIPC(long index, IPC_ENTRY_AJ* pIPCInfo, char* szUser, char* szPassword);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetOneIPAddress(char* strResult, int nSize);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetNetworkParam(unsigned long nParamIndex, char* strResult, int nSize);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_RestoreIPC(long index, IPC_ENTRY_AJ* pIPCInfo, char* szUser, char* szPassword);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_RestoreIPCBySn(char* sDevSn, char* szUser, char* szPassword);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_OpenFactory(char* sDevSn, char* szUser, char* szPassword, unsigned int destIp, char* ssid, char* pass);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SetAllNet(char* sDevSn, int nAllNet, char* szUser, char* szPassword);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SetFactoryCfg(char* sDevSn, int code, int LedMode, int IrcutMode, char* szUser, char* szPassword);
	//设备登录
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SetAutoReconnect(LIB_NETSDK_HANDLE lUserID, int bAutoReconnect);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Reconnect(LIB_NETSDK_HANDLE lUserID);
	LIB_NETSDK_API LIB_NETSDK_HANDLE __stdcall AJ_NETSDK_IPC_Login(char* sDVRIP, long nPort, char* sUserName, char* sPassword);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetLogInUserCount();
	LIB_NETSDK_API long __stdcall AJ_NETSDK_Server_Login(char* sDVRIP, long nPort, char* sUserName, char* sPassword, char* sUserIdentity);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Logout(LIB_NETSDK_HANDLE lUserID);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_LogoutAll();

	//设备设备视频流请求
	LIB_NETSDK_API LIB_NETSDK_HANDLE __stdcall AJ_NETSDK_IPC_RealPlay(char* pserverip, int channel, int streamType, char* user, char* pass, int nLinkMode, int nVideoPort, fRealDataCallBack cbRealDataCallBack, void* pUser);
	LIB_NETSDK_API LIB_NETSDK_HANDLE __stdcall AJ_NETSDK_IPC_RealPlayWithRtsp(char* rtspurl, char* user, char* pass, int nLinkMode, fRealDataCallBack cbRealDataCallBack, void* pUser);
	LIB_NETSDK_API LIB_NETSDK_HANDLE __stdcall AJ_NETSDK_IPC_RealPlayBySimple(char* pserverip, char* user, char* pass, fRealDataCallBack cbRealDataCallBack, int iStreamType, int  nVideoPort, int  bIsTcp, void* pUserData);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_StopRealPlay(LIB_NETSDK_HANDLE lRealHandle);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_StopAllRealPlay();

	//设备音频和对讲
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_StartTalk(int connectType, int destAudiotype, int samplerate, int bitspersample, int channels);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_StopTalk();
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_AddTalk(LIB_NETSDK_HANDLE lUserID);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_RemoveTalk(LIB_NETSDK_HANDLE lUserID);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_StartVoiceCom(LIB_NETSDK_HANDLE lUserID);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_StopVoiceCom(LIB_NETSDK_HANDLE lUserID);

	//音频采集（目前只支持windows系统）
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_StartAudioCapture(int audiotype, int samplerate, int bitspersample, int channels, fCaptureAudioCallBack cbCaptureAudioCallBack, void* pUser);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_StopAudioCapture();
	//自定义对讲数据接口
	//srcAudioType 输入的音频格式。
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_InputAudioData(int srcAudioType, unsigned char* pBuff, int len);

	//获取和设置设备配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetDeviceAbility(LIB_NETSDK_HANDLE lUserID, int nChannel);//设备能力集，诸如是否支持云台、音频等, 见：function_list.h。
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetDeviceConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, long dwCommand);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SetDevcieConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, long dwCommand, void* pInBuffer, long dwInBufferSize);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetDVRConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, long dwCommand);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SystemControl(LIB_NETSDK_HANDLE lUserID, int nChannel, long nCmdValue, long flag, char* pXml);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SetDVRConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, long dwCommand, void* pXml, long dwInBufferSize);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_CreateIFrame(LIB_NETSDK_HANDLE lUserID, int nChannel, int bIsSubStream);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_RebootDVR(LIB_NETSDK_HANDLE lUserID);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_RestoreConfig(LIB_NETSDK_HANDLE lUserID);

	//系统上传和下载文件（包括更新固件）
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Upgrade(LIB_NETSDK_HANDLE lUserID, char* sFileName);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_UploadFile(LIB_NETSDK_HANDLE lUserID, long fileType, char* filename);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SetConfigFile(LIB_NETSDK_HANDLE lUserID, char* sFileName);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetUpgradeProgress(LIB_NETSDK_HANDLE lUserID);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetUpgradeState(LIB_NETSDK_HANDLE lUserID);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_CloseUpgradeHandle(LIB_NETSDK_HANDLE lUserID);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetFileByName(LIB_NETSDK_HANDLE lUserID, long nFileType, char* sDVRFileName, char* saveDir);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetConfigFile(LIB_NETSDK_HANDLE lUserID, char* sFileName);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_StopGetFile(LIB_NETSDK_HANDLE lUserID);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetDownloadState(LIB_NETSDK_HANDLE lUserID);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetDownloadPos(LIB_NETSDK_HANDLE lUserID);
	//云台控制
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_PTZControl(LIB_NETSDK_HANDLE lUserID, int nChannel, long dwPTZCommand, long nTspeed, long nSpeed);
	//此接口支持485透传，支持自定义云台消息
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_PTZControlEx(LIB_NETSDK_HANDLE lUserID, int nChannel, char* pXml);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_PTZPreset(LIB_NETSDK_HANDLE lUserID, int nChannel, long dwPTZPresetCmd, long dwPresetIndex);
	//格式化存储设备
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_FormatDisk(LIB_NETSDK_HANDLE lUserID, long lDiskNumber);
	//设置和获取用户保存数据
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetUserData(LIB_NETSDK_HANDLE lUserID, char* pOutBuffer, int* nInOutLen);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SetUserData(LIB_NETSDK_HANDLE lUserID, char* pBuffer, int len);

	//ipc文件回放
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_ControlPlay(LIB_NETSDK_HANDLE lUserID, int nChannel, int iReplayMode, long Action, long param);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_PlayDeviceByFile(LIB_NETSDK_HANDLE lUserID, int nChannel, char* filenme);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_PlayDeviceByTime(LIB_NETSDK_HANDLE lUserID, int nChannel, long lTimescamp);

	//串口
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SerialStart(LIB_NETSDK_HANDLE lUserID, SerialDataCallBack cbSDCallBack, void* pUser);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SerialSend(LIB_NETSDK_HANDLE lUserID, long lChannel, char* pSendBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SerialStop(LIB_NETSDK_HANDLE lUserID);
	//add by Alvin.Chen @20170310
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetIOInPutStatus(LIB_NETSDK_HANDLE lUserID, int nChannelNo);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetIOOutPutStatus(LIB_NETSDK_HANDLE lUserID, int nChannelNo);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_UploadOEMAppFile(LIB_NETSDK_HANDLE lUserID, char* filePath);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_UploadOEMCfgFile(LIB_NETSDK_HANDLE lUserID, char* filePath);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_UploadOEMMp3File(LIB_NETSDK_HANDLE lUserID, char* filePath);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SearchOEMMp3File(LIB_NETSDK_HANDLE lUserID, int iPage);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_PlayMp3File(LIB_NETSDK_HANDLE lUserID, char* filePath, int nTimes);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_StopPlayMp3File(LIB_NETSDK_HANDLE lUserID);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetPlayAudioStatus(LIB_NETSDK_HANDLE lUserID);

	//获取设备配置新接口  细分获取配置，、
	//请求成功后获取的配置将在辅助通道回调函数中得到
	//辅助通道回调通过AJ_NETSDK_IPC_SetAUXResponseCallBack()设置
	//辅助通道回调中获得的配置信息是一串xml字符串 可以通过AJ_NETSDK_IPC_GetxxxByXml()接口解析转换得到相应的结构体
	//流媒体能力集
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_MediaCapability(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//流媒体配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_MediaStreamConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//媒体设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_MediaConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//视频配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_VideoConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//视频采集设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_VideoCaptureConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//视频编码设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_VideoEncodeConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//音频设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_AudioConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//音频采集配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_AudioCaputreConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//音频编码设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_AudioEncodeConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//告警配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_AlarmConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//移动侦测
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_MotionDetectAlarm(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//IO输入告警参数
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_InputAlarm(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//视频丢失告警参数
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_VideoLostAlarm(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//视频遮挡配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_VideoCoverAlarm(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//存储满告警配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_StorageFullAlarm(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//设备系统配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_SystemConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//云台配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_PtzConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//用户配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_UserConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//系统日志配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_SyslogConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//时区设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_TimeConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//设备系统语言设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_MiscConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//前端录像设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_RecordConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//osd设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_VideoOSDConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//平台设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_PlatformConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//28181配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_GB28181Config(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//网络配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_NetworkConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//局域网配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_NetworkLANConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//wifi配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_NetworkWIFIConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//ADSL拔号配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_NetworkADSLConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//DDNS配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_NetworkDDNSConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//UPNP配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_NetworkUPNPConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//P2P设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_NetworkP2PConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//FTP&SMTP相关配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_ServerConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_FtpServerConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GET_SmtpServerConfig(LIB_NETSDK_HANDLE lUserID, int nChannel);
	//设置设备配置新接口  采用结构体不在使用Xml配置
	//流媒体设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_MediaStreamConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, MediaStreamConfig* pConfig);
	//媒体设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_MediaConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, MediaConfig* pConfig);
	//视频配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_VideoConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, VideoConfig* pConfig);
	//视频采集设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_VideoCaptureConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, VideoCapture* pConfig);
	//视频编码设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_VideoEncodeConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, VideoEncode* pConfig);
	//OSD时间标题设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_VideoOSDConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, VideoOverlay* pConfig);
	//音频设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_AudioConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, AudioConfig* pConfig);
	//音频采集设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_AudioCaputreConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, AudioCapture* pConfig);
	//音频编码设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_AudioEncodeConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, AudioEncode* pConfig);
	//告警配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_AlarmConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, AlarmConfig* pConfig);
	//移动侦测
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_MotionDetectAlarm(LIB_NETSDK_HANDLE lUserID, int nChannel, MotionDetectAlarm* pConfig);
	//视频遮挡
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_VideoCoverAlarm(LIB_NETSDK_HANDLE lUserID, int nChannel, VideoCoverAlarm* pConfig);
	//隐私遮挡设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_VideoMaskConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, VideoMaskConfig* pConfig);
	//设备默认语言设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_MiscConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, MiscConfig* pConfig);
	//平台设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_PlatformConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, PlatformConfig* pConfig);
	//28181设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_GB28181Config(LIB_NETSDK_HANDLE lUserID, int nChannel, GB28181Config* pConfig);
	//用户列表设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_UserConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, UserConfig* pConfig);
	//时间同步设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_TimeConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, TimeConfig* pConfig);
	//当时间同步为手动时设置设备时间
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_TimeManualConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, TimeManualConfg* pConfig);
	//局域网设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_NetworkLANConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, LANConfig* pConfig);
	//Wifi设置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_NetworkWIFIConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, WIFIConfig* pConfig);
	//网络配置
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_SET_NetworkConfig(LIB_NETSDK_HANDLE lUserID, int nChannel, NetworkConfigNew* pConfig);

	/*
	去XML，使用结构体来获取/设置
	设置前，请确保数据正确，否则可能导致设备无法启动(最好先获取一遍，然后修改需要修改的项目)
	*/

	/*
	从XML解析出结构体
	*/
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetNetworkCfgByXml(NetworkConfigNew* pNetworkCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Network_getLANCfgByXml(LANConfig* lanCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Network_getWIFICfgByXml(WIFIConfig* wifiCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Network_getADSLCfgByXml(ADSLConfigNew* adslCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Network_getDDNSCfgByXml(DDNSConfig* ddnsCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Network_getUPNPCfgByXml(UPNPConfig* upnpCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Network_getP2PCfgByXml(P2PConfig* p2pCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Server_getFtpsByXml(ServerConfig* pServerCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Server_getSmtpsByXml(ServerConfig* pServerCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetServerCfgByXml(ServerConfig* pServerCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetRecordCfgByXml(RecordConfig* pRecordCfg, char* xmlBuf);
	LIB_NETSDK_API  long __stdcall AJ_NETSDK_IPC_Media_getCapabilityByXml(char* pRespone,
		int* pVideoCapCount, RESOLUTION_ENTRY** pOutVideoCap,
		int* pAudioCapCount, AUDIO_CODEC_ENTRY** pOutAudioCap);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Media_getAudioByXml(AudioConfig* pAudioCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Media_getVideoByXml(VideoConfig* pVideoCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetMediaCfgByXml(MediaConfig* pMediaCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Media_getVideoCaptureByXml(VideoCapture* pVideoCapture, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Media_getVideoUserOverlayByXml(VideoUserOverlay* pVideoOverlay, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Media_getVideoOverlayByXml(VideoOverlay* pVideoOverlay, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Media_getVideoEncodeByXml(VideoEncode* pVideoEncode, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Media_getJpegEncodeByXml(JpegEncodeCfg* pJpegCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Media_getVideoMaskByXml(VideoMaskConfig* pVideoMask, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetMediaStreamCfgByXml(MediaStreamConfig* pMediaStream, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetPlatformCfgByXml(PlatformConfig* pPlatform, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetGB28181CfgByXml(GB28181Config* pPlatformCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Alarm_getInputByXml(InputAlarm* pInputAlm, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Alarm_getMotionDetectByXml(MotionDetectAlarm* pMDAlm, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Alarm_getVideoLostByXml(VideoLostAlarm* pVideoLost, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Alarm_getVideoCoverByXml(VideoCoverAlarm* pVideoCover, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Alarm_getStorageFullByXml(StorageFullAlarm* pSFAlm, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetAlarmConfigByXml(AlarmConfig* pAlarmCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_InputAlarm_getAlarmChannelCfgByXml(AlarmChannel* pAlarmChannel, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_System_getPTZCfgByXml(PTZConfig* pPtzCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_System_getTimeCfgByXml(TimeConfig* pTimeCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_System_getUserCfgByXml(UserConfig* pUserCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_System_getLogCfgByXml(SyslogConfig* pSyslogCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_System_getMiscCfgByXml(MiscConfig* pMiscCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_GetSystemConfigByXml(SystemConfig* pSystemCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_System_getPTZCommonCfgByXml(PTZCommonConfig* pPtzCommonCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_System_getPTZAdvanceCfgByXml(PTZAdvanceConfig* pPtzAdvanceCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Media_getAudioCaptureByXml(AudioCapture* pCaptureCfg, char* xmlBuf);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_Media_getAudioEncodeByXml(AudioEncode* pEncodeCfg, char* xmlBuf);
	/*
	将结构体转换成XML
	XML内存为动态malloc ，用完后需要调用者手动free，否则会造成内存泄漏
	释放内存必须使用AJ_NETSDK_IPC_XMLGET_SAFE_FREE 接口进行释放。
	*/
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_SystemConfig(SystemConfig* pSystemCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_UserConfig(UserConfig* pUserCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_TimeConfig(TimeConfig* pTimeCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_SyslogConfig(SyslogConfig* pSyslogCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_SysMiscConfig(MiscConfig* pMiscCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_PTZConfig(PTZConfig* pPtzCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_AlarmConfig(AlarmConfig* pAlarmCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_InputAlarmConfig(InputAlarm* pInputAlm);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_MDAlarmConfig(MotionDetectAlarm* pMDAlm);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_VlAlarmConfig(VideoLostAlarm* pVideoLost);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_VCAlarmConfig(VideoCoverAlarm* pVideoCover);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_SFAlarmConfig(StorageFullAlarm* pSFAlm);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_MediaStreamConfig(MediaStreamConfig* mediaStreamCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_PlatformConfig(PlatformConfig* platformCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_GB28181Config(GB28181Config* platformCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_RecordConfig(RecordConfig* recordCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_NetworkConfig(NetworkConfigNew* networkCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_NetworkLANConfig(LANConfig* lanCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_NetworkWIFIConfig(WIFIConfig* wifiCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_NetworkADSLConfig(ADSLConfigNew* adslCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_NetworkDDNSConfig(DDNSConfig* ddnsCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_NetworkUPNPConfig(UPNPConfig* upnpCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_NetworkP2PConfig(P2PConfig* pCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_ServerConfig(ServerConfig* serverCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_FtpConfig(FtpServerList* fptServerList);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_SmtpConfig(SmtpServerList* smtpServerList);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_MediaConfig(MediaConfig* mediaCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_AudioConfig(AudioConfig* audioCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_VideoConfig(VideoConfig* videoCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_VideoUserOverlayConfig(VideoUserOverlay* pCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_VideoOverlayConfig(VideoOverlay* pCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_VideoMaskConfig(VideoMaskConfig* pCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_VideoCaptureConfig(VideoCapture* pCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_VideoEncodeConfig(VideoEncode* pCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_AudioCaptureConfig(AudioCapture* audioCfg);
	LIB_NETSDK_API char* __stdcall AJ_NETSDK_IPC_XMLGET_AudioEncodeConfig(AudioEncode* audioCfg);
	LIB_NETSDK_API long __stdcall AJ_NETSDK_IPC_XMLGET_SAFE_FREE(void* p);

#ifdef __cplusplus
}
#endif
#endif //LIBNETSDK_LIBNETSDK_H
