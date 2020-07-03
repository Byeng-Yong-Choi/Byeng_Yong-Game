#include "Effect.h"


bool Effect::GetRECT(vector<RECT>& rtList, float showtime)
{
	EffectTime= showtime / rtList.size();
	m_rtlist = rtList;
	return true;
}

bool Effect::Init()
{
	C_Object::Init();
	C_Actor::Init();
	EffectDC = CreateCompatibleDC(g_ScreenDC);
	h_Effect = CreateCompatibleBitmap(g_ScreenDC, Winrt.right, Winrt.bottom);
	SelectObject(EffectDC, h_Effect);
	return true;
}

bool Effect::Frame()
{
	C_Object::Frame();
	//endeffect = false;
	if (EffectTime != 0&& EffectShow)
	{
		EffectShowTime+= g_SecondTime;
		if (EffectTime >= EffectShowTime)
		{
			C_Object::GetRECT(m_rtlist[splitenum]);
			return true;
		}
		else
		{
			EffectShowTime -= EffectTime;
			splitenum++;
			if (splitenum==m_rtlist.size())
			{
				splitenum = 0;
				//endeffect = true;
				EffectShow = false;
			}
			C_Object::GetRECT(m_rtlist[splitenum]);
			return true;
		}
	}
}

bool Effect::Render()
{
	BitBlt(g_offScreenDC, IX, IY ,rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCAND);
	BitBlt(g_offScreenDC, IX, IY , rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
	BitBlt(g_offScreenDC, IX, IY , rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);

	BitMapRt.right = IX + rtsrc.right;
	BitMapRt.bottom = IY + rtsrc.bottom;
	BitMapRt.left = IX;
	BitMapRt.top = IY;
	return true;
}

bool Effect::Render(LONG width, LONG height)
{
	float width1 = width;
	float height1 = height;
	////BitBlt(EffectDC, 0, 0, Winrt.right, Winrt.bottom, g_offScreenDC, 0, 0, SRCCOPY);
	//StretchBlt(EffectDC, 0, 0, Winrt.right*2, Winrt.bottom*2, g_offScreenDC, 0, 0, Winrt.right, Winrt.bottom, SRCCOPY);
	//BitBlt(EffectDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCAND);
	//BitBlt(EffectDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
	//BitBlt(EffectDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);

	//StretchBlt(g_offScreenDC, IX, IY, rtsrc.right*2, rtsrc.bottom*2, EffectDC, IX, IY, rtsrc.right, rtsrc.bottom, SRCCOPY);
	////StretchBlt(g_offScreenDC, IX, IY, width-20,height-20, EffectDC, IX, IY, rtsrc.right, rtsrc.bottom, SRCCOPY);

	BitBlt(EffectDC, 0, 0, Winrt.right, Winrt.bottom, g_offScreenDC, 0, 0, SRCCOPY);
	
	StretchBlt(EffectDC, IX, IY, width- EFFECT_SIZE, height- EFFECT_SIZE, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCAND);
	StretchBlt(EffectDC, IX, IY, width - EFFECT_SIZE, height - EFFECT_SIZE, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCINVERT);
	StretchBlt(EffectDC, IX, IY, width - EFFECT_SIZE, height - EFFECT_SIZE, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCINVERT);

	StretchBlt(g_offScreenDC, IX, IY, width - EFFECT_SIZE,height - EFFECT_SIZE, EffectDC, IX, IY, width - EFFECT_SIZE, height - EFFECT_SIZE, SRCCOPY);

	BitMapRt.right = IX + rtsrc.right;
	BitMapRt.bottom = IY + rtsrc.bottom;
	BitMapRt.left = IX;
	BitMapRt.top = IY;
	return true;
}

bool Effect::Release()
{
	C_Object::Release();
	C_Actor::Release();
	DeleteObject(h_Effect);
	ReleaseDC(g_hwnd, EffectDC);
	return true;
}

Effect::Effect()
{
	EffectShow = true;
	endeffect = false;
	EffectShowTime = 0;
	EffectTime = 0;
	splitenum = 0;
}


Effect::~Effect()
{
}
