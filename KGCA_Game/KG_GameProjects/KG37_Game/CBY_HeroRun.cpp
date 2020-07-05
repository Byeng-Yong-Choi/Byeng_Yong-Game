#include "CBY_HeroRun.h"
#include "KG_SoundMgr.h"

namespace CBY
{
	void CBY_HeroRun::Process()
	{
		m_pHero->SetState(CHAR_RUN);

		//SetColDirection();
		I_SOUND.list[m_pHero->m_dwSoundList[SOUND_RUN]]->Play(I_SOUND.m_pSystem);
		/*if (m_pHero->GetStateAnimationStart(CHAR_RUN))
		{
			I_SOUND.list[m_pHero->m_dwSoundList[SOUND_RUN]]->EffectPlay(I_SOUND.m_pSystem);
		}
		if (m_pHero->GetStateAnimationEnd(CHAR_RUN))
		{
			I_SOUND.list[m_pHero->m_dwSoundList[SOUND_RUN]]->EffectPlay(I_SOUND.m_pSystem);
		}*/

		bool bRun = false;
		bool bMove = false;

		if (I_Input.GetKeyCheck(VK_SPACE))
		{
			m_pHero->SetAction(EVENT_JUMP);
			return;
		}

		if (I_Input.GetKeyCheck(VK_SHIFT))
		{
			bRun = true;
		}

		m_vMove = m_pHero->GetHeroPos();
		m_vLook = m_pHero->GetCamera()->m_LookDir;
		m_vSide = m_pHero->GetCamera()->m_SideDir;
		SetColDirection();

		bMove = CharcterMove(bRun);

		if (!bRun)
		{
			m_pHero->SetAction(EVENT_MOVE);
		}

		if (bRun || bMove)
		{
			m_pHero->SetHeroPos(m_vMove);
		}
		else
		{
			m_pHero->SetAction(EVENT_IDLE);
		}
	}

	CBY_HeroRun::CBY_HeroRun(CBY_Hero* host) : CBY_HeroProcess(host)
	{
	}


	CBY_HeroRun::~CBY_HeroRun()
	{
	}
}
