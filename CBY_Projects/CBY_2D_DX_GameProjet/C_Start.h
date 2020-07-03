#pragma once
#include "CShapeObject.h"

class C_Start:public CShapeObject
{
public:
	std::vector<std::shared_ptr<CShapeObject>> m_Start;
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pContext;
	std::vector<PNCT_VERTEX> m_vertexdata;

public:
	bool Render() override;
	HRESULT VertexData(int i);
	HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	void StartText(int i);
	

public:
	C_Start();
	~C_Start();
};

