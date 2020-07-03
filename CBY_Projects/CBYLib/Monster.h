#pragma once
#include "C_Actor.h"
class Monster :public C_Actor
{

public:
	HDC AlphaDC;
	HBITMAP AlphaBitMap;
	float AlphaNum;
	fSphere fsp;
	int Make;
	int Death;
	int temp;
	float monShowTime;
	float HitShowTime;
	int splitenum;

public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;									//기본 이미지 출력
	virtual bool Release() override;

public:
	Monster();
	~Monster();
};

