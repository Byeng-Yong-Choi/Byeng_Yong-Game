#include "C_MonsterState.h"

void C_MonsterState::Input(DWORD Event, DWORD Output)
{
	m_StateMap[Event] = Output;
}

DWORD C_MonsterState::Output(DWORD Event)
{
	return m_StateMap[Event];
}

C_MonsterState::C_MonsterState()
{
}


C_MonsterState::~C_MonsterState()
{
}
