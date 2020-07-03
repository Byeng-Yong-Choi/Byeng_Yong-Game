#pragma once
#include "Cstd.h"

enum State
{
	STATE_STANDBY=0,
	STATE_MAKE,
	STATE_MOVE,
	STATE_DAMAGE,
	STATE_DEATH,
	STATE_CLEAR,
	STATE_COUNT
};

enum Event
{
	EVENT_START=0,
	EVENT_LOST,
	EVENT_HIT,
	EVENT_DEATH,
	EVENT_END
};

class C_MonsterState
{
public:
	std::map<DWORD, DWORD> m_StateMap;
	DWORD m_State;

public:
	void Input(DWORD Event, DWORD Output);
	DWORD Output(DWORD Event);


public:
	C_MonsterState();
	~C_MonsterState();
};

