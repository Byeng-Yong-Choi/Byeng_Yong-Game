#pragma once
#include "KG_Std.h"
#include "CBY_CharacterStateStd.h"

namespace CBY
{
	enum BOSS_STATE
	{
		BOSS_IDLE = 0,
		BOSS_WALK,
		BOSS_RUN,
		BOSS_HIT,
		BOSS_SLEEP,
		BOSS_SCREAM,
		BOSS_ATTACK,
		BOSS_ATTACK_MOUTH,
		BOSS_TAKE_OFF,
		BOSS_LAND,
		BOSS_FLY_IDLE,
		BOSS_FLY_WALK,
		BOSS_FLAME,
		BOSS_FLY_FLAME,
		BOSS_DIE,
		BOSS_STATECOUNT
	};

	enum Boss_State
	{
		BOSS_STATE_IDLE = 0,
		BOSS_STATE_FLYLAND,
		BOSS_STATE_WALK,
		BOSS_STATE_HIT,
		BOSS_STATE_ATTACK,
		BOSS_STATE_FLAME,
		BOSS_STATE_DIE,
		BOSS_STATE_COUNT
	};

	class CBY_BossState
	{
	public:
		std::map<DWORD, DWORD> m_StateMap;
		DWORD m_State;

	public:
		void Input(DWORD Event, DWORD Output);
		DWORD Output(DWORD Event);

	public:
		CBY_BossState();
		virtual ~CBY_BossState();
	};
}
