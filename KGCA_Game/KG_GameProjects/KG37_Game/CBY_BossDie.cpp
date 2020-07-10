#include "CBY_BossDie.h"
#include "KG_SoundMgr.h"
#include "CBY_BossDragon.h"
#include "VFX_ObjMgr.h"

namespace CBY
{
	void CBY_BossDie::Process()
	{
		m_pBoss->obj->Shutdown();
		m_pBoss->obj2->Shutdown();
		m_pBoss->obj3->Shutdown();

		m_pBoss->SetState(BOSS_DIE);
		if (m_pBoss->GetStateAnimationStart(BOSS_DIE))
		{
			I_SOUND.list[m_pBoss->m_dwSoundList[SOUND_BOSS_DIE]]->EffectPlay(I_SOUND.m_pSystem);
		}
		if (m_pBoss->GetStateAnimationEnd(BOSS_DIE))
		{
			m_pBoss->SetEndClip(BOSS_DIE, true);
			m_pBoss->SetDie(true);
		}
	}

	CBY_BossDie::CBY_BossDie(CBY_BossAi* host) : CBY_BossProcess(host)
	{
	}


	CBY_BossDie::~CBY_BossDie()
	{
	}
}
