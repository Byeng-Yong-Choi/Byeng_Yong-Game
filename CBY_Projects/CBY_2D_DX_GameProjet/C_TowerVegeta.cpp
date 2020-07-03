#include "C_TowerVegeta.h"
HRESULT C_TowerVegeta::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;

	m_obj.m_pd3dDevice = pd3dDevice;
	m_obj.m_pContext = Context;
	m_StateList.resize(TOWER_COUNT);
	TextSrc();
	SetWeapon();

	StateNUM = TOWER_STANDBY;
	bVegeta = true;
	return hr;
}

void C_TowerVegeta::SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	m_StateList[StateNUM]->SetMatrix(world, view, proj);
}

void C_TowerVegeta::TextArray(int i)
{
	m_RtList.clear();
	switch (i)
	{
	case TOWER_STANDBY:
		m_RtList.resize(1);
		m_RtList[0] = { 866,118,36,45 };
		break;

	case TOWER_ATTACK:
		m_RtList.resize(2);
		m_RtList[0] = { 847,182,71,60 };
		m_RtList[1] = { 931,124,43,48 };
		break;

	case TOWER_LEVELUP_STATE:
		m_RtList.resize(1);
		m_RtList[0] = { 760,29,69,61 };
		break;

	case TOWER_LEVELUP_ATTACK:
		m_RtList.resize(2);
		m_RtList[0] = { 845,32,64,58 };
		m_RtList[1] = { 946,38,45,49 };
		break;
	}
}

void C_TowerVegeta::SetWeapon()
{
	m_WeaponList.resize(2);
	m_EffectList.resize(2);

	for (int i = 0; i < m_WeaponList.size(); i++)
	{
		m_WeaponList[i].Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/Tower.bmp", "VSmat", "PSBlend2");
		m_RtList.clear();
		m_RtList.resize(1);
		m_RtList[0] = { 971,113,74,60 };

		m_WeaponList[i].SetRectArray(m_RtList, 3840, 1080);
		if (i == 0)
		{
			m_WeaponList[i].SetPower(VEGETA_ATTACK);
		}
		else
		{
			m_WeaponList[i].SetPower(VEGETA_LV_ATTACK);
		}
		m_WeaponList[i].WeaponSpeed = VEGETA_SPEED;
		m_WeaponList[i].m_iTowerChar = VEGETA;
	}

	m_RtList.clear();
	m_RtList.resize(7);
	m_RtList[0] = { 430,481,42,38 };
	m_RtList[1] = { 546,466,72,73 };
	m_RtList[2] = { 665,456,94,93 };
	m_RtList[3] = { 773,447,125,106 };
	m_RtList[4] = { 1,580,125,106 };
	m_RtList[5] = { 127,578,125,106 };
	m_RtList[6] = { 254,577,132,106 };
	
	for (int i = 0; i < m_WeaponList.size(); i++)
	{
		m_EffectList[i].Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/Effectscolor.bmp", "VSmat", "PSBlend2");
		m_EffectList[i].SetRectArray(m_RtList, 1024, 5765);
		m_EffectList[i].SetAnimation(EFFECT_ANIMATION_TIME, 0.1f);

		m_EffectList[i].m_iSound = I_SOUND.Load(L"ve.wav", false,0.6f);
	}

	m_iPrice[0] = 60;
	m_iPrice[1] = 100;
}

void C_TowerVegeta::TextSrc()
{
	//타워 기본
	m_StateList[TOWER_STANDBY] = std::make_shared<TowerSrc>();
	m_StateList[TOWER_STANDBY]->CSprite::Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/Tower.bmp", "VSmat", "PSBlend2");
	TextArray(TOWER_STANDBY);
	m_StateList[TOWER_STANDBY]->SetRectArray(m_RtList, 3840, 1080);
	//m_StateList[TOWER_STANDBY]->UpdateVertexData(m_vertexdata);
	m_StateList[TOWER_STANDBY]->SetAnimation(0.1f, CHAR_ANIMATION_TIME);

	//타워 공격
	m_StateList[TOWER_ATTACK] = std::make_shared<TowerSrc>();
	m_StateList[TOWER_ATTACK]->CSprite::Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/Tower.bmp", "VSmat", "PSBlend2");
	TextArray(TOWER_ATTACK);
	m_StateList[TOWER_ATTACK]->SetRectArray(m_RtList, 3840, 1080);
	//m_StateList[TOWER_ATTACK]->UpdateVertexData(m_vertexdata);
	m_StateList[TOWER_ATTACK]->SetAnimation(0.1f, CHAR_ANIMATION_TIME+ VEGETA_FIRESPEED);

	//타워 레벨 업 기본
	m_StateList[TOWER_LEVELUP_STATE] = std::make_shared<TowerSrc>();
	m_StateList[TOWER_LEVELUP_STATE]->CSprite::Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/Tower.bmp", "VSmat", "PSBlend2");
	TextArray(TOWER_LEVELUP_STATE);
	m_StateList[TOWER_LEVELUP_STATE]->SetRectArray(m_RtList, 3840, 1080);
	//m_StateList[TOWER_LEVELUP_STATE]->UpdateVertexData(m_vertexdata);
	m_StateList[TOWER_LEVELUP_STATE]->SetAnimation(0.1f, CHAR_ANIMATION_TIME);

	//타워 레벨 업 공격
	m_StateList[TOWER_LEVELUP_ATTACK] = std::make_shared<TowerSrc>();
	m_StateList[TOWER_LEVELUP_ATTACK]->CSprite::Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/Tower.bmp", "VSmat", "PSBlend2");
	TextArray(TOWER_LEVELUP_ATTACK);
	m_StateList[TOWER_LEVELUP_ATTACK]->SetRectArray(m_RtList, 3840, 1080);
	//m_StateList[TOWER_LEVELUP_ATTACK]->UpdateVertexData(m_vertexdata);
	m_StateList[TOWER_LEVELUP_ATTACK]->SetAnimation(0.1f, CHAR_ANIMATION_TIME+ VEGETA_FIRESPEED);
}

C_TowerVegeta::C_TowerVegeta()
{
}


C_TowerVegeta::~C_TowerVegeta()
{
}
