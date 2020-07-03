#include "TowerWeapon.h"

bool TowerWeapon::Frame()
{
	fPOINT Weaponpt = GetPosition();
	float fD = sqrt((monpt.x - Weaponpt.x)*(monpt.x - Weaponpt.x) +
		(monpt.y - Weaponpt.y)*(monpt.y - Weaponpt.y));
	Ditance = fD;

	//SetPosition(Weaponpt.x, Weaponpt.y);
	/*IX += 30*g_SecondTime;
	IY += 30 * g_SecondTime;*/

	if (Buu)
	{
		GetRotationBitMap(mondgr, m_pBitMap->MemDC);
		GetMaskRotationBitMap(mondgr, m_pMaskBitMap->MemDC);
		if (CharTime != 0)
		{

			WeaponShowTime += g_SecondTime;
			if (CharTime >= WeaponShowTime)
			{
				C_Object::GetRECT(m_rtlist[splitenum]);
				return true;
			}
			else
			{
				WeaponShowTime = 0.0f;
				splitenum++;

				if (splitenum == m_rtlist.size())
				{
					splitenum = 0;

				}

				C_Object::GetRECT(m_rtlist[splitenum]);
				return true;
			}
		}
	}
	else
	{
		float fDirX = monpt.x - Weaponpt.x;
		float fDirY = monpt.y - Weaponpt.y;
		fDirX /= fD;
		fDirY /= fD;
		IX = Weaponpt.x + fDirX * (WeaponSpeed * g_SecondTime);
		IY = Weaponpt.y + fDirY * (WeaponSpeed * g_SecondTime);
	}
	
	return true;
}

bool TowerWeapon::Render()
{
	if (Buu)					//부우 공격일때
	{
		if (Revers)
		{
			/*StretchBlt(g_offScreenDC, IX, IY, -Ditance, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCAND);
			StretchBlt(g_offScreenDC, IX, IY, -Ditance, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCINVERT);
			StretchBlt(g_offScreenDC, IX, IY, -Ditance, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCINVERT);*/

			StretchBlt(g_offScreenDC, IX + (DestSize / 2), IY - (DestSize / 2), Ditance, DestSize, MaskDC, 0, 0, DestSize, DestSize, SRCAND);
			StretchBlt(g_offScreenDC, IX + (DestSize / 2), IY - (DestSize / 2), Ditance, DestSize, ColorDC, 0, 0, DestSize, DestSize, SRCINVERT);
			StretchBlt(g_offScreenDC, IX + (DestSize / 2), IY - (DestSize / 2), Ditance, DestSize, MaskDC, 0, 0, DestSize, DestSize, SRCINVERT);

			/*StretchBlt(g_offScreenDC, IX, IY, Ditance, DestSize, MaskDC, 0, 0, DestSize, DestSize, SRCAND);
			StretchBlt(g_offScreenDC, IX, IY, Ditance, DestSize, ColorDC, 0, 0, DestSize, DestSize, SRCINVERT);
			StretchBlt(g_offScreenDC, IX, IY, Ditance, DestSize, MaskDC, 0, 0, DestSize, DestSize, SRCINVERT);*/
			SelectObject(ColorDC, OldBitMap);
			SelectObject(MaskDC, OldMaskBitMap);

			//BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCAND);
			//BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
			//BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
		}
		else
		{
			StretchBlt(g_offScreenDC, IX + (DestSize / 2), IY - (DestSize / 2), -Ditance, DestSize, MaskDC, 0, 0, DestSize, DestSize, SRCAND);
			StretchBlt(g_offScreenDC, IX + (DestSize / 2), IY - (DestSize / 2), -Ditance, DestSize, ColorDC, 0, 0, DestSize, DestSize, SRCINVERT);
			StretchBlt(g_offScreenDC, IX + (DestSize / 2), IY - (DestSize / 2), -Ditance, DestSize, MaskDC, 0, 0, DestSize, DestSize, SRCINVERT);

			/*StretchBlt(g_offScreenDC, IX, IY, -Ditance, DestSize, MaskDC, 0, 0, DestSize, DestSize, SRCAND);
			StretchBlt(g_offScreenDC, IX, IY, -Ditance, DestSize, ColorDC, 0, 0, DestSize, DestSize, SRCINVERT);
			StretchBlt(g_offScreenDC, IX, IY, -Ditance, DestSize, MaskDC, 0, 0, DestSize, DestSize, SRCINVERT);*/
			SelectObject(ColorDC, OldBitMap);
			SelectObject(MaskDC, OldMaskBitMap);

			/*BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCAND);
			BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
			BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);*/
		}
		BitMapRt.right = IX + Ditance;
		BitMapRt.bottom = IY + DestSize;
		BitMapRt.left = IX;
		BitMapRt.top = IY;
	}
	else if (Cold)				//프리저 공격일때
	{

	}
	else
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

		BitMapRt.right = IX + rtsrc.right;
		BitMapRt.bottom = IY + rtsrc.bottom;
		BitMapRt.left = IX;
		BitMapRt.top = IY;
	}
	
	return true;
}

void TowerWeapon::GetMonsterNum(int i)
{
	monsternum = i;
}

void TowerWeapon::GetMonsterPosition(fPOINT mon)
{
	//monpt = mon[monsternum].GetPosition();
	monpt = mon;
}



TowerWeapon::TowerWeapon()
{
	//WeaponGO = true;
	WeaponSpeed = 0;
	Buu = false;
	Cold = false;
	splitenum=0;
	WeaponShowTime=0.0f;
	mondgr = 0.0f;
	Power = 0;
}


TowerWeapon::~TowerWeapon()
{
}
