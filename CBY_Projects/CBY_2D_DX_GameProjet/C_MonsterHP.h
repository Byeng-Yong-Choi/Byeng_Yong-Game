#pragma once
#include "C_ShapeMonsetHP.h"

class C_MonsterHP
{
public:
	std::vector<std::shared_ptr<C_ShapeMonsetHP>> m_HPList;
	std::vector<RECT> m_RtList;
	std::vector<PNCT_VERTEX> m_vertexdata;
	std::vector<DWORD> m_IndexData;
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pContext;
	D3DXVECTOR3 m_Pos;
	D3DXMATRIX m_matWorld;

public:
	virtual HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	virtual void SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
	virtual void TextSrc();
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	void SetHPIndex(float first, int i);
	void SetMonsterPos(D3DXVECTOR3 pos);
	void GameSet();

public:
	C_MonsterHP();
	virtual ~C_MonsterHP();
};


