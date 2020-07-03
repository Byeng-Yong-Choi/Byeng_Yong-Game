#include "C_InGame.h"
#include "C_GameStd.h"
#include "MonsterMgr.h"
#include "WeaponMgr.h"
#include "C_EffectMgr.h"
#include "C_SoundManager.h"
#include "C_MoneyMgr.h"
#include "C_GameMap.h"
#include "C_TowerHiddenMgr.h"
#include "C_TowerSrcMgr.h"

bool C_InGame::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	m_pd3dDevice = pd3dDevice;
	m_pContext = Context;

	I_MONEY.SetMoney(GAME_START_MONEY);

	CDX::ApplyBS(m_pContext, CDX::CDxState::g_pAlpahBlend);
	CDX::ApplySS(m_pContext, CDX::CDxState::g_pSampler);
	CDX::ApplyRS(m_pContext, CDX::CDxState::g_pRSSold);
	CDX::ApplyDSS(m_pContext, CDX::CDxState::g_pDSSDisable);

	bgm = I_SOUND.Load(L"ingame2.mp3", false, 0.5f);
	m_iBossSound = I_SOUND.Load(L"Boss.mp3", false, 0.5f);

	I_MAP.Create(m_pd3dDevice, m_pContext);

	m_Menu = std::make_shared<C_Menu>();
	m_Menu->UICreate(m_pd3dDevice, m_pContext);

	MONSTER.Create(m_pd3dDevice, m_pContext);
	TOWERSRC.Create(m_pd3dDevice, m_pContext);
	TOWER_WEAPON.Init();
	TOWER_EFFECT.Init();

	MONSTER.InitMonsterList();

	m_TowerList.resize(TOWER_NUMBER);
	for (int i = 0; i < m_TowerList.size(); i++)
	{
		m_TowerList[i] = std::make_shared<C_Tower>();
		m_TowerList[i]->Init();
		if (FAILED(m_TowerList[i]->CreateInit(m_pd3dDevice, m_pContext)))
		{
			return false;
		}
	}

	return true;
}

bool C_InGame::Frame()
{
	if (!m_bBoss)
	{
		I_SOUND.list[m_iBossSound]->Stop();
		I_SOUND.list[bgm]->Play(I_SOUND.m_pSystem);
	}
	else
	{
		I_SOUND.list[bgm]->Stop();
		I_SOUND.list[m_iBossSound]->Play(I_SOUND.m_pSystem);
	}

	I_MAP.Frame();
	I_MAP.SetMatrix(nullptr, nullptr, &m_pMainCamera->m_OrthoProj);

	m_Menu->Frame();
	m_Menu->UISetMatrix(nullptr, nullptr, &m_pMainCamera->m_OrthoProj);

	GetGameMoney();						//시간에 따른 재화 상승

	for (int i = 0; i < m_TowerList.size(); i++)
	{
		m_TowerList[i]->Process();
	}

	if (MONSTER.GetMonsterList().size() <= 0)					//몬스터가 다 죽었을때
	{
		MONSTER.InitMonsterList();
	}

	MONSTER.SetMatrix(nullptr, &m_pMainCamera->m_View, &m_pMainCamera->m_OrthoProj);

	for (int i = 0; i < MONSTER.GetMonsterList().size();)
	{
		MONSTER.GetMonster(i)->Process(m_TowerList);

		if (MONSTER.GetMonster(i)->MonsterState == STATE_CLEAR || MONSTER.GetMonster(i)->GetDeath())			//몬스터가 죽었을 때
		{
			if (MONSTER.GetMonster(i)->MonsterState == STATE_CLEAR)
			{
				m_Menu->SetHP(1);
			}

			MONSTER.MonsterPop(i);
		}
		else
		{
			i++;
		}
	}

	TOWER_WEAPON.Process(m_TowerList[0]);
	HIDDEN.HiddenMgrFrame(m_TowerList);
	TOWER_WEAPON.SetMatrix(nullptr, &m_pMainCamera->m_View, &m_pMainCamera->m_OrthoProj);
	TOWER_EFFECT.SetMatrix(nullptr, &m_pMainCamera->m_View, &m_pMainCamera->m_OrthoProj);
	return true;
}

bool C_InGame::Render()
{
	I_MAP.Render();
	m_Menu->UIRender();
	MONSTER.Render();
	for (int i = 0; i < m_TowerList.size(); i++)
	{
		m_TowerList[i]->SetWorldandMatrix(&TowerPositionSet(i), &m_pMainCamera->m_View, &m_pMainCamera->m_OrthoProj);
		m_TowerList[i]->Render();
	}
	TOWER_WEAPON.Render();
	TOWER_EFFECT.Render();
	return true;
}

bool C_InGame::Release()
{
	for (int i = 0; i < m_TowerList.size(); i++)
	{
		m_TowerList[i]->Release();
	}
	m_TowerList.clear();
	I_MAP.Release();
	m_Menu->UIRelease();

	TOWERSRC.Release();
	TOWER_WEAPON.Release();
	TOWER_EFFECT.Release();
	MONSTER.Release();
	I_SOUND.Release();
	HIDDEN.Release();
	return true;
}

D3DXMATRIX C_InGame::TowerPositionSet(int i)
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	float UISizeP, UISizeM;
	UISizeP = (Winrt.bottom / 9.0f);
	UISizeM = (Winrt.bottom / 14.0f);
	switch (i)
	{
	case 0:
		mat._41 = -(Winrt.right / 2.0f) + (Winrt.right / 6.1f);
		mat._42 = (Winrt.bottom / 2.0f) - (Winrt.bottom / 2.0f) + UISizeP;
		break;

	case 1:
		mat._41 = -(Winrt.right / 2.0f) + (Winrt.right / 3.0f);
		mat._42 = (Winrt.bottom / 2.0f) - (Winrt.bottom / 1.9f) + UISizeP;
		break;

	case 2:
		mat._41 = -(Winrt.right / 2.0f) + (Winrt.right / 4.0f);
		mat._42 = (Winrt.bottom / 2.0f) - (Winrt.bottom / 2.9f) + UISizeM;
		break;

	case 3:
		mat._41 = -(Winrt.right / 2.0f) + (Winrt.right / 2.42f);
		mat._42 = (Winrt.bottom / 2.0f) - (Winrt.bottom / 2.9f) + UISizeM;
		break;

	case 4:
		mat._41 = -(Winrt.right / 2.0f) + (Winrt.right / 2.16f);
		mat._42 = (Winrt.bottom / 2.0f) - (Winrt.bottom / 1.27f) + UISizeP;
		break;

	case 5:
		mat._41 = -(Winrt.right / 2.0f) + (Winrt.right / 1.7f);
		mat._42 = (Winrt.bottom / 2.0f) - (Winrt.bottom / 2.81f) + UISizeM;
		break;

	case 6:
		mat._41 = -(Winrt.right / 2.0f) + (Winrt.right / 1.15f);
		mat._42 = (Winrt.bottom / 2.0f) - (Winrt.bottom / 2.90f) + UISizeM;
		break;

	case 7:
		mat._41 = -(Winrt.right / 2.0f) + (Winrt.right / 2.16f);
		mat._42 = (Winrt.bottom / 2.0f) - (Winrt.bottom / 1.9f) + UISizeP;
		break;

	case 8:
		mat._41 = -(Winrt.right / 2.0f) + (Winrt.right / 1.2f);
		mat._42 = (Winrt.bottom / 2.0f) - (Winrt.bottom / 4.5f) + UISizeM;
		break;

	case 9:
		mat._41 = -(Winrt.right / 2.0f) + (Winrt.right / 1.20f);
		mat._42 = (Winrt.bottom / 2.0f) - (Winrt.bottom / 1.9f) + UISizeP;
		break;

	case 10:
		mat._41 = -(Winrt.right / 2.0f) + (Winrt.right / 6.19f);
		mat._42 = (Winrt.bottom / 2.0f) - (Winrt.bottom / 1.36f) + UISizeP;
		break;

	case 11:
		mat._41 = -(Winrt.right / 2.0f) + (Winrt.right / 3.4f);
		mat._42 = (Winrt.bottom / 2.0f) - (Winrt.bottom / 4.5f) + UISizeM;
		break;
	}

	return mat;
}

bool C_InGame::GameLose()
{
	if (m_Menu->PlayerDeath())
	{
		I_SOUND.list[bgm]->Stop();
		I_SOUND.list[m_iBossSound]->Stop();
		return true;
	}
	return false;
}

void C_InGame::SetCamera(CCamera* camera)
{
	m_pMainCamera = camera;
}

void C_InGame::GetGameMoney()
{
	fMoneyTime += g_SecondTime;

	if (fMoneyTime > 1.0f)
	{
		fMoneyTime = 0.0f;
		int money;
		money = I_MONEY.GetMoney() + MONEY_TIME_NUM;
		I_MONEY.SetMoney(money);
	}
}


HRESULT C_InGame::CreateDXrc()
{
	HRESULT hr = S_OK;

	I_MAP.CreateVertexData();
	I_MAP.UpdateVertexData();

	m_Menu->CreateVertexData();
	m_Menu->UpdateVertexData(m_Menu->m_VerTex);

	MONSTER.PosUpdate();
	/*for (int i = 0; i < m_pMonsterList.size(); i++)
	{
		m_pMonsterList[i]->pig->m_Src->m_Pos = D3DXVECTOR3(-(Winrt.right / 2.03f), -(Winrt.bottom / 11.61f) + (Winrt.bottom / 8.0f), 0.0f);

		if (m_pMonsterList[i]->MonsterState == STATE_STANDBY || m_pMonsterList[i]->MonsterState == STATE_MAKE)
		{
			m_pMonsterList[i]->SetMonster(MONSTER_PIG, m_iLevel);
		}
	}*/
	return hr;
}

void C_InGame::GameSet()
{
	fMoneyTime = 0;
	m_iLevel = -1;

	m_Menu->GameSet();
	I_MONEY.SetMoney(GAME_START_MONEY);
	for (int i = 0; i < m_TowerList.size(); i++)
	{
		m_TowerList[i]->GameSet();
	}

	MONSTER.MonsterClear();

	selectbgm = 0;
	m_bBoss = false;
}

C_InGame::C_InGame()
{
	fMoneyTime = 0;
	m_iLevel = -1;
	selectbgm = 0;
	m_bBoss = false;
}


C_InGame::~C_InGame()
{
}
