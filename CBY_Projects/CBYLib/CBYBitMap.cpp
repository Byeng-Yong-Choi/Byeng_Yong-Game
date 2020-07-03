#include "CBYBitMap.h"

bool CBYBitMap::Load(const TCHAR* FileName)
{
	c_HBitmap = (HBITMAP)LoadImage(g_hInstance, FileName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);		//비트맵 이미지를비트맵핸들상태로 받아서 핸들에 전달한다
	MemDC = CreateCompatibleDC(g_ScreenDC);																				//윈도우에 출력하기 위해 ScreenDC와 호환되는 메모리DC인 MemDC를 만든다
	SelectObject(MemDC, c_HBitmap);																						//메모리DC에 비트맵 핸들을 SelectObject해서 정보를 넣는다

	//GetObject(c_HBitmap, sizeof(BITMAP), &c_Bitmap);			//비트맵의 정보를 받아낸다

	return true;
}

void CBYBitMap::rect(RECT charRt1,RECT charRt2)																			//비트맵 이미지에서 출력범위를 받아내는 함수
{
	rtdesk = charRt1;
	rtsrc = charRt2;
}

bool CBYBitMap::Init()
{
	return true;
}
bool CBYBitMap::Frame()
{
	return true;
}

bool CBYBitMap::weaponRender(float XY)																	//무기 발사시 출력되는 렌더로 출력시작된 위치에서 y축으로 위로 올라가기만 한다
{
	weIY -= XY;
	if (weIY <= 0)																						//무기가 발사되고 클라이언트 화면 바깥으로 나갈려하면 false를 반환하여 삭제 유도		
	{
		return false;
	}
	//BitBlt(g_offScreenDC, weIX, weIY, rtdesk.right, rtdesk.bottom, MemDC, rtsrc.left, rtsrc.top, SRCCOPY);
	TransparentBlt(g_offScreenDC, weIX, weIY, rtdesk.right, rtdesk.bottom, MemDC, rtsrc.left, rtsrc.top, rtdesk.right, rtdesk.bottom, RGB(255, 255, 255));
	BitMapRt.right = weIX + rtdesk.right;
	BitMapRt.bottom = weIY + rtdesk.bottom;
	BitMapRt.left = weIX;
	BitMapRt.top = weIY;
	return true;
}


bool CBYBitMap::Render()																				//기본적인 비트맵 출력으로 원래는 rect함수로 정보를 받고 출력
{																														
	//BitBlt(g_offScreenDC, rtdesk.left, rtdesk.top, rtdesk.right, rtdesk.bottom, MemDC, rtsrc.left, rtsrc.top, SRCCOPY);
	TransparentBlt(g_offScreenDC, rtdesk.left, rtdesk.top, rtdesk.right, rtdesk.bottom, MemDC, rtsrc.left, rtsrc.top, rtdesk.right, rtdesk.bottom, RGB(255, 255, 255));
	BitMapRt.right = rtdesk.left+rtdesk.right;
	BitMapRt.bottom = rtdesk.top+rtdesk.bottom;
	BitMapRt.left = rtdesk.left;
	BitMapRt.top = rtdesk.top;
	return true;
}

bool CBYBitMap::Render(float IX, float IY)																				//기본적인 비트맵 출력으로 원래는 rect함수로 위치만 받아내서 출력하기 편하나
{																														//시간의 동기화로 x,y좌표가 실수로 들어오는 경우를 위해 생성
	//BitBlt(g_offScreenDC, IX, IY, rtdesk.right, rtdesk.bottom, MemDC, rtsrc.left, rtsrc.top, SRCCOPY);
	TransparentBlt(g_offScreenDC, IX, IY, rtdesk.right, rtdesk.bottom, MemDC, rtsrc.left, rtsrc.top, rtdesk.right, rtdesk.bottom, RGB(255, 255, 255));
	BitMapRt.right = IX+rtdesk.right;
	BitMapRt.bottom= IY+rtdesk.bottom;
	BitMapRt.left = IX;
	BitMapRt.top = IY;
	return true;
}

bool CBYBitMap::Release()
{
	DeleteObject(c_HBitmap);
	ReleaseDC(g_hwnd,MemDC);
	return true;
}

CBYBitMap::CBYBitMap()
{
}


CBYBitMap::~CBYBitMap()
{
}
