#include "C_GameMap.h"

HRESULT C_GameMap::CreateVertexData()
{
	HRESULT hr = S_OK;
	m_VerTex.clear();
	m_obj.m_VertexSize = sizeof(PNCT_VERTEX);

	m_VerTex.resize(4);
	//m_VerTex[0].p = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	//m_VerTex[1].p = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	//m_VerTex[2].p = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	//m_VerTex[3].p = D3DXVECTOR3(1.0f, -1.0f, 0.0f);										//정점 좌표 입력
	m_VerTex[0].p = D3DXVECTOR3(-(Winrt.right / 2.0f), Winrt.bottom / 2.0f, 0.0f);
	m_VerTex[1].p = D3DXVECTOR3(Winrt.right / 2.0f, Winrt.bottom / 2.0f, 0.0f);
	m_VerTex[2].p = D3DXVECTOR3(-(Winrt.right / 2.0f), (-(Winrt.bottom / 2.0f)+(Winrt.bottom/5.0f)), 0.0f);
	m_VerTex[3].p = D3DXVECTOR3(Winrt.right / 2.0f, (-(Winrt.bottom / 2.0f) + (Winrt.bottom / 5.0f)), 0.0f);			//정점 좌표 입력

	m_VerTex[0].c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VerTex[1].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VerTex[2].c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VerTex[3].c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);									//정점 컬러값 입력

	m_VerTex[0].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VerTex[1].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VerTex[2].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VerTex[3].t = D3DXVECTOR2(1.0f, 1.0f);												//정점 텍스쳐 좌표

	return hr;
}

void C_GameMap::SetCbData(D3DXVECTOR3 pos, float scope)					//타워의 스코프 위치를 보여주기위한 함수
{
	m_ConData.ddata.x = pos.x;
	m_ConData.ddata.y = pos.y;
	m_ConData.ddata.z = pos.z;
	m_ConData.ddata.w = scope;

}
bool C_GameMap::Frame()
{
	//m_ConData.bdata.x = 0;
	return true;
}

C_GameMap::C_GameMap()
{
}


C_GameMap::~C_GameMap()
{
}


bool C_MapMgr::Init()
{
	return true;
}

bool C_MapMgr::Frame()
{
	m_Map->Frame();
	return true;
}

bool C_MapMgr::Render()
{
	m_Map->Render();
	return true;
}

bool C_MapMgr::Release()
{
	m_Map->Release();
	return true;
}

HRESULT C_MapMgr::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;

	m_Map = std::make_shared<C_GameMap>();
	m_Map->Create(pd3dDevice, Context, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/map/Level1map.bmp", "VSmat", "PSBlend2");

	return hr;
}

void C_MapMgr::SetCbData(D3DXVECTOR3 pos, float scope)
{
	m_Map->SetCbData(pos, scope);
}

void C_MapMgr::SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	m_Map->SetMatrix(world, view, proj);
}

void C_MapMgr::CreateVertexData()
{
	m_Map->CreateVertexData();
}

void C_MapMgr::UpdateVertexData()
{
	m_Map->UpdateVertexData(m_Map->m_VerTex);
}

C_MapMgr::C_MapMgr()
{

}

C_MapMgr::~C_MapMgr()
{

}