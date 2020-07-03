#include "C_Menu.h"

HRESULT C_Menu::CreateVertexData()
{
	HRESULT hr = S_OK;
	m_VerTex.clear();
	m_obj.m_VertexSize = sizeof(PNCT_VERTEX);

	m_VerTex.resize(4);
	//m_VerTex[0].p = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	//m_VerTex[1].p = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	//m_VerTex[2].p = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	//m_VerTex[3].p = D3DXVECTOR3(1.0f, -1.0f, 0.0f);										//정점 좌표 입력
	m_VerTex[0].p = D3DXVECTOR3(-(Winrt.right / 2.0f), (-(Winrt.bottom / 2.0f) + (Winrt.bottom / 5.0f)), 0.0f);
	m_VerTex[1].p = D3DXVECTOR3(Winrt.right / 2.0f, (-(Winrt.bottom / 2.0f) + (Winrt.bottom / 5.0f)), 0.0f);
	m_VerTex[2].p = D3DXVECTOR3(-(Winrt.right / 2.0f), -(Winrt.bottom / 2.0f), 0.0f);
	m_VerTex[3].p = D3DXVECTOR3(Winrt.right / 2.0f, -(Winrt.bottom / 2.0f), 0.0f);			//정점 좌표 입력

	m_VerTex[0].c = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);
	m_VerTex[1].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VerTex[2].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VerTex[3].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);									//정점 컬러값 입력

	m_VerTex[0].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VerTex[1].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VerTex[2].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VerTex[3].t = D3DXVECTOR2(1.0f, 1.0f);												//정점 텍스쳐 좌표

	RECT rt = { 299,6,155,45 };

	UVConvert(m_VerTex, rt, 614, 296);

	return hr;
}

HRESULT C_Menu::UICreate(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;
	Create(pd3dDevice, Context, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/UI/UI.png", "VSmat", "PSBlend2");
	m_HP.Create(pd3dDevice, Context);
	m_UIMoney.Create(pd3dDevice, Context);
	m_Score.Create(pd3dDevice, Context);
	return hr;
}

void C_Menu::UISetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	SetMatrix(world, view, proj);
	m_HP.SetWorldandMatrix(world, view, proj);
	m_UIMoney.SetMatrix(world, view, proj);
	m_Score.SetMatrix(world, view, proj);
}

bool C_Menu::Frame()
{
	m_HP.Frame();
	m_UIMoney.Frame();
	m_Score.Frame();
	return true;
}

bool C_Menu::UIRender()
{
	Render();
	m_HP.Render();
	m_UIMoney.Render();
	m_Score.Render();
	return true;
}

bool C_Menu::UIRelease()
{
	Release();
	m_HP.Release();
	m_UIMoney.Release();
	m_Score.Release();
	return true;
}

void C_Menu::SetHP(int i)
{
	m_HP.SetHP(i);
}

bool C_Menu::PlayerDeath()
{
	if (m_HP.GetHP() <= 0)
	{
		return true;
	}
	return false;
}

void C_Menu::GameSet()
{
	m_HP.GameSet();
	m_Score.GameSet();
}

C_Menu::C_Menu()
{
}


C_Menu::~C_Menu()
{
}
