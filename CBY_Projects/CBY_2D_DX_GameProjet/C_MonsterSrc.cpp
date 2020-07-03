#include "C_MonsterSrc.h"

void C_MonsterSrc::SetState(int i)
{
	StateNUM = i;
}

int C_MonsterSrc::GetState()
{
	return StateNUM;
}

bool C_MonsterSrc::GetSpliteEnd()
{
	return m_StateList[StateNUM]->End;
}

HRESULT C_MonsterSrc::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;


	return hr;
}

void C_MonsterSrc::SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	m_world = *world;
	m_StateList[StateNUM]->SetMatrix(world, view, proj);

	if (world != nullptr)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(world->_41, world->_42, world->_43);
		RectSet(pos);
	}
}

void C_MonsterSrc::RectSet(D3DXVECTOR3 pos)
{
	m_Center.x = 0 + pos.x;
	m_Center.y = 0 + pos.y;
	m_Center.z = 0 + pos.z;			//센터값에서 이동값 더함

	m_fRect.left = m_Center.x - m_Size.x;
	m_fRect.top = m_Center.y + m_Size.y;
	m_fRect.right = m_Center.x + m_Size.x;
	m_fRect.bottom = m_Center.y - m_Size.y;
}

void C_MonsterSrc::SetAniTime(float Time)
{

}
bool C_MonsterSrc::Init()
{
	return true;
}


bool C_MonsterSrc::Frame()
{
	m_fChangTime += g_SecondTime;
	m_StateList[StateNUM]->CSprite::Update(m_fChangTime);
	return true;
}

bool C_MonsterSrc::Render()
{
	m_StateList[StateNUM]->CSprite::Render();
	return true;
}

HRESULT C_MonsterSrc::CreateVertexData()					//정점 버퍼 데이터 생성
{
	HRESULT hr = S_OK;

	m_obj.m_VertexSize = sizeof(PNCT_VERTEX);

	m_VerTex.resize(4);
	m_VerTex[0].p = D3DXVECTOR3(-29.0f, 29.0f, 0.5f);
	m_VerTex[1].p = D3DXVECTOR3(29.0f, 29.0f, 0.5f);
	m_VerTex[2].p = D3DXVECTOR3(-29.0f, -29.0f, 0.5f);
	m_VerTex[3].p = D3DXVECTOR3(29.0f, -29.0f, 0.5f);				//정점 좌표 입력(직교 투영)	
	
	m_VerTex[0].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VerTex[1].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VerTex[2].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VerTex[3].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	
	m_VerTex[0].c = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);
	m_VerTex[1].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VerTex[2].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VerTex[3].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);			//정점 컬러값 입력
	
	m_VerTex[0].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VerTex[1].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VerTex[2].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VerTex[3].t = D3DXVECTOR2(1.0f, 1.0f);			//정점 텍스쳐 좌표

	m_Center.x = 0;
	m_Center.y = 0;
	m_Center.z = 0;

	m_Size.x = abs((m_VerTex[0].p.x - m_VerTex[1].p.x) / 2);
	m_Size.y = abs((m_VerTex[2].p.y - m_VerTex[0].p.y) / 2);
	m_Size.z = abs((m_VerTex[0].p.z - m_VerTex[1].p.z) / 2);

	m_fRect.left = m_Center.x - m_Size.x;
	m_fRect.top = m_Center.y + m_Size.y;
	m_fRect.right = m_Center.x + m_Size.x;
	m_fRect.bottom = m_Center.y - m_Size.y;

	return hr;
}

void C_MonsterSrc::TextSrc()
{
	


}

void C_MonsterSrc::TextArray(int a)
{

}

void C_MonsterSrc::SetMonsterHP(int i)
{
	m_iHp += i;
}

bool C_MonsterSrc::Release()
{
	for (int i = 0; i < m_StateList.size(); i++)
	{
		m_StateList[i]->CSprite::Release();
	}
	m_StateList.clear();
	m_Weapon.Release();

	return true;
}

C_MonsterSrc::C_MonsterSrc()
{
	D3DXMatrixIdentity(&m_mat);
	D3DXMatrixIdentity(&m_world);
	D3DXMatrixIdentity(&m_view);
	D3DXMatrixIdentity(&m_proj);
	m_fChangTime = 0.0f;
	//m_iRoot = 0;
}


C_MonsterSrc::~C_MonsterSrc()
{
}
