#include "TowerSrc.h"
#include "WeaponMgr.h"
#include "C_GameStd.h"

HRESULT TowerSrc::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;


	return hr;
}

void TowerSrc::SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	m_StateList[StateNUM]->SetMatrix(world, view, proj);
	m_world = *world;
	D3DXVECTOR3 pos = D3DXVECTOR3(world->_41, world->_42, world->_43);
	RectSet(pos);
}

void TowerSrc::RectSet(D3DXVECTOR3 pos)
{
	m_Center.x = 0 + pos.x;
	m_Center.y = 0 + pos.y;
	m_Center.z = 0 + pos.z;			//센터값에서 이동값 더함

	m_fRect.left = m_Center.x - m_Size.x;
	m_fRect.top = m_Center.y + m_Size.y;
	m_fRect.right = m_Center.x + m_Size.x;
	m_fRect.bottom = m_Center.y - m_Size.y;
}

void TowerSrc::SetAniTime(float Time)
{

}
bool TowerSrc::Init()
{
	return true;
}

void TowerSrc::SetState(DWORD dwState)
{
	if (StateNUM != dwState)
	{
		StateNUM = dwState;
		m_Index = 0;
	}
}

bool TowerSrc::Update(float& ftime)
{
	End = false;
	if (ftime >= m_SecPerRender)
	{
		m_Index++;
		ftime = 0;
		if (m_Index > (m_StateList[StateNUM]->GetSpriteSize() - 1))
		{
			End = true;
			m_Index = 0;
		}
	}
	m_StateList[StateNUM]->End = End;

	/*float f_ChangTime = ftime;
	m_StateList[StateNUM]->CSprite::Update(f_ChangTime);
	ftime = f_ChangTime;
	return true;*/
	return true;
}

bool TowerSrc::Frame()
{
	m_StateList[StateNUM]->CSprite::Frame();
	return true;
}

bool TowerSrc::Render()
{
	m_StateList[StateNUM]->SetSpriteIndex(m_Index);
	m_StateList[StateNUM]->CSprite::Render();
	return true;
}

HRESULT TowerSrc::CreateVertexData()					//정점 버퍼 데이터 생성
{
	HRESULT hr = S_OK;

	m_obj.m_VertexSize = sizeof(PNCT_VERTEX);

	m_VerTex.resize(4);

	m_VerTex[0].p = D3DXVECTOR3(-35.0f, 35.0f, 0.5f);
	m_VerTex[1].p = D3DXVECTOR3(35.0f, 35.0f, 0.5f);
	m_VerTex[2].p = D3DXVECTOR3(-35.0f, -35.0f, 0.5f);
	m_VerTex[3].p = D3DXVECTOR3(35.0f, -35.0f, 0.5f);				//정점 좌표 입력(직교 투영)	

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

void TowerSrc::TextSrc()
{
	//타워 기본
	m_StateList[TOWER_STANDBY] = std::make_shared<TowerSrc>();
	m_StateList[TOWER_STANDBY]->CSprite::Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/Tower.bmp", "VSmat", "PSBlend2");
	TextArray(TOWER_STANDBY);
	m_StateList[TOWER_STANDBY]->SetRectArray(m_RtList, 3840, 1080);
	m_StateList[TOWER_STANDBY]->SetAnimation(0.1f, CHAR_ANIMATION_TIME);

	//타워 공격
	m_StateList[TOWER_ATTACK] = std::make_shared<TowerSrc>();
	m_StateList[TOWER_ATTACK]->CSprite::Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/Tower.bmp", "VSmat", "PSBlend2");
	TextArray(TOWER_ATTACK);
	m_StateList[TOWER_ATTACK]->SetRectArray(m_RtList, 3840, 1080);
	m_StateList[TOWER_ATTACK]->SetAnimation(0.1f, CHAR_ANIMATION_TIME);

	//타워 레벨 업 기본
	m_StateList[TOWER_LEVELUP_STATE] = std::make_shared<TowerSrc>();
	m_StateList[TOWER_LEVELUP_STATE]->CSprite::Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/Tower.bmp", "VSmat", "PSBlend2");
	TextArray(TOWER_LEVELUP_STATE);
	m_StateList[TOWER_LEVELUP_STATE]->SetRectArray(m_RtList, 3840, 1080);
	m_StateList[TOWER_LEVELUP_STATE]->SetAnimation(0.1f, CHAR_ANIMATION_TIME);

	//타워 레벨 업 공격
	m_StateList[TOWER_LEVELUP_ATTACK] = std::make_shared<TowerSrc>();
	m_StateList[TOWER_LEVELUP_ATTACK]->CSprite::Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/Tower.bmp", "VSmat", "PSBlend2");
	TextArray(TOWER_LEVELUP_ATTACK);
	m_StateList[TOWER_LEVELUP_ATTACK]->SetRectArray(m_RtList, 3840, 1080);
	m_StateList[TOWER_LEVELUP_ATTACK]->SetAnimation(0.1f, CHAR_ANIMATION_TIME);
}

void TowerSrc::TextArray(int a)
{

}

void TowerSrc::SetLV(int i)
{
	iLV = i;
}

int TowerSrc::GetLV()
{
	return iLV;
}

TowerWeapon* TowerSrc::GetWeapon()
{
	return &m_WeaponList[iLV];
}

C_EffectSrc* TowerSrc::GetEffect()
{
	return &m_EffectList[iLV];
}

bool TowerSrc::Release()
{
	for (int i = 0; i < m_StateList.size(); i++)
	{
		m_StateList[i]->CSprite::Release();
	}
	m_StateList.clear();

	for (int i = 0; i < m_WeaponList.size(); i++)
	{
		m_WeaponList[i].Release();
	}
	m_WeaponList.clear();

	return true;
}

bool TowerSrc::Attack()
{
	if (bGoku)
	{
		if (StateNUM == TOWER_ATTACK)
		{
			return m_StateList[StateNUM]->End;
		}
		else if (StateNUM == TOWER_LEVELUP_ATTACK)
		{
			if (m_StateList[StateNUM]->m_Index == 1 && bGoKuLVAttack)
			{
				bGoKuLVAttack = false;
				return true;
			}

			if (m_StateList[StateNUM]->End)
			{
				bGoKuLVAttack = true;
				return true;
			}
		}
	}
	else
	{
		if (StateNUM == TOWER_ATTACK || StateNUM == TOWER_LEVELUP_ATTACK)
		{
			return m_StateList[StateNUM]->End;
		}
	}

	return false;
}

//void TowerSrc::Attack()
//{
//	TowerWeapon* weapon = TOWER_WEAPON.m_WeaponQueue.Pop();
//	*weapon = m_Weapon;
//	TOWER_WEAPON.m_pWeaponList.push_back(weapon);
//}




TowerSrc::TowerSrc()
{
	D3DXMatrixIdentity(&m_mat);
	D3DXMatrixIdentity(&m_world);
	D3DXMatrixIdentity(&m_view);
	D3DXMatrixIdentity(&m_proj);
	StateNUM = TOWER_STANDBY;
	m_Index = 0;

	bCold = false;
	bGoku = false;
	bVegeta = false;
	bGoKuLVAttack = true;
}


TowerSrc::~TowerSrc()
{
}
