#include "C_Make.h"

void C_Make::Process(vector<C_Tower>& Tower)
{
	m_pMonster->Frame();
	RECT HPrt = { 0,0,m_pMonster->BitMapRt.right-15,5 };
	m_pMonster->HPbar->GetRECT(HPrt);
	m_pMonster->HPbar->SetPosition(m_pMonster->BitMapRt.left, m_pMonster->BitMapRt.bottom + 5);
	m_pMonster->HPbar->HPNUM = m_pMonster->HPNUMBER;
	m_pMonster->HPbar->Init();
	m_pMonster->HPbar->Frame();
	m_pMonster->Make = 1;
	m_pMonster->HPbar->Make = 1;
	m_pMonster->SetAction(EVENT_START);


}

C_Make::C_Make(C_Monster* host) :C_MonsterStateProcess(host)
{
}


C_Make::~C_Make()
{
}
