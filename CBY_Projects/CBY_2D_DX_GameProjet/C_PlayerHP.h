#pragma once
#include "CSprite.h"
#include "CShapeObject.h"

#define LIFE 5

class C_PlayerHP
{
private:
	int m_iHPNumber;

public:
	std::vector<std::shared_ptr<CSprite>> m_HPList;
	std::shared_ptr<CShapeObject> m_Lifefont;
	std::vector<RECT> m_RtList;
	std::vector<PNCT_VERTEX> m_vertexdata;
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pContext;
	std::vector<D3DXVECTOR3> m_PosList;
	D3DXMATRIX m_matWorld;

public:
	virtual HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	virtual void SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
	virtual void VertexData();
	virtual void TextSrc();
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	void SetHP(int i);
	int GetHP();
	void GameSet();

public:
	C_PlayerHP();
	virtual ~C_PlayerHP();
};

