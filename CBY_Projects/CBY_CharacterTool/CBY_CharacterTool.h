
// CBY_CharacterTool.h: CBY_CharacterTool 응용 프로그램의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
#include "CBY_ToolMain.h"


// CCBYCharacterToolApp:
// 이 클래스의 구현에 대해서는 CBY_CharacterTool.cpp을(를) 참조하세요.
//

class CCBYCharacterToolApp : public CWinAppEx
{
public:
	CCBYCharacterToolApp() noexcept;
	CBY_ToolMain m_main;

// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	BOOL OnIdle(LONG lCount);
};

extern CCBYCharacterToolApp theApp;
