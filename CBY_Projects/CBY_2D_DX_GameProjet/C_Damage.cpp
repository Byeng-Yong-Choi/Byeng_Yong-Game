#include "C_Damage.h"

void C_Damage::Process(std::vector<std::shared_ptr<C_Tower>>& Tower)
{
	fDamagetime += g_SecondTime;

	m_pMonster->m_Monster->SetState(MONSTER_DAMAGE);
	m_pMonster->SetMonsterAttackHP();
	if (m_pMonster->HPZero())
	{
		m_pMonster->SetAction(EVENT_DEATH);
		return;
	}
	m_pMonster->SetAction(EVENT_LOST);

}


C_Damage::C_Damage(C_Monster* host) :C_MonsterStateProcess(host)
{
	fDamagetime = 0.0f;
}


C_Damage::~C_Damage()
{
}
