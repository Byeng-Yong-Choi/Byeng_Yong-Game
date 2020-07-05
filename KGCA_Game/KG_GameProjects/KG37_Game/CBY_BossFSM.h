#pragma once
#include "CBY_BossState.h"

namespace CBY
{
	class CBY_BossFSM
	{
	public:
		std::map<DWORD, CBY_BossState*>  m_MapList;

	public:
		bool Init();
		void Input(DWORD State, DWORD Event, DWORD Output);
		DWORD Output(DWORD State, DWORD Event);

	public:
		static CBY_BossFSM& Get()
		{
			static CBY_BossFSM mgr;
			return mgr;
		}

	public:
		CBY_BossFSM();
		virtual ~CBY_BossFSM();
	};
}
