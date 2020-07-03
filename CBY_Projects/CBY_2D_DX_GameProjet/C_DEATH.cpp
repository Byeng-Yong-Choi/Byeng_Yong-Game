#include "C_DEATH.h"
#include "C_MoneyMgr.h"

void C_DEATH :: Process(std::vector<std::shared_ptr<C_Tower>>& Tower)
{
	m_pMonster->m_Monster->SetState(MONSTER_DEATH);
	m_pMonster->SetTarget(false);

	m_pMonster->m_Monster->m_ConData.ddata.x -= g_SecondTime;
	if (m_pMonster->m_Monster->m_ConData.ddata.x < 0)
	{
		m_pMonster->m_Monster->m_ConData.ddata.x = 0;
	}

	if (m_pMonster->m_Monster->GetSpliteEnd())
	{
		//fDeathTime = 0;
		m_pMonster->m_Monster->m_ConData.ddata.x = 1.0f;
		m_pMonster->SetDeath(true);
		int i=0;
		if (m_pMonster->GetBoss())
		{
			i += 50;
		}
		int money = MONSTER_MONEY + I_MONEY.GetMoney() + i;
		I_MONEY.SetMoney(money);
	}
}

C_DEATH::C_DEATH(C_Monster* host) :C_MonsterStateProcess(host)
{
}


C_DEATH::~C_DEATH()
{
}
