#include "TowerScope.h"

void TowerScope::ScopeSetPosition(float x, float y)
{
	Pt.x = x;
	Pt.y = y;
}

void TowerScope::SetScorpeScale(float x)
{
	ScorpeScale = x;
}

bool TowerScope::Init()
{
	EllipseDC = CreateCompatibleDC(g_ScreenDC);
	hh= (HBRUSH)CreateSolidBrush(RGB(255, 0, 255));
	aa = CreateCompatibleBitmap(g_ScreenDC, Winrt.right, Winrt.bottom);
	SelectObject(EllipseDC, aa);
	SelectObject(EllipseDC, hh);
	//PatBlt(EllipseDC, 0, 0, Winrt.right, Winrt.bottom, PATCOPY);
	Ellipse(EllipseDC, 0, 0, ScorpeScale, ScorpeScale);
	return true;
}

bool TowerScope::Frame()
{
	PatBlt(EllipseDC, 0, 0, Winrt.right, Winrt.bottom, PATCOPY);
	return true;
}

bool TowerScope::Render()
{
	BitBlt(g_offScreenDC, Pt.x-(ScorpeScale/2), Pt.y- (ScorpeScale / 2), ScorpeScale, ScorpeScale, EllipseDC, 0, 0, SRCINVERT);
	return true;
}

bool TowerScope::Release()
{
	DeleteObject(hh);
	DeleteObject(aa);
	ReleaseDC(g_hwnd, EllipseDC);
	return true;
}

TowerScope::TowerScope()
{
}


TowerScope::~TowerScope()
{
}
