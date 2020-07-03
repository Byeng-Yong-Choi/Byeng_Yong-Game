#include "C_TowerState.h"

void C_TowerState::Input(DWORD Event, DWORD Output)
{
	m_StateMap[Event] = Output;
}

DWORD C_TowerState::Output(DWORD Event)
{
	return m_StateMap[Event];
}

C_TowerState::C_TowerState()
{
}


C_TowerState::~C_TowerState()
{
}
