#include "C_ATTACK.h"
#include "WeaponMgr.h"
#include "C_EffectMgr.h"
#include "MonsterMgr.h"

void C_ATTACK::Process()
{
	bool ColdAttack = false;
	int iHidden=0;
	for (int i = 0 ; i <MONSTER.GetMonsterList().size(); i++)
	{
		if (MONSTER.GetMonster(i)->GetTarget())
		{
			if (ColCheck(MONSTER.GetMonster(i)))
			{
				if (m_pTower->MakeTowerNum == COLD)
				{
					if (MONSTER.GetMonster(i)->GetColdEffectTime())
					{
						TOWER_EFFECT.EffectMake(m_pTower->m_Tower[COLD]->GetEffect(), &MONSTER.GetMonster(i)->m_MatWorld);
					}
					ColdAttack = true;
					MONSTER.GetMonster(i)->SetColdAttack(true, m_pTower->GetColdSlowValue());
				}
				else
				{
					if (m_pTower->m_Tower[m_pTower->MakeTowerNum]->Attack())
					{
						TOWER_WEAPON.WeaponMake(m_pTower->m_Tower[m_pTower->MakeTowerNum]->GetWeapon(),
							&m_pTower->m_matWorld, MONSTER.GetMonster(i)->m_iNumber, m_pTower->m_Tower[m_pTower->MakeTowerNum]->GetLV());
					}
					if (m_pTower->bHidden())
					{
						if (iHidden >= 2)
						{
							return;
						}
						iHidden++;
					}
					else
					{
						return;
					}
				}
			}
		}
	}

	if (ColdAttack || iHidden > 0)
	{
		return;
	}

	m_pTower->SetAction(EVENT_LOSTTARGET);
	if (m_pTower->MakeLvTower)
	{
		if (m_pTower->bHidden())
		{
			m_pTower->m_Tower[m_pTower->MakeTowerNum]->SetState(TOWER_STANDBY);
		}
		else
		{
			m_pTower->m_Tower[m_pTower->MakeTowerNum]->SetState(TOWER_LEVELUP_STATE);
		}
	}
	else
	{
		m_pTower->m_Tower[m_pTower->MakeTowerNum]->SetState(TOWER_STANDBY);
	}
	return;

}



C_ATTACK::C_ATTACK(C_Tower* host) : C_TowerStateProcess(host)
{
	
}


C_ATTACK::~C_ATTACK()
{
}
