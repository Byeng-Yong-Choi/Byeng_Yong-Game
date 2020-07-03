#pragma once
#include "CShapeBox.h"
class CBY_BoneBox : public CShapeBox
{
public:
	D3DXVECTOR3 m_vMax;
	D3DXVECTOR3 m_vMin;

public:
	HRESULT CreateVertexData();
	HRESULT CreateIndexData();
	void SetBox(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax);

public:
	CBY_BoneBox();
	virtual ~CBY_BoneBox();
};

