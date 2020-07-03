#include "C_STANDBY.h"

void C_STANDBY::Process(std::vector<std::shared_ptr<C_Tower>>& Tower)
{
	m_pMonster->SetAction(EVENT_START);
}


C_STANDBY::C_STANDBY(C_Monster* host) :C_MonsterStateProcess(host)
{
}


C_STANDBY::~C_STANDBY()
{
}
