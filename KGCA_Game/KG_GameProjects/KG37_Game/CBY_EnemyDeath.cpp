#include "CBY_EnemyDeath.h"
#include "KG_SoundMgr.h"

namespace CBY
{
	void CBY_EnemyDeath::Process()
	{
		m_pEnemy->SetState(CHAR_DEATH);
		if (m_pEnemy->GetStateAnimationStart(CHAR_DEATH))
		{
			if (m_pEnemy->m_dwSoundList.size() > 0)
				I_SOUND.list[m_pEnemy->m_dwSoundList[SOUND_DIE]]->EffectPlay(I_SOUND.m_pSystem);
		}
		if (m_pEnemy->GetStateAnimationEnd(CHAR_DEATH))
		{
			m_pEnemy->SetEndClip(CHAR_DEATH, true);
			m_pEnemy->SetDie(true);
		}
	}

	CBY_EnemyDeath::CBY_EnemyDeath(CBY_EnemyAi* host) : CBY_EnemyProcess(host)
	{
	}


	CBY_EnemyDeath::~CBY_EnemyDeath()
	{
	}
}
