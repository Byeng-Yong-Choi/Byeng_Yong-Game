#pragma once
#include "C_Actor.h"
#define CHARSPEED 300

class Char:public C_Actor
{
public:
	HBITMAP RotatBit;
	HBITMAP MaskRotatBit;
	int CharShowTime;
	int splitenum;
	int num;
	float EX;
	float EY;

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;									//기본 이미지 출력
	bool Release() override;

public:
	Char();
	~Char();
};

