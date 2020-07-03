#include "TowerLose.h"

bool TowerLose::Init()
{
	C_Object::Init();
	alpha.BlendOp = AC_SRC_OVER;
	alpha.AlphaFormat = AC_SRC_OVER;	//32비트 이미지면 alpha아니면 0
	alpha.BlendFlags = 0;				//무조건 0
	alpha.SourceConstantAlpha = 0;		//알파값 투명도 0~255를 넣는다.

	AlphaDC = CreateCompatibleDC(g_ScreenDC);
	AlphaBitMap = CreateCompatibleBitmap(g_ScreenDC, Winrt.right, Winrt.bottom);
	SelectObject(AlphaDC, AlphaBitMap);
	return true;
}

bool TowerLose::Frame()
{
	C_Object::Frame();

	if (Death == 0)
	{

		AlphaNum += g_SecondTime*ShowTime;


		if (AlphaNum > 255)
		{
			AlphaNum = 255.0f;
			Death = 2;
		}
		alpha.SourceConstantAlpha = AlphaNum;

	}
	
	return true;
}

bool TowerLose::Render()
{
	if (Death == 0)
	{
		BitBlt(AlphaDC, 0, 0, Winrt.right, Winrt.bottom, g_offScreenDC, 0, 0, SRCCOPY);
		BitBlt(AlphaDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCAND);
		BitBlt(AlphaDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
		BitBlt(AlphaDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
		AlphaBlend(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, AlphaDC, IX, IY, rtsrc.right, rtsrc.bottom, alpha);
	}
	else
	{
		BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCAND);
		BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
		BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
	}
	return true;
}

bool TowerLose::Release()
{
	C_Object::Release();
	return true;
}


TowerLose::TowerLose()
{
	AlphaNum = 0.0f;
	ShowTime = 0.0f;
	Death = 0;
}


TowerLose::~TowerLose()
{
}
