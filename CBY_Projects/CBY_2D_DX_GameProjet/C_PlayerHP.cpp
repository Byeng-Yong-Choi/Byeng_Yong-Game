#include "C_PlayerHP.h"

HRESULT C_PlayerHP::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;

	m_pd3dDevice = pd3dDevice;
	m_pContext = Context;
	m_HPList.resize(LIFE);
	m_PosList.resize(LIFE+1);
	VertexData();
	TextSrc();

	return hr;
}

void C_PlayerHP::SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	for (int i = 0; i < m_HPList.size(); i++)
	{
		m_matWorld._41 = m_PosList[i].x;
		m_matWorld._42 = m_PosList[i].y;
		m_matWorld._43 = m_PosList[i].z;
		m_HPList[i]->SetMatrix(&m_matWorld, view, proj);
	}
	m_matWorld._41 = m_PosList[LIFE].x;
	m_matWorld._42 = m_PosList[LIFE].y;
	m_matWorld._43 = m_PosList[LIFE].z;
	m_Lifefont->SetMatrix(&m_matWorld, view, proj);
}

bool C_PlayerHP::Init()
{
	return true;
}


bool C_PlayerHP::Frame()
{
	for (int i = 0; i < m_PosList.size(); i++)
	{
		m_PosList[i].x = (Winrt.right / 2.2f) - ((Winrt.right / 25.6f)*i);
		m_PosList[i].y = (-Winrt.bottom / 2.0f) + (Winrt.bottom / 6.5f);
		m_PosList[i].z = 1.0f;
	}
	return true;
}

bool C_PlayerHP::Render()
{
	for (int i = 0; i < m_HPList.size(); i++)
	{
		m_HPList[i]->Render();
	}
	m_Lifefont->Render();
	return true;
}

void C_PlayerHP::VertexData()
{
	m_vertexdata.resize(4);

	m_vertexdata[0].p = D3DXVECTOR3(-25.0f, 25.0f, 0.5f);
	m_vertexdata[1].p = D3DXVECTOR3(25.0f, 25.0f, 0.5f);
	m_vertexdata[2].p = D3DXVECTOR3(-25.0f, -25.0f, 0.5f);
	m_vertexdata[3].p = D3DXVECTOR3(25.0f, -25.0f, 0.5f);				//정점 좌표 입력(직교 투영)	

	m_vertexdata[0].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vertexdata[1].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vertexdata[2].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vertexdata[3].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_vertexdata[0].c = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);
	m_vertexdata[1].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_vertexdata[2].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_vertexdata[3].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);			//정점 컬러값 입력

	m_vertexdata[0].t = D3DXVECTOR2(0.0f, 0.0f);
	m_vertexdata[1].t = D3DXVECTOR2(1.0f, 0.0f);
	m_vertexdata[2].t = D3DXVECTOR2(0.0f, 1.0f);
	m_vertexdata[3].t = D3DXVECTOR2(1.0f, 1.0f);			//정점 텍스쳐 좌표
}

void C_PlayerHP::TextSrc()
{
	m_RtList.resize(2);
	m_RtList[0] = { 93,0,53,58 };
	m_RtList[1] = { 93,60,53,58 };

	m_HPList.resize(5);
	for (int i = 0; i < m_HPList.size(); i++)
	{
		m_HPList[i] = std::make_shared<CSprite>();
		m_HPList[i]->Create(m_pd3dDevice, m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/UI/hp.png", "VSmat", "PSBlend2");
		m_HPList[i]->SetRectArray(m_RtList, 245, 368);
		m_HPList[i]->UpdateVertexData(m_vertexdata);
	}

	m_RtList.clear();
	m_RtList.resize(1);
	m_RtList[0] = { 0,89,61,34 };
	m_Lifefont = std::make_shared<CShapeObject>();
	m_Lifefont->Create(m_pd3dDevice, m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/UI/hp.png", "VSmat", "PSBlend2");
	m_Lifefont->UVConvert(m_vertexdata, m_RtList[0], 245, 368);
	m_Lifefont->UpdateVertexData(m_vertexdata);

}

bool C_PlayerHP::Release()
{
	m_RtList.clear();
	m_vertexdata.clear();
	for (int i = 0; i < m_HPList.size(); i++)
	{
		m_HPList[i]->Release();
	}
	m_HPList.clear();
	return true;
}

void C_PlayerHP::SetHP(int i)
{
	m_iHPNumber -= i;
	for (int i = m_iHPNumber; i < m_HPList.size(); i++)
	{
		m_HPList[i]->m_Index=1;
	}
}

int C_PlayerHP::GetHP()
{
	return m_iHPNumber;
}

void C_PlayerHP::GameSet()
{
	for (int i = 0; i < m_HPList.size(); i++)
	{
		m_HPList[i]->m_Index = 0;
	}

	m_iHPNumber = LIFE;
}

C_PlayerHP::C_PlayerHP()
{
	D3DXMatrixIdentity(&m_matWorld);
	m_iHPNumber = LIFE;
}


C_PlayerHP::~C_PlayerHP()
{
}
