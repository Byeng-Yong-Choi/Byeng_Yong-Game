#include "C_Object.h"


bool C_Object::Load(const TCHAR* FileName, const TCHAR* MaskFileName)
{
	m_pBitMap = (C_BitMapManager::get().Load(FileName));
	if (m_pBitMap != nullptr)
	{
		if (MaskFileName != nullptr)
		{
			m_pMaskBitMap = C_BitMapManager::get().Load(MaskFileName);
		}
	}
	return true;
}

bool C_Object::GetRECT(vector<RECT>& rtList, float showtime)
{
	CharTime = showtime / rtList.size();
	m_rtlist = rtList;
	return true;
}


fPOINT C_Object::GetPoint()
{
	return { IX,IY };
}

bool C_Object::GetRECT(RECT rt1)										//소스의 위치정보 받기
{
	rtsrc = rt1;

	DestSize = sqrt((rt1.right*rt1.right) + (rt1.bottom*rt1.bottom));
	return true;
}

bool C_Object::GetRECT(vector<RECT>& rtList)										//소스의 위치정보 받기
{
	m_rtlist = rtList;

	return true;
}

bool C_Object::SetPosition(float x, float y)												//소스의 데이터 입력
{
	IX = x;
	IY = y;

	BitMapRt.right = IX + rtsrc.right;
	BitMapRt.bottom = IY + rtsrc.bottom;
	BitMapRt.left = IX;
	BitMapRt.top = IY;

	return true;
}

fPOINT C_Object::GetPosition()
{
	BitMapRt.right = IX + rtsrc.right;
	BitMapRt.bottom = IY + rtsrc.bottom;
	BitMapRt.left = IX;
	BitMapRt.top = IY;

	return { IX,IY };
}

bool C_Object::Init()
{
	return true;
}
bool C_Object::Frame()
{
	Input.Frame();
	m_mousepos.x = (float)Input.m_Mouse.x;
	m_mousepos.y = (float)Input.m_Mouse.y;
	return true;
}
bool C_Object::Render()									//기본 이미지 출력
{
	BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCCOPY);
	
	return true;
}
bool C_Object::Release()
{
	return true;
}


C_Object::C_Object()
{
}


C_Object::~C_Object()
{
}
