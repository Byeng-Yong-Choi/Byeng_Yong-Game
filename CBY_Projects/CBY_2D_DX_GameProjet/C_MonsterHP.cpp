#include "C_MonsterHP.h"

HRESULT C_MonsterHP::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;

	m_pd3dDevice = pd3dDevice;
	m_pContext = Context;
	TextSrc();

	return hr;
}

void C_MonsterHP::SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	
	m_matWorld._41 = m_Pos.x;
	m_matWorld._42 = m_Pos.y;
	m_matWorld._43 = m_Pos.z;
	for (int i = 0; i < m_HPList.size(); i++)
	{
		m_HPList[i]->SetMatrix(&m_matWorld, view, proj);
	}
}

bool C_MonsterHP::Init()
{
	return true;
}


bool C_MonsterHP::Frame()
{
	return true;
}

bool C_MonsterHP::Render()
{
	for (int i = 1; i >= 0; i--)
	{
		m_HPList[i]->Render();
	}
	
	return true;
}


void C_MonsterHP::TextSrc()
{
	m_HPList.resize(2);

	for (int i = 0; i < m_HPList.size(); i++)
	{
		m_HPList[i] = std::make_shared<C_ShapeMonsetHP>();
		if (i == 0)
		{
			m_HPList[i]->Create(m_pd3dDevice, m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/UI/hpred_1.png", "VSmat", "PSBlend2");
		}
		else
		{
			m_HPList[i]->Create(m_pd3dDevice, m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/UI/hpblack_1.png", "VSmat", "PSBlend2");
		}
	}

}

bool C_MonsterHP::Release()
{
	m_vertexdata.clear();
	m_IndexData.clear();
	m_HPList[0]->Release();
	m_HPList[1]->Release();
	m_HPList.clear();
	return true;
}

void C_MonsterHP::SetHPIndex(float first, int i)
{
	float hp =  i / first;
	if (hp == 1)
	{
		m_HPList[0]->m_obj.m_StartIndex = 0;
		return;
	}

	for (int i = 0; i < 10; i++)
	{
		if (hp >= (0.9 - (i * 0.1f)))
		{
			m_HPList[0]->m_obj.m_StartIndex = 6 * (i+1);
			if (i == 9)
			{
				m_HPList[0]->m_obj.m_StartIndex = 59;
			}
			return;
		}
	}

}

void C_MonsterHP::SetMonsterPos(D3DXVECTOR3 pos)
{
	m_Pos = pos;
	m_Pos.y += 30.0f;
}

void C_MonsterHP::GameSet()
{
	m_HPList[0]->m_obj.m_StartIndex = 0;
}

C_MonsterHP::C_MonsterHP()
{
	D3DXMatrixIdentity(&m_matWorld);
}


C_MonsterHP::~C_MonsterHP()
{
}

