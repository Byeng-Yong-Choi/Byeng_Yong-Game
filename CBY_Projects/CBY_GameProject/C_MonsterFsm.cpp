#include "C_MonsterFsm.h"

bool C_MonsterFsm::Init()
{
	Input(STATE_STANDBY,EVENT_START,STATE_MAKE);
	Input(STATE_MAKE, EVENT_START, STATE_MOVE);
	Input(STATE_MOVE, EVENT_HIT, STATE_DAMAGE);
	Input(STATE_DAMAGE, EVENT_LOST, STATE_MOVE);
	Input(STATE_DAMAGE, EVENT_HIT, STATE_MOVE);
	Input(STATE_MOVE, EVENT_DEATH, STATE_DEATH);
	Input(STATE_DAMAGE, EVENT_DEATH, STATE_DEATH);
	Input(STATE_MOVE, EVENT_END, STATE_CLEAR);

	return true;
}

void C_MonsterFsm::Input(DWORD State, DWORD Event, DWORD Output)
{
	C_MonsterState* monst=nullptr;

	std::map< DWORD, C_MonsterState*>::iterator monIter;
	monIter = m_MapList.find(State);
	if (monIter == m_MapList.end())
	{
		monst = new C_MonsterState;
		m_MapList[State] = monst;
	}
	else
	{
		monst = (C_MonsterState*)monIter->second;
	}

	monst->Input(Event, Output);

}

DWORD C_MonsterFsm::Output(DWORD State, DWORD Event)
{
	C_MonsterState* monst=m_MapList[State];
	return monst->Output(Event);
}

C_MonsterFsm::C_MonsterFsm()
{
}


C_MonsterFsm::~C_MonsterFsm()
{
}
