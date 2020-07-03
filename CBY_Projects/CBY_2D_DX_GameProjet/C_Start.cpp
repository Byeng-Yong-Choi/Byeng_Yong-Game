#include "C_Start.h"

HRESULT C_Start::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;

	m_pd3dDevice = pd3dDevice;
	m_pContext = Context;
	m_Start.resize(2);
	for (int i = 0; i < 2; i++)
	{
		StartText(i);
	}

	return hr;
}

void C_Start::StartText(int i)
{
		m_Start[i] = std::make_shared<CShapeObject>();
		m_Start[i]->Create(m_pd3dDevice, m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/start/start.png", "VSmat", "PS");
		VertexData(i);
		m_Start[i]->UpdateVertexData(m_vertexdata);
	
}

HRESULT C_Start::VertexData(int i)
{
	HRESULT hr = S_OK;
	m_vertexdata.resize(4);
	switch (i)
	{
	case 0:
		m_vertexdata[0].p = D3DXVECTOR3(-640.0f, 360.0f, 0.0f);
		m_vertexdata[1].p = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
		m_vertexdata[2].p = D3DXVECTOR3(-640.0f, 0.0f, 0.0f);
		m_vertexdata[3].p = D3DXVECTOR3(640.0f, 0.0f, 0.0f);
		m_vertexdata[0].t = D3DXVECTOR2(0.0f, 0.0f);
		m_vertexdata[1].t = D3DXVECTOR2(1.0f, 0.0f);
		m_vertexdata[2].t = D3DXVECTOR2(0.0f, 0.5f);
		m_vertexdata[3].t = D3DXVECTOR2(1.0f, 0.5f);
		break;

	case 1:
		m_vertexdata[0].p = D3DXVECTOR3(-640.0f, 0.0f, 0.0f);
		m_vertexdata[1].p = D3DXVECTOR3(640.0f, 0.0f, 0.0f);
		m_vertexdata[2].p = D3DXVECTOR3(-640.0f, -360.0f, 0.0f);
		m_vertexdata[3].p = D3DXVECTOR3(640.0f, -360.0f, 0.0f);
		m_vertexdata[0].t = D3DXVECTOR2(0.0f, 0.5f);
		m_vertexdata[1].t = D3DXVECTOR2(1.0f, 0.5f);
		m_vertexdata[2].t = D3DXVECTOR2(0.0f, 1.0f);
		m_vertexdata[3].t = D3DXVECTOR2(1.0f, 1.0f);
		break;														//정점 좌표 입력
	}		

	return hr;
}

bool C_Start::Render()
{
	for (int i = 0; i < 2; i++)
	{
		m_Start[i]->Render();
	}
	return true;
}

C_Start::C_Start()
{
}


C_Start::~C_Start()
{
}
