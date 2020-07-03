#include "CBYTime.h"

float g_gametime = 0;
int g_FPS = 0;
float g_SecondTime = 0;

bool CBYTime::Init()
{
	NowTime = timeGetTime();
	return true;
}

bool CBYTime::Frame()
{
	DWORD FrameInTime = timeGetTime();
	SecondTime = (float)(FrameInTime - NowTime) / 1000;
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
	g_FPS = CFPS;
	fps++;
	//= GameTime;
	NowTime = timeGetTime();
	return true;
}

bool CBYTime::Render()
{
	return true;
}

bool CBYTime::Release()
{
	return true;
}


CBYTime::CBYTime()
{
	GameTime = 0.0f;
}


CBYTime::~CBYTime()
{
}
