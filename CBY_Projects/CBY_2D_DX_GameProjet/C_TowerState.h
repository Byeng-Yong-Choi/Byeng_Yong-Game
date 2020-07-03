#pragma once
#include "Cstd.h"

enum TOWERState
{
	STATE_NULL = 0,
	STATE_TOWERSTANDBY,
	STATE_CREAT,
	STATE_ATTACK,
	STATE_LEVELUP,
	STATE_DEMOLISH,
	STATE_TOWERCOUNT
};

enum TOWEREvent
{
	EVENT_TARGET = 0,
	EVENT_NULL,
	EVENT_LOSTTARGET,
	EVENT_CREATCLICK,
	EVENT_LEVELCLICK,
	EVENT_DEMOLISHCLICK,
};

class C_TowerState
{
public:
	std::map<DWORD, DWORD> m_StateMap;
	DWORD m_State;

public:
	void Input(DWORD Event, DWORD Output);
	DWORD Output(DWORD Event);


public:
	C_TowerState();
	~C_TowerState();
};

