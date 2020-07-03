#pragma once
#include "C_Object.h"

class TowerLose : public C_Object
{
public:
	HDC AlphaDC;
	HBITMAP AlphaBitMap;
	float AlphaNum;
	int Death;
	int ShowTime;

public:
	bool Init();
	bool Frame();
	bool Render();									//기본 이미지 출력
	bool Release();

public:
	TowerLose();
	~TowerLose();
};

