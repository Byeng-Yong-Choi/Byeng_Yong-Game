#include "CTime.h"

float g_gametime=0;

float g_SecondTime=0;

bool CTime::Init()
{
	NowTime = timeGetTime();
	return true;
}

bool CTime::Frame()
{
	DWORD FrameInTime= timeGetTime();
	SecondTime = (float)(FrameInTime - NowTime)/1000;
	g_gametime += SecondTime;
	g_SecondTime = SecondTime;
	static int fps;
	TotalFrameTime += SecondTime;
	
	if (TotalFrameTime >= 1.0f)
	{
		CFPS = fps;
		fps = 0;
		TotalFrameTime -= 1.0f;
	}

	fps++;
	 //= GameTime;
	NowTime = timeGetTime();
	return true;
}

bool CTime::Render()
{
	ZeroMemory(Buffer, sizeof(TCHAR) * 256);
	swprintf_s(Buffer, L"\n플레이 시간:%0.0f초,  FPS:%d", g_gametime, CFPS);
	RECT rt;
	GetClientRect(g_hwnd, &rt);
	SetBkMode(g_offScreenDC, TRANSPARENT);
	SetTextColor(g_offScreenDC, RGB(125, 0, 255));
	DrawText(g_offScreenDC, Buffer, -1, &rt, DT_LEFT | DT_VCENTER);
//	OutputDebugString(Buffer);
	return true;
}

bool CTime::Release()
{
	return true;
}


CTime::CTime()
{
	GameTime = 0.0f;
}


CTime::~CTime()
{
}
