#pragma once
#include "CSprite.h"
#include "CShapeObject.h"

class C_Score :public CSprite
{
public:
	std::vector<std::shared_ptr<C_Score>> m_Score;
	std::shared_ptr<CShapeObject> m_Scorefont;
	std::vector<PNCT_VERTEX> m_vertexdata;
	std::vector<RECT> m_RtList;
	std::vector<D3DXVECTOR3> m_PosList;
	D3DXMATRIX m_matWorld;

private:
	float m_fScore;

public:
	float m_ChangeTime;
	int m_Spritenum;

public:
	HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
	HRESULT CreateVertexData();
	void FontVertex();
	void Textarray();
	bool Frame();
	bool Render();
	bool Release();
	float GetScore();

public:
	void GameSet();

public:
	C_Score();
	virtual ~C_Score();
};

