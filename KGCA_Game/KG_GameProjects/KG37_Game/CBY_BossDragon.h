#pragma once
#include "CBY_BossAi.h"

namespace CBY
{
	enum CJH_BOSS_SOUNDTRACK
	{
		SOUND_BOSS_APPEAR = 0,
		SOUND_BOSS_HIT,
		SOUND_BOSS_DIE,
		SOUND_BOSS_ATTACK,
		SOUND_BOSS_ATTACK_MOUTH,
		SOUND_BOSS_MOVES,
		SOUND_BOSS_FLAME,
		SOUND_BOSS_COUNT,

	};

	class CBY_BossDragon : public CBY_BossAi
	{
	public:
		bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)override;

	public:
		CBY_BossDragon();
		virtual ~CBY_BossDragon();
	};
}

