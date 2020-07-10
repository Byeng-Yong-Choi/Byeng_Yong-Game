#include "CBY_BossFlyLand.h"

namespace CBY
{
	void CBY_BossFlyLand::Process()
	{
		if (!m_pBoss->GetFly())
		{
			m_pBoss->SetState(BOSS_TAKE_OFF);
			if (m_pBoss->GetStateAnimationEnd(BOSS_TAKE_OFF))
			{
				m_pBoss->SetAction(EVENT_IDLE);
				m_pBoss->SetFly(true);
			}
		}
		else
		{
			m_pBoss->SetState(BOSS_LAND);
			if (m_pBoss->GetStateAnimationEnd(BOSS_LAND))
			{
				m_pBoss->SetAction(EVENT_IDLE);
				m_pBoss->SetFly(false);
			}
		}
	}

	CBY_BossFlyLand::CBY_BossFlyLand(CBY_BossAi* host) : CBY_BossProcess(host)
	{
	}


	CBY_BossFlyLand::~CBY_BossFlyLand()
	{
	}
}
