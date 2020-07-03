#include "Monster.h"

bool Monster::Init()
{
	C_Object::Init();
	C_Actor::Init();
	alpha.BlendOp = AC_SRC_OVER;
	alpha.AlphaFormat = AC_SRC_OVER;	//32비트 이미지면 alpha아니면 0
	alpha.BlendFlags=0;				//무조건 0
	alpha.SourceConstantAlpha = 255;		//알파값 투명도 0~255를 넣는다.

	AlphaDC = CreateCompatibleDC(g_ScreenDC);
	AlphaBitMap = CreateCompatibleBitmap(g_ScreenDC, Winrt.right, Winrt.bottom);
	SelectObject(AlphaDC, AlphaBitMap);

	return true;
}

bool Monster::Frame()
{
	C_Object::Frame();
	if (Death == 1)
	{
		C_Object::GetRECT(m_rtlist[5]);
		if (AlphaNum == 0.0f)
		{
			temp = 1;
			return true;
		}
		else
		{
			AlphaNum -= g_SecondTime * 600;
			if (AlphaNum < 0)
			{
				AlphaNum = 0.0f;
			}
			alpha.SourceConstantAlpha = AlphaNum;
		}
	}
	else if (Death == 0)
	{
		if (temp == 1)
		{
			temp = 0;
		}
		AlphaNum += g_SecondTime * 200;
		
		monShowTime += g_SecondTime;
		if (CharTime >= monShowTime)
		{
			C_Object::GetRECT(m_rtlist[splitenum]);
		}
		else
		{
			monShowTime = 0.0f;
			splitenum++;
			if (splitenum == 4)
			{
				splitenum = 0;
			}
			C_Object::GetRECT(m_rtlist[splitenum]);
		}

		if (AlphaNum > 255)
		{
			AlphaNum = 255.0f;
			Death=2;
		}
		alpha.SourceConstantAlpha = AlphaNum;
		
	}
	else if(Death == 2)
	{
		if (CharTime != 0)
		{
			monShowTime += g_SecondTime;
			if (CharTime >= monShowTime)
			{
				C_Object::GetRECT(m_rtlist[splitenum]);
				
			}
			else
			{
				monShowTime = 0.0f;
				splitenum++;
				if (splitenum == 4)
				{
					splitenum = 0;
				}
				C_Object::GetRECT(m_rtlist[splitenum]);
				
			}
		}
	}
	else
	{
		C_Object::GetRECT(m_rtlist[4]);
		HitShowTime += g_SecondTime/5.0f;
		if (CharTime < HitShowTime)
		{
			Death = 2;
			monShowTime = 0.0f;
			HitShowTime = 0.0f;
		}
	}


	fPOINT ftr;
	ftr=C_Object::GetPosition();
	
	/*ftr.left = IX;
	ftr.top = IY;
	ftr.right =(IX - rtsrc.right)+ rtsrc.right;
	ftr.bottom = (IY - rtsrc.right)+rtsrc.bottom;*/

	fsp = I_Col.RecttoSphere(BitMapRt);
	return true;
}

bool Monster::Render()
{
	if (Make == 1)
	{
		if (Death < 2)
		{
			//if (Rotate)
			//{
			//	BitBlt(AlphaDC, 0, 0, Winrt.right, Winrt.bottom, g_offScreenDC, 0, 0, SRCCOPY);
			//	BitBlt(AlphaDC, IX, IY, DestSize, DestSize, MaskDC, 0, 0, SRCAND);
			//	BitBlt(AlphaDC, IX, IY, DestSize, DestSize, ColorDC, 0, 0, SRCINVERT);
			//	BitBlt(AlphaDC, IX, IY, DestSize, DestSize, MaskDC, 0, 0, SRCINVERT);
			//	//StretchBlt(ReversDC,IX, IY, DestSize, DestSize, AlphaDC, IX, IY, DestSize, DestSize,SRCCOPY);
			//	AlphaBlend(g_offScreenDC, IX, IY, DestSize, DestSize, AlphaDC, IX, IY, -DestSize, DestSize, alpha);

			//	SelectObject(ColorDC, OldBitMap);
			//	SelectObject(MaskDC, OldMaskBitMap);
			//}
			//else
			//{
				if (Revers)
				{
					GetReversDC();
					AlphaBlend(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, ReversDC, IX, IY, rtsrc.right, rtsrc.bottom, alpha);

				}
				else
				{
					BitBlt(AlphaDC, 0, 0, Winrt.right, Winrt.bottom, g_offScreenDC, 0, 0, SRCCOPY);
					BitBlt(AlphaDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCAND);
					BitBlt(AlphaDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
					BitBlt(AlphaDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
					AlphaBlend(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, AlphaDC, IX, IY, rtsrc.right, rtsrc.bottom, alpha);
				}
			//}
		
		}
		else
		{
			/*if (Rotate)
			{
				if (Revers)
				{
					StretchBlt(g_offScreenDC, IX + rtsrc.right, IY-15, -DestSize, DestSize, MaskDC, 0, 0, DestSize-5, DestSize-5, SRCAND);
					StretchBlt(g_offScreenDC, IX + rtsrc.right, IY - 15, -DestSize, DestSize, ColorDC, 0, 0, DestSize-5, DestSize-5, SRCINVERT);
					StretchBlt(g_offScreenDC, IX + rtsrc.right, IY - 15, -DestSize, DestSize, MaskDC, 0, 0, DestSize-5, DestSize-5, SRCINVERT);
					SelectObject(ColorDC, OldBitMap);
					SelectObject(MaskDC, OldMaskBitMap);
				}
				else
				{
					BitBlt(g_offScreenDC, IX, IY, DestSize, DestSize, MaskDC, 0, 0, SRCAND);
					BitBlt(g_offScreenDC, IX, IY, DestSize, DestSize, ColorDC, 0, 0, SRCINVERT);
					BitBlt(g_offScreenDC, IX, IY, DestSize, DestSize, MaskDC, 0, 0, SRCINVERT);
					SelectObject(ColorDC, OldBitMap);
					SelectObject(MaskDC, OldMaskBitMap);
				}
			}
			else*/
			//{
				if (Revers)
				{
					StretchBlt(g_offScreenDC, IX + rtsrc.right, IY, -rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCAND);
					StretchBlt(g_offScreenDC, IX + rtsrc.right, IY, -rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCINVERT);
					StretchBlt(g_offScreenDC, IX + rtsrc.right, IY, -rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, rtsrc.right, rtsrc.bottom, SRCINVERT);
				}
				else
				{
					/*GetReversDC();
					BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, ReversDC, IX, IY, SRCCOPY);*/
					BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCAND);
					BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
					BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
				}
				
			//}
		}

		BitMapRt.right = IX + rtsrc.right;
		BitMapRt.bottom = IY + rtsrc.bottom;
		BitMapRt.left = IX;
		BitMapRt.top = IY;
	}
	return true;
}

bool Monster::Release()
{
	C_Object::Release();
	C_Actor::Release();
	DeleteObject(AlphaBitMap);
	ReleaseDC(g_hwnd, AlphaDC);
	return true;
}

Monster::Monster()
{
	Make = 0;
	temp = 0;
	AlphaNum = 0.0f;
	Death = 0;
	monShowTime = 0.0f;
	HitShowTime = 0.0f;
	splitenum = 0;
}


Monster::~Monster()
{
}
