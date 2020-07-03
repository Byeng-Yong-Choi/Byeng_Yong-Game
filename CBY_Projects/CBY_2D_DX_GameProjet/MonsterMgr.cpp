#include "MonsterMgr.h"
#include "C_GameStd.h"

HRESULT MonsterMgr::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;

	m_MonsterSrc = std::make_shared<C_Monster>();
	m_PigSrc = std::make_shared<C_monPig>();
	m_RoboSrc = std::make_shared<C_monRobo>();;
	m_BossSrc = std::make_shared<C_Boss>();;
	m_HPSrc = std::make_shared<C_MonsterHP>();;

	m_PigSrc->Create(pd3dDevice, Context);
	m_RoboSrc->Create(pd3dDevice, Context);
	m_BossSrc->Create(pd3dDevice, Context);
	m_HPSrc->Create(pd3dDevice, Context);

	for (int i = 0; i < MONSTERNUMBER; i++)
	{
		C_Monster* mon = new C_Monster;
		*mon = *m_MonsterSrc;
		mon->Init();
		mon->GetMonsterSrc(m_PigSrc.get(), m_RoboSrc.get(), m_BossSrc.get(), m_HPSrc.get());
		m_MonsterQueue.Push(mon);
	}


	return hr;
}

void MonsterMgr::InitMonsterList()
{
	m_iLevel += 1;

	m_bBoss = false;

	int iMon = m_iLevel % 2;
	if (m_iLevel % 5 == 0 && m_iLevel != 0)
	{
		m_bBoss = true;
		MonsterMake(MONSTER_BOSS, m_iLevel, 0);
	}
	else
	{
		for (int i = 0; i < MONSTERNUM; i++)
		{
			MonsterMake(iMon, m_iLevel, i);
		}
	}
}


bool MonsterMgr::Init()
{
	return true;
}

bool MonsterMgr::Frame()
{
	for (int i = 0; i < m_pMonsterList.size(); i++)
	{
		m_pMonsterList[i]->Frame();
	}

	return true;
}

bool MonsterMgr::Render()
{
	for (int i = 0; i < m_pMonsterList.size(); i++)
	{
		m_pMonsterList[i]->SetMatrix(nullptr, &m_matView, &m_matProj);

		if (!m_pMonsterList[i]->Gethide())
		{
			m_pMonsterList[i]->Render();
		}
	}

	/*for (int i = 0; i < m_pMonsterList.size(); i++)
	{
		m_pMonsterList[i]->Render();
	}*/
	return true;
}

bool MonsterMgr::Release()
{
	for (int i = 0; i < m_pMonsterList.size(); i++)
	{
		m_pMonsterList[i]->Release();
	}
	m_pMonsterList.clear();
	m_PigSrc->Release();
	m_RoboSrc->Release();
	m_BossSrc->Release();
	m_HPSrc->Release();
	return true;
}

void MonsterMgr::MonsterClear()
{
	for (DWORD dw = 0; dw < m_pMonsterList.size();)
	{
		MonsterPop(dw);
	}
}

void MonsterMgr::SetMatrix(D3DXMATRIX* World, D3DXMATRIX* View, D3DXMATRIX* Proj)
{
	if (World != nullptr)
	{
		m_matWorld = *World;
	}
	if (View != nullptr)
	{
		m_matView = *View;
	}
	if (Proj != nullptr)
	{
		m_matProj = *Proj;
	}
}

void MonsterMgr::MonsterMake(int iMonstertype, int iLevel, DWORD index)
{
	C_Monster* mon = m_MonsterQueue.Pop();
	mon->SetMonster(iMonstertype, iLevel);
	mon->m_iNumber = index;
	mon->m_iNumber = index;
	mon->m_fStartTime = index * 1.5f;
	mon->SetMatrix(nullptr, &m_matView, &m_matProj);
	m_pMonsterList.push_back(mon);
}

Moniter MonsterMgr::MonsterPop(Moniter imon)
{
	Moniter mon;
	m_MonsterQueue.Push(*imon);
	mon = m_pMonsterList.erase(imon);
	return mon;
}

void MonsterMgr::MonsterPop(int imon)
{
	Moniter iter = m_pMonsterList.begin() + imon;
	m_MonsterQueue.Push(*iter);
	m_pMonsterList.erase(iter);
}

void MonsterMgr::PosUpdate()
{
	C_Monster* mon = m_MonsterQueue.Pop();
	while (mon !=nullptr)
	{
		m_pMonsterList.push_back(mon);
		mon = m_MonsterQueue.Pop();
	}

	while(m_pMonsterList.size() !=0 )
	{
		m_pMonsterList[0]->pig->m_Src->m_Pos = D3DXVECTOR3(-(Winrt.right / 2.03f), -(Winrt.bottom / 11.61f)+(Winrt.bottom / 8.0f), 0.0f);
		std::vector<C_Monster*>::iterator iter = m_pMonsterList.begin();
		m_MonsterQueue.Push(*iter);
		m_pMonsterList.erase(iter);
	}
}

MonsterMgr::MonsterMgr()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	m_iLevel = -1;
	m_bBoss = false;
}


MonsterMgr::~MonsterMgr()
{
}
