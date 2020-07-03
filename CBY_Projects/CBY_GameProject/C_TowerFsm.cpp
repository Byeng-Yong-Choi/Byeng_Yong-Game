#include "C_TowerFsm.h"

bool C_TowerFsm::Init()
{
	Input(STATE_NULL, EVENT_CREATCLICK, STATE_TOWERSTANDBY);
	Input(STATE_TOWERSTANDBY, EVENT_TARGET, STATE_ATTACK);
	Input(STATE_TOWERSTANDBY, EVENT_LEVELCLICK, STATE_LEVELUP);
	Input(STATE_TOWERSTANDBY, EVENT_DEMOLISHCLICK, STATE_DEMOLISH);
	Input(STATE_LEVELUP, EVENT_NULL, STATE_TOWERSTANDBY);
	Input(STATE_ATTACK, EVENT_LOSTTARGET, STATE_TOWERSTANDBY);
	Input(STATE_ATTACK, EVENT_LEVELCLICK, STATE_LEVELUP);
	Input(STATE_ATTACK, EVENT_DEMOLISHCLICK, STATE_DEMOLISH);
	Input(STATE_DEMOLISH, EVENT_NULL, STATE_NULL);
	Input(STATE_ATTACK, EVENT_NULL, STATE_NULL);
	Input(STATE_TOWERSTANDBY, EVENT_NULL, STATE_NULL);
	Input(STATE_LEVELUP, EVENT_NULL, STATE_NULL);
	

	return true;
}

void C_TowerFsm::Input(DWORD State, DWORD Event, DWORD Output)
{
	C_TowerState* monst = nullptr;
	
	std::map< DWORD, C_TowerState*>::iterator towIter;
	towIter = m_MapList.find(State);
	if (towIter == m_MapList.end())
	{
		monst = new C_TowerState;
		m_MapList[State] = monst;
	}
	else
	{
		monst = (C_TowerState*)towIter->second;
	}

	monst->Input(Event, Output);

}

DWORD C_TowerFsm::Output(DWORD State, DWORD Event)
{
	C_TowerState* towst = m_MapList[State];
	return towst->Output(Event);
}

C_TowerFsm::C_TowerFsm()
{
}


C_TowerFsm::~C_TowerFsm()
{
}
