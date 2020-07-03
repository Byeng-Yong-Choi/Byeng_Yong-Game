#include "TowerCoice.h"

bool TowerCoice::Init()
{
	C_Object::Init();
	ChoiceDC = CreateCompatibleDC(g_ScreenDC);
	h_Choice = CreateCompatibleBitmap(g_ScreenDC, Winrt.right, Winrt.bottom);
	SelectObject(ChoiceDC, h_Choice);
	return true;
}

bool TowerCoice::Frame()
{
	C_Object::Frame();
	Position=GetPosition();
	if (IX < m_mousepos.x && m_mousepos.x < BitMapRt.right)
	{
		if (IY < m_mousepos.y && m_mousepos.y < BitMapRt.bottom)
		{
			SetCursor(LoadCursor(NULL, IDC_HAND));
			if (Input.KeyCheck(VK_LBUTTON))
			{
				TowerClick = true;
			}
		}
	}
	return true;
}

bool TowerCoice::Render()
{
	/*BitBlt(ChoiceDC, 0, 0, Winrt.right, Winrt.bottom, g_offScreenDC, 0, 0, SRCCOPY);

	StretchBlt(ChoiceDC, IX, IY, rtsrc.right - SIZE, rtsrc.bottom - SIZE, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCAND);
	StretchBlt(ChoiceDC, IX, IY, rtsrc.right - SIZE, rtsrc.bottom - SIZE, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCINVERT);
	StretchBlt(ChoiceDC, IX, IY, rtsrc.right - SIZE, rtsrc.bottom - SIZE, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCINVERT);

	StretchBlt(g_offScreenDC, IX, IY, rtsrc.right - SIZE, rtsrc.bottom - SIZE, ChoiceDC, IX, IY, rtsrc.right - SIZE, rtsrc.bottom - SIZE, SRCCOPY);*/

	BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCAND);
	BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
	BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);


	return true;
}

bool TowerCoice::Release()
{
	C_Object::Release();
	DeleteObject(h_Choice);
	ReleaseDC(g_hwnd, ChoiceDC);
	return true;
}


TowerCoice::TowerCoice()
{
	TowerClick = false;
}


TowerCoice::~TowerCoice()
{
}
