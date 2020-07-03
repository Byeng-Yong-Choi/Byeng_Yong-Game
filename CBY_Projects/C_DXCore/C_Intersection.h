#pragma once
#include "Cstd.h"

class C_Intersection
{
public:
	fPOINT Center(fRECT rt1);									//RECT�� �޾� �߽����� ���ϴ� �Լ�
	float Distance(fPOINT pt1, fPOINT pt2);						//�� ���� �Ÿ��� ���ϴ� �Լ�
	float GetDistance(float ax, float ay, float bx, float by);		//������ �Ÿ��� ���ϴ� �Լ�
	float GetDistance(D3DXVECTOR3 v1, D3DXVECTOR3 v2);			//�� ������ �Ÿ�
	bool collision(fRECT rt1, fRECT rt2);						//�� RECT�� �浹�ϴ��� Ȯ���ϴ� �Լ�

public:
	fSphere RecttoSphere(fRECT rt1);							//RECT�� ������ �ٲٴ� �Լ�
	bool Spherecollision(fSphere sh1, fSphere sh2);				//�� ���� �浹 Ȯ���Լ�

public:
	float Smoothstep(float edge0, float edge1, float x);
	float Clamp(float x, float lowerlimit, float upperlimit);
	float Lerp(float value1, float value2, float amount);

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
