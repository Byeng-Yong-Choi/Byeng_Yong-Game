#pragma once
#include "Cstd.h"
class CBYBitMap
{
public:
	HBITMAP c_HBitmap;			//비트맵 핸들
	BITMAP c_Bitmap;			//비트맵의 폭을 구하기 위한 변수
	HDC MemDC;					//비트맵 이미지의 메모리DC
	RECT BitMapRt;				//비트맵의 위치정보를 전달하기 위한 RECT
	RECT rtdesk;				//이미지의 출력될 좌표와 넓이,폭 정보
	RECT rtsrc;					//비트맵 이미지에서 출력할려는 곳의 좌표
	float weIX;					//무기좌표 X
	float weIY;					//무기좌표 Y

public:
	bool Init();
	bool Frame();
	bool Render();									//기본 이미지 출력
	bool Render(float IX, float IY);				//시간 동기화로 x,y좌표가 실수로 넘어올시 사용하게 될 Render
	bool Release();
	bool Load(const TCHAR* FileName);				//이미지를 로드하여 핸들과 DC에 넣는 함수
	bool weaponRender(float IY);					//무기 발사시 출력되는 렌더로 출력시작된 위치에서 y축으로 위로 올라가기만 한다
	void rect(RECT charRt1, RECT charRt2);			//비트맵 이미지에서 출력범위를 받아내는 함수

public:
	CBYBitMap();
	~CBYBitMap();
};

