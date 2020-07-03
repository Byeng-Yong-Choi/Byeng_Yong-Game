#pragma once
#include "C_TowerState.h"

class C_TowerFsm
{
public:
	std::map<DWORD, C_TowerState*>  m_MapList;

public:
	bool Init();
	void Input(DWORD State, DWORD Event, DWORD Output);
	DWORD Output(DWORD State, DWORD Event);

public:
	static C_TowerFsm& Get()
	{
		static C_TowerFsm mgr;
		return mgr;
	}
public:
	C_TowerFsm();
	~C_TowerFsm();
};