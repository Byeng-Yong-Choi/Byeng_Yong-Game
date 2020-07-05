#include "CBY_BossState.h"

namespace CBY
{
	void CBY_BossState::Input(DWORD Event, DWORD Output)
	{
		m_StateMap[Event] = Output;
	}

	DWORD CBY_BossState::Output(DWORD Event)
	{
		return m_StateMap[Event];
	}

	CBY_BossState::CBY_BossState()
	{
	}


	CBY_BossState::~CBY_BossState()
	{
	}
}
