#pragma once
#include "C_Object.h"


class Health:public C_Object
{
public:
	HDC HealthDC;
	HDC HealthDC_2;
	HBITMAP m_hHealth;
	HBITMAP m_hHealth2;
	HBRUSH HealthColor;
	HBRUSH HealthColor2;
	int Hit;
	int Death;
	int Make;
	float HPNUM;
	float IRight;
	float RedX;

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;									//기본 이미지 출력
	bool Release() override;

public:
	Health();
	~Health();
};

