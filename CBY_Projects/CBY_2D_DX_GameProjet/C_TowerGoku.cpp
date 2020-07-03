#include "C_TowerGoku.h"
HRESULT C_TowerGoku::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;
	m_obj.m_pd3dDevice = pd3dDevice;
	m_obj.m_pContext = Context;
	m_StateList.resize(TOWER_COUNT);
	TextSrc();
	SetWeapon();

	StateNUM = TOWER_STANDBY;
	bGoku = true;
	return hr;
}

void C_TowerGoku::SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	m_StateList[StateNUM]->SetMatrix(world, view, proj);
}

void C_TowerGoku::TextArray(int i)
{
	m_RtList.clear();
	switch (i)
	{
	case TOWER_STANDBY:
		m_RtList.resize(1);
		m_RtList[0] = { 199, 302, 35, 51 };
		break;

	case TOWER_ATTACK:
		m_RtList.resize(3);
		m_RtList[0] = { 115,270,71,60 };
		m_RtList[1] = { 39,321,42,66 };
		m_RtList[2] = { 86,339,36,51 };
		break;

	case TOWER_LEVELUP_STATE:
		m_RtList.resize(1);
		m_RtList[0] = { 178,457,69,61 };
		break;

	case TOWER_LEVELUP_ATTACK:
		m_RtList.resize(3);
		m_RtList[0] = { 183,386,66,58 };
		m_RtList[1] = { 34,413,52,48 };
		m_RtList[2] = { 34,473,51,48 };
		break;
		
	}
}

void C_TowerGoku::SetWeapon()
{
	m_WeaponList.resize(2);
	m_EffectList.resize(2);

	for (int i = 0; i < m_WeaponList.size(); i++)
	{
		m_WeaponList[i].Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/Tower.bmp", "VSmat", "PSBlend2");
		if (i == 0)
		{
			m_RtList.clear();
			m_RtList.resize(1);
			m_RtList[0] = { 36, 322, 42, 14 };
			m_WeaponList[i].SetPower(GOKU_ATTACK);
		}
		else
		{
			m_RtList.clear();
			m_RtList.resize(1);
			m_RtList[0] = { 124, 430, 15, 14 };
			m_WeaponList[i].SetPower(GOKU_LV_ATTACK);
		}

		m_WeaponList[i].SetRectArray(m_RtList, 3840, 1080);
		m_WeaponList[i].WeaponSpeed = GOKU_SPEED;
		m_WeaponList[i].m_iTowerChar = GOKU;
	}

	for (int i = 0; i < m_EffectList.size(); i++)
	{
		m_EffectList[i].Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/Effectscolor.bmp", "VSmat", "PSBlend2");
		
		if (i == 0)
		{
			m_RtList.clear();
			m_RtList.resize(5);
			m_RtList[0] = { 156,5347,40,39 };
			m_RtList[1] = { 238,5331,72,73 };
			m_RtList[2] = { 337,5330,72,73 };
			m_RtList[3] = { 434,5331,72,73 };
			m_RtList[4] = { 282,1945,47,46 };
		}
		else
		{
			m_RtList.clear();
			m_RtList.resize(4);
			m_RtList[0] = { 272,1935,40,39 };
			m_RtList[1] = { 369,1933,72,73 };
			m_RtList[2] = { 466,1936,72,73 };
			m_RtList[3] = { 566,1937,69,69 };
		}
		m_EffectList[i].SetRectArray(m_RtList, 1024, 5765);
		m_EffectList[i].SetAnimation(EFFECT_ANIMATION_TIME, 0.1f);
					   
		m_EffectList[i].m_iSound = I_SOUND.Load(L"son.wav", false,0.4f);
	}

	m_iPrice[0] = 50;
	m_iPrice[1] = 100;
}

C_TowerGoku::C_TowerGoku()
{
}


C_TowerGoku::~C_TowerGoku()
{
}
