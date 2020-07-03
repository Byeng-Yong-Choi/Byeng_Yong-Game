#include "C_UIMoney.h"
#include "C_GameStd.h"
#include "C_MoneyMgr.h"

HRESULT C_UIMoney::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;

	VertexData();
	m_Money.resize(4);
	TextArray();
	for (int i = 0; i < m_Money.size(); i++)
	{
		m_Money[i] = std::make_shared<CSprite>();
		m_Money[i]->Create(pd3dDevice, Context, L"../../data/shader/ColorBlend.txt", nullptr, "VSmat", "PS");
		m_Money[i]->UpdateVertexData(m_vertexdata);
		m_Money[i]->SetTextureArray(m_str);
	}
	m_Moneyfont = std::make_shared<CShapeObject>();
	m_Moneyfont->Create(pd3dDevice, Context, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/UI/money.png", "VSmat", "PSBlend2");
	RECT rt = {419,54,35,35};
	m_Moneyfont->UVConvert(m_vertexdata, rt, 984, 312);
	m_Moneyfont->UpdateVertexData(m_vertexdata);

	return hr;
}

void C_UIMoney::SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	for (int i = 0; i < m_Money.size(); i++)
	{
		m_Money[i]->SetMatrix(nullptr, view, proj);
	}

	m_Moneyfont->SetMatrix(nullptr, view, proj);
}

void C_UIMoney::TextArray()
{
	m_str.clear();
	int index = 0;
	m_str.resize(10);
	m_str[index++] = L"../../data/2DGame/UI/time_40-0.png";
	m_str[index++] = L"../../data/2DGame/UI/time_40-1.png";
	m_str[index++] = L"../../data/2DGame/UI/time_40-2.png";
	m_str[index++] = L"../../data/2DGame/UI/time_40-3.png";
	m_str[index++] = L"../../data/2DGame/UI/time_40-4.png";
	m_str[index++] = L"../../data/2DGame/UI/time_40-5.png";
	m_str[index++] = L"../../data/2DGame/UI/time_40-6.png";
	m_str[index++] = L"../../data/2DGame/UI/time_40-7.png";
	m_str[index++] = L"../../data/2DGame/UI/time_40-8.png";
	m_str[index++] = L"../../data/2DGame/UI/time_40-9.png";
}


void C_UIMoney::VertexData()
{
	m_vertexdata.resize(4);

	m_vertexdata[0].p = D3DXVECTOR3(-15.0f, 15.0f, 0.5f);
	m_vertexdata[1].p = D3DXVECTOR3(15.0f, 15.0f, 0.5f);
	m_vertexdata[2].p = D3DXVECTOR3(-15.0f, -15.0f, 0.5f);
	m_vertexdata[3].p = D3DXVECTOR3(15.0f, -15.0f, 0.5f);				//정점 좌표 입력(직교 투영)	

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

bool C_UIMoney::Frame()
{
	int index, a, money;
	money = I_MONEY.GetMoney();

	index = money / 1000;
	a = money % 1000;
	m_Money[0]->m_Index = index;

	index = a / 100;
	a %= 100;
	m_Money[1]->m_Index = index;

	index = a / 10;
	a %= 10;
	m_Money[2]->m_Index = index;
	m_Money[3]->m_Index = a;

	m_Moneyfont->m_matWorld._41 = -(Winrt.right / 2.2f) - ((Winrt.right / 70.6f));
	m_Moneyfont->m_matWorld._42 = (-Winrt.bottom / 2.0f) + (Winrt.bottom / 6.5f);
	m_Moneyfont->m_matWorld._43 = 1.0f;
	for (int i = 0; i < m_Money.size(); i++)
	{
		m_Money[i]->m_matWorld._41 = -(Winrt.right / 2.25f) + ((Winrt.right / 70.6f)*i);
		m_Money[i]->m_matWorld._42 = (-Winrt.bottom / 2.0f) + (Winrt.bottom / 6.5f);
		m_Money[i]->m_matWorld._43 = 1.0f;
	}
	return true;
}

bool C_UIMoney::Render()
{
	for (int i = 0; i < m_Money.size(); i++)
	{
		m_Money[i]->Render();
	}
	m_Moneyfont->Render();
	return true;
}

bool C_UIMoney::Release()
{
	for (int i = 0; i < m_Money.size(); i++)
	{
		m_Money[i]->Release();
	}
	m_Moneyfont->Release();
	return true;
}

void C_UIMoney::GameSet()
{
	for (int i = 0; i < m_Money.size(); i++)
	{
		m_Money[i]->m_Index = 0;
	}
}

C_UIMoney::C_UIMoney()
{
}


C_UIMoney::~C_UIMoney()
{
}
