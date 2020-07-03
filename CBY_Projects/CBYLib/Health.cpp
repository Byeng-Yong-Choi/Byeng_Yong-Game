#include "Health.h"

bool Health::Init()
{
	C_Object::Init();
	HealthDC = CreateCompatibleDC(g_ScreenDC);
	HealthDC_2 = CreateCompatibleDC(g_ScreenDC);
	m_hHealth = CreateCompatibleBitmap(g_ScreenDC, Winrt.right, Winrt.bottom);
	m_hHealth2 = CreateCompatibleBitmap(g_ScreenDC, Winrt.right, Winrt.bottom);
	SelectObject(HealthDC, m_hHealth);
	SelectObject(HealthDC_2, m_hHealth2);
	COLORREF HColor = RGB(255, 0, 0);
	COLORREF HColor2 = RGB(0, 0, 0);
	HealthColor = CreateSolidBrush(HColor);	
	HealthColor2 = CreateSolidBrush(HColor2);
	SelectObject(HealthDC, HealthColor);
	SelectObject(HealthDC_2, HealthColor2);

	PatBlt(HealthDC, 0, 0, rtsrc.right, rtsrc.bottom, PATCOPY);
	PatBlt(HealthDC_2, 0, 0, rtsrc.right, rtsrc.bottom, PATCOPY);

	IRight=IX+rtsrc.right;
	RedX = rtsrc.right/ HPNUM;
	return true;
}

bool Health::Frame()
{
	C_Object::Frame();
	IRight = IX + rtsrc.right;
	if (HPNUM <= Hit)
	{
		Hit = HPNUM;
	}

	return true;
}

bool Health::Render()
{
	if (Make == 1)
	{
		if (Death == 0)
		{
			BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, HealthDC, 0, 0, SRCCOPY);
			StretchBlt(g_offScreenDC, IRight, IY, (-RedX * Hit), rtsrc.bottom, HealthDC_2, 0, 0, rtsrc.right, rtsrc.bottom, SRCCOPY);

			if (Hit == HPNUM)
			{
				BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, HealthDC_2, 0, 0, SRCCOPY);
			}
		}
		else
		{
			return true;
			BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, HealthDC_2, 0, 0, SRCCOPY);
		}
	}
	return true;
}

bool Health::Release()
{
	C_Object::Release();
	DeleteObject(HealthColor);
	ReleaseDC(g_hwnd, HealthDC);
	return true;
}

Health::Health()
{
	Make = 0;
	Hit = 0;
	Death=0;
}


Health::~Health()
{
}
