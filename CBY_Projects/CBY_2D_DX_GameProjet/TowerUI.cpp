#include "TowerUI.h"



bool TowerUI::Init()
{
	
	
	return true;
}

bool TowerUI::Frame()
{
	
	/*if (splite)
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

	SetPosition(IX, IY);*/
	
	return true;
}

bool TowerUI::Render()
{
	
	return true;
}

bool TowerUI::Release()
{

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
