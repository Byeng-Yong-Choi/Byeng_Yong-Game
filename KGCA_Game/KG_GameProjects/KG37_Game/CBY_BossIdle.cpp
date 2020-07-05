#include "CBY_BossIdle.h"
#include "CBY_GameMgr.h"
#include"KG_SoundMgr.h"

namespace CBY
{
	void CBY_BossIdle::Process()
	{
		if (m_pBoss->GetSleep())
		{
			m_pBoss->BossSleepState();
		}
		else
		{
			UpdateLook();

			if (m_pBoss->GetFly())
			{
				m_pBoss->SetState(BOSS_FLY_IDLE);
			}
			else
			{
				m_pBoss->SetState(BOSS_IDLE);
			}
			
			m_fSkillTime += g_SecondTime;
			if (m_fSkillTime > m_pBoss->GetSkillTime())
			{
				m_pBoss->SetAction(EVENT_SKILL);
				m_fSkillTime = 0.0f;
				m_fTime = 0.0f;
			}
			else
			{
				if (m_fTime > 2.0f)
				{
					srand((unsigned int)time(NULL));
					int iCase = rand() % 10;

					if (iCase < 7)
					{
						m_pBoss->SetAction(EVENT_MOVE);
						//m_pBoss->SetAction(EVENT_ATTACK);
					}
					else
					{
					}
					m_fTime = 0.0f;
				}
				else
				{
					m_fTime += g_SecondTime;
				}
			}
		}

	}

	CBY_BossIdle::CBY_BossIdle(CBY_BossAi* host) : CBY_BossProcess(host)
	{
		m_fTime = 0;
	}


	CBY_BossIdle::~CBY_BossIdle()
	{
	}
}

