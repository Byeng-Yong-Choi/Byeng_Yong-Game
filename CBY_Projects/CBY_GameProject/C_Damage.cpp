#include "C_Damage.h"

void C_Damage::Process(vector<C_Tower>& Tower)
{
	std::list<TowerEffect*> wf;
	TowerEffect* Effectnode;
	std::list<TowerEffect*>::iterator efiter;


	m_pMonster->Rotate = false;
	
	
	m_pMonster->Frame();
	m_pMonster->HPbar->Frame();
	
	std::list<TowerWeapon*> wea;
	TowerWeapon* weanode;
	std::list<TowerWeapon*>::iterator weaiter;
	
	

	for (int i = 0; i < Tower.size(); i++)
	{
		//wea = Tower[i].m_WeaponList;

		for (weaiter = Tower[i].m_WeaponList.begin(); weaiter != Tower[i].m_WeaponList.end(); )
		{
			weanode = weaiter.operator*();

			if (I_Col.collision(m_pMonster->BitMapRt, weanode->BitMapRt))
			{
				
				m_pMonster->HPbar->Hit += weanode->Power;
				weanode->Release();
				delete weanode;
				weaiter = Tower[i].m_WeaponList.erase(weaiter);
					TowerEffect* Effect = new TowerEffect(Tower[i].m_Effect[Tower[i].TowerNumber]);
					Effect->Frame();
					Effect->SetPosition(I_Col.Center(m_pMonster->BitMapRt));
					Tower[i].m_EffectList.push_back(Effect);
				//}
				if (m_pMonster->HPbar->Hit >= m_pMonster->HPNUMBER)
				{
					m_pMonster->SetAction(EVENT_DEATH);
					return;
				}
				m_pMonster->Death = 5;
				m_pMonster->SetAction(EVENT_HIT);
				return;
			}
			else
			{
				weaiter++;
			}
		}
		
		
	}
	if (m_pMonster->HPbar->Hit >= m_pMonster->HPNUMBER)
	{
		m_pMonster->SetAction(EVENT_DEATH);
		return;
	}

	
	 m_pMonster->SetAction(EVENT_LOST);
}


C_Damage::C_Damage(C_Monster* host) :C_MonsterStateProcess(host)
{
	motiontime = 1.0f;
	damagetime = 1.0f;
}


C_Damage::~C_Damage()
{
}
