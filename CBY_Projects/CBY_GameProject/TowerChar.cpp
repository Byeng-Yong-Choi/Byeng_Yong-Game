#include "TowerChar.h"

void TowerChar::GetSphere(float x, float y,float i)
{
	
	fRECT frt;
	
	frt.left = x - (i / 2.0f);
	frt.top = y - (i / 2.0f);
	frt.right = frt.left + i;
	frt.bottom = frt.top + i;
	
	charfsp=I_Col.RecttoSphere(frt);
}

bool TowerChar::GetRECT(vector<RECT>& rtList, float showtime)
{
	CharTime = showtime / rtList.size();
	m_rtlist = rtList;
	return true;
}

bool TowerChar::Init()
{
	C_Object::Init();
	C_Actor::Init();
	return true;
}

bool TowerChar::Frame()
{
	C_Object::Frame();

	if (LevelUP) 
	{
		switch (state)
		{
		case STATE_NOMAL:
			C_Object::GetRECT(m_rtlist[0]);
			break;

		case STATE_ATTACKER:
			if (CharTime != 0)
			{
				
				WeaponGo = false;
				CharShowTime += g_SecondTime;
				if (CharTime >= CharShowTime)
				{
					C_Object::GetRECT(m_rtlist[splitenum]);
					return true;
				}
				else
				{
					CharShowTime = 0.0f;
					splitenum++;

					if (splitenum == m_rtlist.size())
					{
						splitenum = 1;
						WeaponGo = true;
					}
					
					if (splitenum == 3)					//손오공 타워 조건
					{
						WeaponGo = true;
					}

					C_Object::GetRECT(m_rtlist[splitenum]);
					return true;
				}
			}
			break;
		}
	}

	else 
	{
		switch (state)
		{
		case STATE_NOMAL:
			C_Object::GetRECT(m_rtlist[0]);
			break;

		case STATE_ATTACKER:
			if (CharTime != 0)
			{
				WeaponGo = false;
				CharShowTime += g_SecondTime;
				if (CharTime >= CharShowTime)
				{
					C_Object::GetRECT(m_rtlist[splitenum]);
					return true;
				}
				else
				{
					CharShowTime = 0.0f;
					splitenum++;
					if (splitenum == m_rtlist.size())
					{
						WeaponGo = true;
						splitenum = 1;
					}
					C_Object::GetRECT(m_rtlist[splitenum]);
					return true;
				}
			}
			break;
		}
	}
	return true;
}

bool TowerChar::Render()
{
	if (Revers)
	{
		StretchBlt(g_offScreenDC, IX + rtsrc.right, IY, -rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCAND);
		StretchBlt(g_offScreenDC, IX + rtsrc.right, IY, -rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCINVERT);
		StretchBlt(g_offScreenDC, IX + rtsrc.right, IY, -rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCINVERT);
	}
	else
	{
		BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCAND);
		BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
		BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
	}

	return true;
}

bool TowerChar::Release()
{
	C_Object::Release();
	C_Actor::Release();
	return true;
}

TowerChar::TowerChar()
{
	WeaponGo = false;
	state = 0;
	splitenum = 1;
	CharShowTime = 0.0f;
	LevelUP=false;
	power = 0;
	price = 0;
}


TowerChar::~TowerChar()
{
}
