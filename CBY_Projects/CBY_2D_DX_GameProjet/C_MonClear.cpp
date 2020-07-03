#include "C_MonClear.h"

void C_MonClear::Process(std::vector<std::shared_ptr<C_Tower>>& Tower)
{
	m_pMonster->m_Monster->SetState(MONSTER_DEATH);
	
}


C_MonClear::C_MonClear(C_Monster* host) :C_MonsterStateProcess(host)
{
}


C_MonClear::~C_MonClear()
{
}
