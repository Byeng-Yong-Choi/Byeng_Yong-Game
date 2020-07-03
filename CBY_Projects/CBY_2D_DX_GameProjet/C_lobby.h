#pragma once
#include "CShapeObject.h"

class C_lobby :public CShapeObject
{
public:
	std::shared_ptr<CShapeObject> m_Lobby;
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pContext;
	std::vector<PNCT_VERTEX> m_vertexdata;

public:
	bool Render() override;
	HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);

public:
	C_lobby();
	~C_lobby();
};

