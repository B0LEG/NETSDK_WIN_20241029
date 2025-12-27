// UCRemoteConfig.h : UCRemoteConfig DLL µДЦчН·ОДјю
//

#pragma once

#ifndef __AFXWIN_H__
	#error "ФЪ°ьє¬ґЛОДјюЦ®З°°ьє¬Ў°stdafx.hЎ±ТФЙъіЙ PCH ОДјю"
#endif

#include "resource.h"		// Цч·ыєЕ


// CUCRemoteConfigApp
// УР№ШґЛАаКµПЦµДРЕПўЈ¬ЗлІОФД UCRemoteConfig.cpp
//

class CUCRemoteConfigApp : public CWinApp
{
public:
	CUCRemoteConfigApp();

// ЦШРґ
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


#define CONFIG_API extern "C" __declspec(dllexport) 

// pDeviceIP -- Йи±ёIPЈ¬ iPort -- ¶ЛїЪЈ¬ pUser -- УГ»§ГыВрЈ¬ pPassword --ГЬВлЈ¬ pLan -- УпСФЈ¬ pParent -- ёёґ°їЪѕд±ъЈ¬
// ·µ»ШlCOnfigID -- ЕдЦГ±кК¶ЈЁ1-100Ј©
CONFIG_API long CFG_UC_Init(char *pDeviceIP, int iPort, char *pUser, char *pPassword, char *pLan, LPVOID pParent, short iChannel=0);


// lCOnfigID -- ЕдЦГ±кК¶
CONFIG_API long CFG_UC_UnInit(long lConfigID);


// lCOnfigID -- ЕдЦГ±кК¶Ј¬ rcPos -- ПФКѕО»ЦГ
// rcPos(0,0,0,0) №Ш±Хґ°їЪ
CONFIG_API long CFG_UC_ShowModelessDlg(long lConfigID, RECT rcPos);

CONFIG_API long CFG_UC_ShowDlg(LPVOID pParent);
CONFIG_API long CFG_UC_CloseDlg();
