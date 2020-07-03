#include "C_Actor.h"








bool C_Actor::SetPosition(float x, float y)												//소스의 데이터 입력
{
	IX = x-(rtsrc.right/2);
	if (IX < 0)
	{
		IX = 0;
	}
	else if (IX > Winrt.right)
	{
		IX = Winrt.right - rtsrc.right;
	}
	IY = y-(rtsrc.bottom/2);
	if (IY < 0)
	{
		IY = 0;
	}
	else if (IY > Winrt.bottom)
	{
		IY = Winrt.bottom - rtsrc.bottom;
	}

	BitMapRt.right = IX + rtsrc.right;
	BitMapRt.bottom = IY + rtsrc.bottom;
	BitMapRt.left = IX;
	BitMapRt.top = IY;
	return true;
}

bool C_Actor::SetPosition(fPOINT fpt)												//소스의 데이터 입력
{
	IX = fpt.x - (rtsrc.right / 2);
	if (IX < 0)
	{
		IX = 0;
	}
	else if (IX > Winrt.right)
	{
		IX = Winrt.right - rtsrc.right;
	}
	IY = fpt.y - (rtsrc.bottom / 2);
	if (IY < 0)
	{
		IY = 0;
	}
	else if (IY > Winrt.bottom)
	{
		IY = Winrt.bottom - rtsrc.bottom;
	}

	BitMapRt.right = IX + rtsrc.right;
	BitMapRt.bottom = IY + rtsrc.bottom;
	BitMapRt.left = IX;
	BitMapRt.top = IY;
	return true;
}

fPOINT C_Actor::GetPosition()												//소스의 데이터 입력
{
	return { IX,IY };
}

float C_Actor::Enemy(fPOINT userfp, fPOINT Enemyfp)
{

	float distance = I_Col.Distance(userfp, Enemyfp);
	float a = userfp.x - Enemyfp.x;
	float degrees= acos(a/distance) * 180 / 3.141592;
	
	if (degrees < 0)
	{
		degrees += 360;
	}

	if (90< degrees&& 270>degrees)
	{
		Revers = false;
	}
	else
	{
		Revers = true;
	}
	return degrees;
}


void C_Actor::GetReversDC()
{
	BitBlt(ReversDC, 0, 0, Winrt.right, Winrt.bottom, g_offScreenDC, 0, 0, SRCCOPY);
	StretchBlt(ReversDC, IX + rtsrc.right, IY, -rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCAND);
	StretchBlt(ReversDC, IX + rtsrc.right, IY, -rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCINVERT);
	StretchBlt(ReversDC, IX + rtsrc.right, IY, -rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCINVERT);
}

bool C_Actor::Init()
{
	TransDC = CreateCompatibleDC(g_ScreenDC);
	BrushDC = CreateCompatibleDC(g_ScreenDC);
	MaskDC = CreateCompatibleDC(g_ScreenDC);
	ColorDC = CreateCompatibleDC(g_ScreenDC);
	ReversDC = CreateCompatibleDC(g_ScreenDC);
	ReversBitMap = CreateCompatibleBitmap(g_ScreenDC, Winrt.right, Winrt.bottom);
	SelectObject(ReversDC, ReversBitMap);
	COLORREF HColor = RGB(255, 255, 255);
	TransBrush = CreateSolidBrush(HColor);
	return true;
}

bool C_Actor::Frame()
{
	return true;
}

bool C_Actor::Render()
{
	return true;
}

bool C_Actor::Release()
{
	DeleteObject(OldBitMap);
	DeleteObject(ReversBitMap);
	DeleteObject(TransBrush);
	DeleteDC(BrushDC);
	DeleteDC(TransDC);
	DeleteDC(ReversDC);
	DeleteDC(MaskDC);
	DeleteDC(ColorDC);
	return true;
}

void C_Actor::GetRotationBitMap(float Angle, HDC SrcDC)
{
	HBITMAP Rotat = CreateCompatibleBitmap(g_ScreenDC, DestSize, DestSize);
	OldBitMap = (HBITMAP)SelectObject(TransDC, Rotat);
	SelectObject(BrushDC, TransBrush);
	PatBlt(TransDC, 0, 0, DestSize, DestSize, PATCOPY);
	int Old = SetGraphicsMode(TransDC, GM_ADVANCED);
	XFORM xForm;
	float radian = Angle * (3.141592 / 180);
	xForm.eM11 = cos(radian);
	xForm.eM12 = sin(radian);
	xForm.eM21 = -sin(radian);
	xForm.eM22 = cos(radian);
	xForm.eDx = DestSize / 2;
	xForm.eDy = DestSize / 2;

	SetWorldTransform(TransDC, &xForm);
	//BitBlt(TransDC, -(rtsrc.right / 2), -(rtsrc.bottom / 2), rtsrc.right, rtsrc.bottom, SrcDC, rtsrc.left, rtsrc.top, SRCCOPY);
	BitBlt(TransDC, 0, 0, rtsrc.right, rtsrc.bottom, SrcDC, rtsrc.left, rtsrc.top, SRCCOPY);
	xForm.eM11 = 1;	xForm.eM12 = 0;
	xForm.eM21 = 0;	xForm.eM22 = 1;
	xForm.eDx = 0;
	xForm.eDy = 0;
	SetWorldTransform(TransDC, &xForm);

	SetGraphicsMode(TransDC, Old);
	SelectObject(TransDC, OldBitMap);
	OldBitMap = (HBITMAP)SelectObject(ColorDC, Rotat);
	DeleteObject(Rotat);
	Rotate = true;
}

void C_Actor::GetMaskRotationBitMap(float Angle, HDC SrcDC)
{
	HBITMAP Rotat = CreateCompatibleBitmap(g_ScreenDC, DestSize, DestSize);
	OldBitMap = (HBITMAP)SelectObject(TransDC, Rotat);
	SelectObject(BrushDC, TransBrush);
	PatBlt(TransDC, 0, 0, DestSize, DestSize, PATCOPY);
	int Old = SetGraphicsMode(TransDC, GM_ADVANCED);
	XFORM xForm;
	float radian = Angle * (3.141592 / 180);
	xForm.eM11 = cos(radian);
	xForm.eM12 = sin(radian);
	xForm.eM21 = -sin(radian);
	xForm.eM22 = cos(radian);
	xForm.eDx = DestSize / 2;
	xForm.eDy = DestSize / 2;

	SetWorldTransform(TransDC, &xForm);
	//BitBlt(TransDC, -(rtsrc.right / 2), -(rtsrc.bottom / 2), rtsrc.right, rtsrc.bottom, SrcDC, rtsrc.left, rtsrc.top, SRCCOPY);
	BitBlt(TransDC, 0, 0, rtsrc.right, rtsrc.bottom, SrcDC, rtsrc.left, rtsrc.top, SRCCOPY);
	xForm.eM11 = 1;	xForm.eM12 = 0;
	xForm.eM21 = 0;	xForm.eM22 = 1;
	xForm.eDx = 0;
	xForm.eDy = 0;
	SetWorldTransform(TransDC, &xForm);

	SetGraphicsMode(TransDC, Old);
	SelectObject(TransDC, OldBitMap);
	OldMaskBitMap = (HBITMAP) SelectObject(MaskDC, Rotat);
	DeleteObject(Rotat);
	Rotate = true;
}

C_Actor::C_Actor()
{
	Rotate = false;
	Revers = false;
}


C_Actor::~C_Actor()
{
}
