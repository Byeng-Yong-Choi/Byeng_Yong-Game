#pragma once
#include "CBYWindow.h"
#include "Cstd.h"

class CBYCore:public CBYWindow
{
protected:
	bool GetGameRun;
	CTime time;
	CBY_Input Input;
	C_BitMap Bitmap;
	HDC ScreenDC;
	HDC offScreenDC;
	HBITMAP offScreenDCBitMap;
	HBRUSH BackColor;

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	bool CoreInit();
	bool CoreFrame();
	bool PreRender();
	bool CoreRender();
	bool PostRender();
	bool CoreRelease();
	bool GameRun();

public:
	CBYCore();
	virtual ~CBYCore();
};
