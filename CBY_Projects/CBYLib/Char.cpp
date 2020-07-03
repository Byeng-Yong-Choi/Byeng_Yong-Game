#include "Char.h"

bool Char::Init()
{
	C_Object::Init();
	C_Actor::Init();
	
	return true;
}

bool Char::Frame()
{
	C_Object::Frame();

	splitenum++;
			if (splitenum == m_rtlist.size())
			{
				splitenum = 0;
			}
			C_Object::GetRECT(m_rtlist[splitenum]);
		


	if (Input.GetKeyCheck(VK_UP))																//방향키로 캐릭터의 이동을 정의 계산하며 사양의 차이에 불공정함이 생기지 않게 시간의 동기화를 이용
	{
		IY -= CHARSPEED * g_SecondTime;
	}
	if (Input.GetKeyCheck(VK_LEFT))
	{
		IX -= CHARSPEED * g_SecondTime;
	}
	if (Input.GetKeyCheck(VK_RIGHT))
	{
		IX += CHARSPEED * g_SecondTime;
	}
	if (Input.GetKeyCheck(VK_DOWN))
	{
		IY += CHARSPEED * g_SecondTime;
	}

	if (IX <= Winrt.left)		//캐릭터가 화면 밖으로 이동하는것을 제한하는 조건문들이다.
	{
		IX = Winrt.left;
	}
	if (IX >= Winrt.right - rtsrc.right)
	{
		IX = Winrt.right - rtsrc.right;			//IX가 right좌표로 가면 화면 밖으로 나가므로 그 폭만큼 빼서 좌표를 넣으면 더 나가지 않고 딱 맞게 들어간다.
	}
	if (IY <= Winrt.top)
	{
		IY = Winrt.top;
	}
	if (IY >= Winrt.bottom - rtsrc.bottom)
	{
		IY = Winrt.bottom - rtsrc.bottom;
	}

	return true;
}

bool Char::Render()
{
		/*BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCAND);
		BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
		BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);*/
	BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCCOPY);

		BitMapRt.right = IX + rtsrc.right;
		BitMapRt.bottom = IY + rtsrc.bottom;
		BitMapRt.left = IX;
		BitMapRt.top = IY;

	return true;
}

bool Char::Release()
{
	C_Object::Release();
	C_Actor::Release();
	return true;
}

Char::Char()
{
	splitenum = 0;
	CharShowTime=0;
	num = 0;
}


Char::~Char()
{
}
