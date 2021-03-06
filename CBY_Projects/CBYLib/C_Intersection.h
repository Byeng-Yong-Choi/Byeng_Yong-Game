#pragma once
#include "Cstd.h"

class C_Intersection
{
public:
	fPOINT Center(fRECT rt1);									//RECT를 받아 중심점을 구하는 함수
	float Distance(fPOINT pt1, fPOINT pt2);						//두 점의 거리를 구하는 함수
	float GetDistance(float ax, float ay, float bx, float by);		//두점의 거리를 구하는 함수
	bool collision(fRECT rt1, fRECT rt2);						//두 RECT가 충돌하는지 확인하는 함수

public:
	fSphere RecttoSphere(fRECT rt1);							//RECT를 원으로 바꾸는 함수
	bool Spherecollision(fSphere sh1, fSphere sh2);				//두 원의 충돌 확인함수

public:
	static C_Intersection& Get()
	{
		static C_Intersection mgr;
		return mgr;
	}

public:
	C_Intersection();
	~C_Intersection();
};

#define I_Col C_Intersection::Get()
