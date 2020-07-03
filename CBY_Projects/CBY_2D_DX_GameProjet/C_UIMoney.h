#pragma once
#include "CSprite.h"
#include "CShapeObject.h"

class C_UIMoney
{
public:
	std::vector<std::shared_ptr<CSprite>> m_Money;
	std::shared_ptr<CShapeObject> m_Moneyfont;
	T_STR_VECTOR m_str;
	std::vector<PNCT_VERTEX> m_vertexdata;

public:
	float m_ChangeTime;
	int m_Spritenum;

public:
	HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
	void VertexData();
	void TextArray();
	bool Frame();
	bool Render();
	bool Release();

public:
	void GameSet();

public:
	C_UIMoney();
	virtual ~C_UIMoney();
};

