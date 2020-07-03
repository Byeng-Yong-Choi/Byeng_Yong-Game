#pragma once
#include "C_BitMap.h"
#include "C_BitMapManager.h"
#include "C_Intersection.h"
#include <map>
#include <list>
#include <vector>

class C_Object
{
public:
	enum { KEY_FREE, KEY_UP, KEY_HOLD, KEY_PUSH };
	std::vector<RECT> m_rtlist;

public:
	fPOINT m_mousepos;
	BLENDFUNCTION alpha;
	C_BitMap* m_pBitMap;
	C_BitMap* m_pMaskBitMap;
	CBY_Input Input;
	fRECT BitMapRt;
	RECT rtsrc;
	float IX;
	float IY;
	float DestSize;
	float CharTime;

public:
	bool Load(const TCHAR* FileName, const TCHAR* MaskFileName=nullptr);
	virtual bool SetPosition(float x, float y);
	virtual fPOINT GetPosition();
	virtual bool GetRECT(RECT rt1);
	virtual bool GetRECT(vector<RECT>& rtList);
	virtual bool GetRECT(vector<RECT>& rtList, float showtime);
	fPOINT GetPoint();
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();									//기본 이미지 출력
	virtual bool Release();

public:
	C_Object();
	virtual ~C_Object();
};

