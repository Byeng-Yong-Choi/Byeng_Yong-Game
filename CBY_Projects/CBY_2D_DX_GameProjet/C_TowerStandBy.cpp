#include "C_TowerStandBy.h"
#include "MonsterMgr.h"

void C_TowerStandBy::Process()
{
	for (int i = 0; i < MONSTER.GetMonsterList().size(); i++)
	{
		if (ColCheck(MONSTER.GetMonster(i)))
		{
			m_pTower->TowerRot(MONSTER.GetMonster(i)->m_Monster->m_Center);
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
