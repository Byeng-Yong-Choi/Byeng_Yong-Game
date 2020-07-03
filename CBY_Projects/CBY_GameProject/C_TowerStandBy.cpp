#include "C_TowerStandBy.h"

void C_TowerStandBy::Process(list<C_Monster*>& obj)
{
	TowerWeapon* weanode;
	std::list<TowerWeapon*>::iterator weaiter;

	TowerEffect* Effectnode;
	std::list<TowerEffect*>::iterator efiter;

	if (obj.empty())
	{
		for (weaiter = m_pTower->m_WeaponList.begin(); weaiter != m_pTower->m_WeaponList.end(); )
		{
			weanode = weaiter.operator*();
			weanode->Release();
			delete weanode;
			weaiter = m_pTower->m_WeaponList.erase(weaiter);
		}
		m_pTower->m_WeaponList.clear();

		for (efiter = m_pTower->m_EffectList.begin(); efiter != m_pTower->m_EffectList.end(); )
		{
			Effectnode = efiter.operator*();
			delete Effectnode;
			efiter = m_pTower->m_EffectList.erase(efiter);
		}
		m_pTower->m_EffectList.clear();
	}

	if (m_pTower->TowerNumber == 3 && m_pTower->m_WeaponList.size() == 1)
	{
		for (weaiter = m_pTower->m_WeaponList.begin(); weaiter != m_pTower->m_WeaponList.end();)
		{
			weanode = weaiter.operator*();
			weanode->Release();
			delete weanode;
			weaiter = m_pTower->m_WeaponList.erase(weaiter);
		}
		m_pTower->m_WeaponList.clear();
	}



	C_Monster* iter;
	bool weapongo = false;
	int Tower;
	int MonsterNumber;
	fPOINT monpt;
	Tower = m_pTower->TowerNumber;
	if (m_pTower->MakeLvTower)
	{
		m_pTower->m_LVTower[Tower].CharShowTime = 0.0f;
	}
	else
	{
		m_pTower->m_Tower[Tower].CharShowTime = 0.0f;
	}
	m_pTower->TowerStateNumber = 0;
	m_pTower->Frame();
	if (!m_pTower->m_WeaponList.empty())
	{
		for (auto Weapon1 : m_pTower->m_WeaponList)
		{
			MonsterNumber = Weapon1->monsternum;
			for (auto monster : obj)
			{
				if (monster->index == MonsterNumber)
				{
					iter = monster;
					monpt = iter->GetPosition();
					weapongo = true;
					break;
				}
				else
					weapongo = false;
			}
			if (weapongo)
			{
				if(!Weapon1->Buu)
				Weapon1->GetMonsterPosition(monpt);
				//Weapon1->Frame();
			}
			else
			{
				//Weapon1->Frame();
				//Weapon1->WeaponGO = false;
			}
		}

	}
	fSphere towerfS;
	fSphere monsterfS;
	for (auto monst : obj)
	{
		if (m_pTower->MakeLvTower)
		{
			towerfS = m_pTower->m_LVTower[Tower].charfsp;
			monsterfS = monst->fsp;
		}
		else
		{
			towerfS = m_pTower->m_Tower[Tower].charfsp;
			monsterfS = monst->fsp;
		}

		if (I_Col.Spherecollision(towerfS, monsterfS))
		{
			if(m_pTower->MakeLvTower)
			{
				dgr=m_pTower->m_LVTower[Tower].Enemy(m_pTower->CenterPT, monst->GetPoint());
			}
			else
			{
				dgr=m_pTower->m_Tower[Tower].Enemy(m_pTower->CenterPT, monst->GetPoint());
			}
			
			
			
			m_pTower->SetAction(EVENT_TARGET);
			return;
		}
	}

}

C_TowerStandBy::C_TowerStandBy(C_Tower* host) : C_TowerStateProcess(host)
{
}


C_TowerStandBy::~C_TowerStandBy()
{
}
