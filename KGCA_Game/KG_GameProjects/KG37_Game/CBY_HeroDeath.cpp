#include "CBY_HeroDeath.h"
#include "KG_SoundMgr.h"
#include "UI_objMgr.h"

namespace CBY
{
	void CBY_HeroDeath::Process()
	{
		m_pHero->SetState(CHAR_DEATH);
		if (m_pHero->GetStateAnimationStart(CHAR_DEATH))
		{
			if (m_pHero->m_dwSoundList.size() > 0)
				I_SOUND.list[m_pHero->m_dwSoundList[SOUND_DIE]]->EffectPlay(I_SOUND.m_pSystem);
		}
		
		if (m_pHero->GetStateAnimationEnd(CHAR_DEATH))
		{
			m_pHero->SetEndClip(CHAR_DEATH, true);
			m_bDeathEnd = true;
		}

		if (m_bDeathEnd && I_Input.KeyCheck(VK_F5))
		{
			m_pHero->SetAction(EVENT_CREATE);
			m_pHero->InitSet();
			m_bDeathEnd = false;
		}
	}

	CBY_HeroDeath::CBY_HeroDeath(CBY_Hero* host) : CBY_HeroProcess(host)
	{
		m_bDeathEnd = false;
	}


	CBY_HeroDeath::~CBY_HeroDeath()
	{
	}
}
