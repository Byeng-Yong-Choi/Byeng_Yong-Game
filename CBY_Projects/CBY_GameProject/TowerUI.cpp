#include "TowerUI.h"

//bool TowerUI::GetRECT(RECT rt1)
//{
//	rtsrc = rt1;
//	return true;
//}

bool TowerUI::Init()
{
	C_Object::Init();
	
	return true;
}

bool TowerUI::Frame()
{
	C_Object::Frame();
	if (splite)
	{

		GetRECT(m_rtlist[0]);
		if (BitMapRt.left < m_mousepos.x && m_mousepos.x < BitMapRt.right)
		{
			if (BitMapRt.top < m_mousepos.y && m_mousepos.y < BitMapRt.bottom)
			{
				SetCursor(LoadCursor(NULL, IDC_HAND));
				GetRECT(m_rtlist[1]);

				if (Input.KeyCheck(VK_LBUTTON))
				{
					GetRECT(m_rtlist[2]);
					Click = true;
				}
			}
		}
	}
	else
	{
		ClickTime += g_SecondTime;

		if (BitMapRt.left < m_mousepos.x && m_mousepos.x < BitMapRt.right)
		{
			if (BitMapRt.top < m_mousepos.y && m_mousepos.y < BitMapRt.bottom)
			{
				SetCursor(LoadCursor(NULL, IDC_HAND));
				if (Input.KeyCheck(VK_LBUTTON))
				{
					if (Click&&ClickTime >= 0.3f)
					{
						Click = false;
						ClickTime = 0.0f;
					}

					if (ClickTime >= 0.3f)
					{
						Click = true;
						ClickTime = 0.0f;
					}
				}
			}
		}
	}

	SetPosition(IX, IY);
	
	return true;
}

bool TowerUI::Render()
{
	BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCAND);
	BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
	BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
	return true;
}

bool TowerUI::Release()
{
	C_Object::Release();
	return true;
}

TowerUI::TowerUI()
{
	ClickTime = 0.3f;
	splite = false;
	Click = false;
}


TowerUI::~TowerUI()
{
}
