#pragma once
#include "C_Object.h"
class C_Actor:public C_Object
{
public:
	HBITMAP OldBitMap;
	HBITMAP OldMaskBitMap;
	HBITMAP OldColorBitMap;
	HBITMAP ReversBitMap;
	HBRUSH TransBrush;
	HDC TransDC;
	HDC BrushDC;
	HDC MaskDC;
	HDC ColorDC;
	HDC ReversDC;

	
	bool Rotate;
	bool Revers;


public:
	bool SetRECT(RECT rt);
	
	

public:
	fPOINT GetPosition();
	bool SetPosition(float x, float y) override;
	void GetRotationBitMap(float Angle, HDC SrcDC);
	void GetMaskRotationBitMap(float Angle, HDC SrcDC);
	void GetReversDC();
	float Enemy(fPOINT userfp, fPOINT Enemyfp);
	bool SetPosition(fPOINT fpt);

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	C_Actor();
	~C_Actor();
};

