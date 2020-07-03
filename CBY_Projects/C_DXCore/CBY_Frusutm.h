#pragma once
#include "CShapeBox.h"

class CBY_Frusutm
{
public:
	D3DXVECTOR3 m_vFrustum[8];
	CBY_Plane m_Plane[6];					//���������� 8���� ���� 6���� ����� ������ (�ڽ����� ������ �ݿ��� ���Ѱ��̴� �ڽ��� �����ϸ� ���ϴ�)

public:
	void SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);

public:
	CBY_Frusutm();
	virtual ~CBY_Frusutm();
};

