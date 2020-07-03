#include "Weapon.h"



//bool Weapon::EndY(float Y)
//{
//	IY -= Y;
//	if (IY <= 0)																						//무기가 발사되고 클라이언트 화면 바깥으로 나갈려하면 false를 반환하여 삭제 유도		
//	{
//		return true;
//	}
//	return false;
//}

bool Weapon::Init()
{
	C_Object::Init();
	C_Actor::Init();
	return true;
}

bool Weapon::Frame()
{
	C_Object::Frame();
	if (Input.GetKeyCheck(VK_SPACE))																//SPACE BAR입력시 무기가 발사되게 입력 받는다
	{
		if (Input.KeyData[VK_SPACE] == KEY_PUSH || Input.KeyData[VK_SPACE] == KEY_HOLD)
		{
			if (Input.KeyData[VK_SPACE] == KEY_HOLD)											//KEY_HOLD상태면 계속 발사가 이루워지는게 아닌 500프레임에 한번씩 발사가 되도록 한다
			{
				num += 1;
				if (num == 100)
				{
					num = 0;
					return true;
				}
				else
				{
					return false;
				}
			}
			else if (Input.KeyData[VK_SPACE] == KEY_PUSH)
			{
				num = 0;
				return true;															//푸쉬는 될때마다 발사
			}
		}

	}
	else
		return false;
}

bool Weapon::Render()
{
	BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCAND);
	BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
	BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);

	BitMapRt.right = IX + rtsrc.right;
	BitMapRt.bottom = IY + rtsrc.bottom;
	BitMapRt.left = IX;
	BitMapRt.top = IY;
	return true;
}

bool Weapon::Release()
{
	C_Object::Release();
	C_Actor::Release();
	return true;
}


Weapon::Weapon()
{
	num = 0;
}


Weapon::~Weapon()
{
}
