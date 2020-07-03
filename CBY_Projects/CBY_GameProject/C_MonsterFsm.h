#pragma once
#include "C_MonsterState.h"

class C_MonsterFsm
{
public:
	std::map<DWORD, C_MonsterState*>  m_MapList;

public:
	bool Init();
	void Input(DWORD State, DWORD Event, DWORD Output);
	DWORD Output(DWORD State, DWORD Event);

public:
	static C_MonsterFsm& Get()
	{
		static C_MonsterFsm mgr;
		return mgr;
	}

public:
	C_MonsterFsm();
	~C_MonsterFsm();
};

