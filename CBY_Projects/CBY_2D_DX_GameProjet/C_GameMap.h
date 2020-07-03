#pragma once
#include "CShapeMap.h"

class C_GameMap:public CShapeMap
{
public:
	HRESULT CreateVertexData() override;
	void SetCbData(D3DXVECTOR3 pos, float scope);
	bool Frame() override;

public:
	C_GameMap();
	~C_GameMap();
};

class C_MapMgr :public Singleton<C_MapMgr>
{
public:
	std::shared_ptr<C_GameMap> m_Map;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	void SetCbData(D3DXVECTOR3 pos, float scope);
	void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
	void CreateVertexData();
	void UpdateVertexData();

public:
	HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
public:
	C_MapMgr();
	~C_MapMgr();
};

#define I_MAP C_MapMgr::GetInstance()
