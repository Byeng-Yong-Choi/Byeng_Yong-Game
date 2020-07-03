#include "C_TowerCold.h"
HRESULT C_TowerCold::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;
	m_obj.m_pd3dDevice = pd3dDevice;
	m_obj.m_pContext = Context;
	m_StateList.resize(TOWER_COUNT);
	TextSrc();
	SetWeapon();

	StateNUM = TOWER_STANDBY;
	bCold = true;
	return hr;
}

void C_TowerCold::SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	m_StateList[StateNUM]->SetMatrix(world, view, proj);
}

void C_TowerCold::TextArray(int i)
{
	m_RtList.clear();
	switch (i)
	{
	case TOWER_STANDBY:
		m_RtList.resize(1);
		m_RtList[0] = { 633,42,35,47 };
		break;

	case TOWER_ATTACK:
		m_RtList.resize(1);
		m_RtList[0] = { 381,30,65,58 };
		break;

	case TOWER_LEVELUP_STATE:
		m_RtList.resize(1);
		m_RtList[0] = { 448,32,64,55 };
		break;

	case TOWER_LEVELUP_ATTACK:
		m_RtList.resize(1);
		m_RtList[0] = { 518,32,64,55 };
		break;
	}
}

void C_TowerCold::SetWeapon()
{
	m_WeaponList.resize(1);
	m_EffectList.resize(1);
	m_WeaponList[0].Cold = true;
	m_WeaponList[0].m_iTowerChar = COLD;
	m_EffectList[0].Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/Icecolor.bmp", "VSmat", "PSBlend2");

	m_RtList.clear();
	m_RtList.resize(7);
	m_RtList[0] = { 11,215,43,21 };
	m_RtList[1] = { 74,212,50,27 };
	m_RtList[2] = { 133,209,56,32 };
	m_RtList[3] = { 201,208,56,32 };
	m_RtList[4] = { 343,209,44,32 };
	m_RtList[5] = { 407,208,44,36 };
	m_RtList[6] = { 468,210,44,36 };

	m_EffectList[0].SetRectArray(m_RtList, 518, 429);
	m_EffectList[0].SetAnimation(EFFECT_ANIMATION_TIME-1.0f, 0.1f);

	m_EffectList[0].m_iSound = I_SOUND.Load(L"cold.wav", false,0.5f);
	
	m_iPrice[0] = 80;
	m_iPrice[1] = 80;
}

TowerWeapon* C_TowerCold::GetWeapon()
{
	return &m_WeaponList[0];
}

C_EffectSrc* C_TowerCold::GetEffect()
{
	return &m_EffectList[0];
}

C_TowerCold::C_TowerCold()
{
	fSpeedSlow[0] = COLD_SLOW;
	fSpeedSlow[1] = COLD_SLOW;
}


C_TowerCold::~C_TowerCold()
{
}
