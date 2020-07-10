#pragma once
#include "CBY_BossAi.h"

namespace KYS
{
	class VFX_EffectObj;
	class VFX_Effects;
}

namespace CBY
{
	class CBY_BossFlame : public CBY_BossProcess
	{
	private:
		float m_fPlayerHit;

	public:
		void Process()override;

	private:
		bool m_bFlame;
		D3DXVECTOR3 m_vBallPos;
		D3DXVECTOR3 m_vFlamePos;

	public:
		CBY_BossFlame(CBY_BossAi* host);
		virtual ~CBY_BossFlame();
	};
}

