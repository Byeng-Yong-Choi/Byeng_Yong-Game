#pragma once
#include "CCamera.h"
#include "CShapeObject.h"
#include "CBY_DxRT.h"


class CBY_Minimap:public CCamera
{
public:
	std::shared_ptr<CShapeObject> m_pMiniMap;
	std::shared_ptr<CBY_DxRT> m_RT;
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_Context;
	D3DXMATRIX	m_matTopView;
	D3DXMATRIX	m_matTopProj;
	

public:
	HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	virtual void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);

public:
	bool Render();
	bool Release();

public:
	CBY_Minimap();
	virtual ~CBY_Minimap();
};

