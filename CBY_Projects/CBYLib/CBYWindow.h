#pragma once
#include "Cstd.h"

class CBYWindow
{
public:
	WNDCLASSEX wn;				//윈도우 클래스 확장버전을 이용할 변수
	HWND C_hwnd;				//생성된 윈도우의 핸들(현재 한개의 윈도우만 이용하므로 전체에서 사용될 한개의 핸들만 선언)
	HINSTANCE hInstance;		//윈도우에 사용될 인스턴스 윈도우의 고유넘버

public:
	bool InitWindow(const TCHAR* WindowClass, const TCHAR* WindowName);
	virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	


public:
	CBYWindow();
	virtual ~CBYWindow();
};

