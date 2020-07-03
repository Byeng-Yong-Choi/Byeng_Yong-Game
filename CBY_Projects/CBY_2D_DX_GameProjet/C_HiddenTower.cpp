#include "C_HiddenTower.h"

HRESULT C_HiddenTower::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;
	m_obj.m_pd3dDevice = pd3dDevice;
	m_obj.m_pContext = Context;
	m_StateList.resize(2);
	TextSrc();
	SetWeapon();

	StateNUM = TOWER_STANDBY;
	return hr;
}

void C_HiddenTower::SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	m_StateList[StateNUM]->SetMatrix(world, view, proj);
}

void C_HiddenTower::TextArray(int i)
{
	m_RtList.clear();
	switch (i)
	{
	case TOWER_STANDBY:
		m_RtList.resize(5);
		m_RtList[0] = { 247, 91, 57, 64 };
		m_RtList[1] = { 312, 91, 67, 64 };
		m_RtList[2] = { 382, 92, 67, 64 };
		m_RtList[3] = { 312, 91, 67, 64 };
		m_RtList[4] = { 247, 91, 57, 64 };
		break;

	case TOWER_ATTACK:
		m_RtList.resize(3);
		m_RtList[0] = { 5,105,33,51 };
		m_RtList[1] = { 7,464,33,51 };
		m_RtList[2] = { 43,464,40,51 };
		break;
	}
}

void C_HiddenTower::SetWeapon()
{
	m_WeaponList.resize(1);
	m_EffectList.resize(1);

	for (int i = 0; i < m_WeaponList.size(); i++)
	{
		m_WeaponList[i].Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/veget.bmp", "VSmat", "PSBlend2");
		m_RtList.clear();
		m_RtList.resize(1);
		m_RtList[0] = { 165, 454, 72, 62 };
		m_WeaponList[i].SetPower(HIDDEN_ATTACK);

		m_WeaponList[i].SetRectArray(m_RtList, 494, 1055);
		m_WeaponList[i].WeaponSpeed = HIDDEN_SPEED;
		m_WeaponList[i].m_iTowerChar = HIDDEN;
	}

	for (int i = 0; i < m_EffectList.size(); i++)
	{
		m_EffectList[i].Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/Effectscolor.bmp", "VSmat", "PSBlend2");

		m_RtList.clear();
		m_RtList.resize(7);
		m_RtList[0] = { 243,3941,224,154 };
		m_RtList[1] = { 487,3941,224,154 };
		m_RtList[2] = { 726,3941,224,154 };
		m_RtList[3] = { 7,4181,224,154 };
		m_RtList[4] = { 243,4181,224,154 };
		m_RtList[5] = { 489,4181,224,154 };
		m_RtList[6] = { 729,4181,224,154 };
		
		m_EffectList[i].SetRectArray(m_RtList, 1024, 5765);
		m_EffectList[i].SetAnimation(EFFECT_ANIMATION_TIME, 0.1f);

		m_EffectList[i].m_iSound = I_SOUND.Load(L"hidden.wav", false, 0.6f);
	}

	m_iPrice[0] = 50;
	m_iPrice[1] = 100;
}

void C_HiddenTower::TextSrc()
{
	//타워 기본
	m_StateList[TOWER_STANDBY] = std::make_shared<TowerSrc>();
	m_StateList[TOWER_STANDBY]->CSprite::Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/veget.bmp", "VSmat", "PSBlend2");
	TextArray(TOWER_STANDBY);
	m_StateList[TOWER_STANDBY]->SetRectArray(m_RtList, 494, 1055);
	m_StateList[TOWER_STANDBY]->SetAnimation(0.1f, CHAR_ANIMATION_TIME);

	//타워 공격
	m_StateList[TOWER_ATTACK] = std::make_shared<TowerSrc>();
	m_StateList[TOWER_ATTACK]->CSprite::Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/veget.bmp", "VSmat", "PSBlend2");
	TextArray(TOWER_ATTACK);
	m_StateList[TOWER_ATTACK]->SetRectArray(m_RtList, 494, 1055);
	m_StateList[TOWER_ATTACK]->SetAnimation(0.1f, CHAR_ANIMATION_TIME);
}


C_HiddenTower::C_HiddenTower()
{
}


C_HiddenTower::~C_HiddenTower()
{
}
