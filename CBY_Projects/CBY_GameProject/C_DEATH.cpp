#include "C_DEATH.h"

void C_DEATH :: Process(vector<C_Tower>& Tower)
{
	//std::list<TowerEffect*> wf;
	//TowerEffect* Effectnode;
	//std::list<TowerEffect*>::iterator efiter;

	//for (int i = 0; i < Tower.size(); i++)
	//{
	//	for (efiter = Tower[i].m_EffectList.begin(); efiter != Tower[i].m_EffectList.end(); )
	//	{
	//		Effectnode = efiter.operator*();
	//		Effectnode->Frame();
	//		//Effectnode->SetPosition(I_Col.Center(m_pMonster->BitMapRt));
	//		if (!Effectnode->EffectShow)
	//		{
	//			delete Effectnode;
	//			efiter = Tower[i].m_EffectList.erase(efiter);
	//		}
	//		else
	//		{
	//			//Effectnode->Render();
	//			efiter++;
	//		}
	//	}
	//}

	m_pMonster->Rotate = false;
	
	int monindex=-1;
	m_pMonster->Death = 1;
	m_pMonster->HPbar->Death = 1;
	m_pMonster->Frame();
	m_pMonster->HPbar->Frame();
	if (m_pMonster->temp == 1)
	{
		m_pMonster->Make = 0;
		m_pMonster->HPbar->Make = 0;
		m_pMonster->HPbar->Release();
		m_pMonster->ok = true;
		monindex = m_pMonster->index;
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
			if(weanode->monsternum == monindex)
			{
				if (weanode->Buu)
				{
					weaiter++;
				}
				else
				{
					weanode->Release();
					delete weanode;
					weaiter = Tower[i].m_WeaponList.erase(weaiter);
				}
			}
			else
			{
				weaiter++;
			}
		}
	}
}

C_DEATH::C_DEATH(C_Monster* host) :C_MonsterStateProcess(host)
{
}


C_DEATH::~C_DEATH()
{
}
