#include "C_Null.h"

void C_Null::Process()
{
	cold = false;
	if (m_pTower->MakeTower)
	{
		m_pTower->SetAction(EVENT_CREATCLICK);
	}
}

C_Null::C_Null(C_Tower* host) : C_TowerStateProcess(host)
{
}


C_Null::~C_Null()
{
}
