#include "TowerWeapon.h"

bool TowerWeapon::Frame()
{
	D3DXVECTOR3 vPos = { 0,0,0 };

	float fD = sqrt((m_vTargetpos.x - m_Center.x) * (m_vTargetpos.x - m_Center.x) +
		(m_vTargetpos.y - m_Center.y)*(m_vTargetpos.y - m_Center.y) + 
		(m_vTargetpos.z - m_Center.z) * (m_vTargetpos.z - m_Center.z));

	float fDirX = m_vTargetpos.x - m_Center.x;
	float fDirY = m_vTargetpos.y - m_Center.y;
	float fDirZ = m_vTargetpos.z - m_Center.z;
	fDirX /= fD;
	fDirY /= fD;
	fDirZ /= fD;

	vPos.x = m_Center.x + fDirX * (WeaponSpeed * g_SecondTime);
	vPos.y = m_Center.y + fDirY * (WeaponSpeed * g_SecondTime);
	vPos.z = m_Center.z + fDirZ * (WeaponSpeed * g_SecondTime);

	m_World._41 = vPos.x;
	m_World._42 = vPos.y;
	m_World._43 = vPos.z;

	return true;
}

HRESULT TowerWeapon::CreateVertexData()
{
	HRESULT hr = S_OK;

	m_obj.m_VertexSize = sizeof(PNCT_VERTEX);

	m_VerTex.resize(4);

	m_VerTex[0].p = D3DXVECTOR3(-21.0f, 7.0f, 0.5f);
	m_VerTex[1].p = D3DXVECTOR3(21.0f, 7.0f, 0.5f);
	m_VerTex[2].p = D3DXVECTOR3(-21.0f, -7.0f, 0.5f);
	m_VerTex[3].p = D3DXVECTOR3(21.0f, -7.0f, 0.5f);				//정점 좌표 입력(직교 투영)	

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

void TowerWeapon::SetMonsterNum(int i)
{
	iMonsternum = i;
}

void TowerWeapon::SetMonsterPosition(D3DXVECTOR3 mon)
{
	//monpt = mon[monsternum].GetPosition();
	m_vTargetpos = mon;
}

void TowerWeapon::SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	if (world != nullptr)
	{
		D3DXMATRIX mat = *world;
		m_World = mat * m_World;
	}

	SetMatrix(&m_World, view, proj);

	D3DXVECTOR3 pos = D3DXVECTOR3(m_World._41, m_World._42, m_World._43);
	RectSet(pos);
}

void TowerWeapon::RectSet(D3DXVECTOR3 pos)
{
	m_Center.x = pos.x;
	m_Center.y = pos.y;
	m_Center.z = pos.z;			//센터값에서 이동값 더함

	m_fRect.left = m_Center.x - m_Size.x;
	m_fRect.top = m_Center.y + m_Size.y;
	m_fRect.right = m_Center.x + m_Size.x;
	m_fRect.bottom = m_Center.y - m_Size.y;
}

void TowerWeapon::SetPower(int power)
{
	m_iPower = power;
}

int TowerWeapon::GetPower()
{
	return m_iPower;
}


TowerWeapon::TowerWeapon()
{
	//WeaponGO = true;
	WeaponSpeed = 0;
	fMove = 0;
	Cold = false;
	m_iPower = 0;
	bLV = false;
	D3DXMatrixIdentity(&m_World);
}


TowerWeapon::~TowerWeapon()
{
}
