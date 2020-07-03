#include "C_Make.h"

void C_Make::Process(std::vector<std::shared_ptr<C_Tower>>& Tower)
{
	m_pMonster->m_fTime += g_SecondTime;
	if (m_pMonster->m_fStartTime <= m_pMonster->m_fTime)
	{
		m_pMonster->m_iRoot = 0;
		m_pMonster->Sethide(false);
		m_pMonster->SetTarget(true);
		m_pMonster->SetAction(EVENT_START);
		m_pMonster->SetRotaion(135.0f);
		m_pMonster->m_fTime = 0.0f;
	}


}

C_Make::C_Make(C_Monster* host) :C_MonsterStateProcess(host)
{
	fStart = 0.0f;
}


C_Make::~C_Make()
{
}
