#include "C_MonClear.h"

void C_MonClear::Process(vector<C_Tower>& Tower)
{
	
	m_pMonster->Rotate = false;

	int monindex = -1;
	m_pMonster->Death = 1;
	m_pMonster->HPbar->Death = 1;
	m_pMonster->Frame();
	m_pMonster->HPbar->Frame();
	if (m_pMonster->temp == 1)
	{
		m_pMonster->Make = 0;
		m_pMonster->HPbar->Make = 0;
		m_pMonster->HPbar->Release();
		monindex = m_pMonster->index;
		m_pMonster->End = true;
	}

	std::list<TowerWeapon*> wea;
	TowerWeapon* weanode;
	std::list<TowerWeapon*>::iterator weaiter;
	for (int i = 0; i < Tower.size(); i++)
	{
		//wea = Tower[i].m_WeaponList;

		for (weaiter = Tower[i].m_WeaponList.begin(); weaiter != Tower[i].m_WeaponList.end(); )
		{
			weanode = weaiter.operator*();
			if (weanode->monsternum == monindex)
			{
				weanode->Release();
				delete weanode;
				weaiter = Tower[i].m_WeaponList.erase(weaiter);
			}
			else
			{
				weaiter++;
			}
		}
	}
}


C_MonClear::C_MonClear(C_Monster* host) :C_MonsterStateProcess(host)
{
}


C_MonClear::~C_MonClear()
{
}
