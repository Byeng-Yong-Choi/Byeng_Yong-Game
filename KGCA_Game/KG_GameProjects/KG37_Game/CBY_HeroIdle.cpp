#include "CBY_HeroIdle.h"
#include "KG_SoundMgr.h"
namespace CBY
{
	void CBY_HeroIdle::Process()
	{
		m_pHero->SetState(CBY::CHAR_IDLE);
		//SetColDirection();
		//m_pHero->SetHeroPos(m_vMove);
		I_SOUND.list[m_pHero->m_dwSoundList[SOUND_RUN]]->Stop();
		I_SOUND.list[m_pHero->m_dwSoundList[SOUND_MOVE]]->Stop();

		if (I_Input.GetKeyCheck(VK_SPACE))
		{
			m_pHero->SetAction(EVENT_JUMP);
			return;
		}

		if (I_Input.GetKeyCheck('R'))
		{
			m_pHero->SetAction(EVENT_RELOAD);
			return;
		}

		if (I_Input.GetKeyCheck(VK_SHIFT))
		{
			m_pHero->SetAction(EVENT_RUN);
			return;
		}

		if (I_Input.KeyCheck(VK_LBUTTON))
		{
			if (m_pHero->GetAmmo())		//총알이 있을시
			{
				m_pHero->SetAction(EVENT_ATTACK);
				return;
			}
			I_SOUND.list[m_pHero->m_dwSoundList[SOUND_DIE]]->Play(I_SOUND.m_pSystem);
		}

		if (I_Input.GetKeyCheck('W'))
		{
			m_pHero->SetAction(EVENT_MOVE);
			return;
		}

		if (I_Input.GetKeyCheck('S'))
		{
			m_pHero->SetAction(EVENT_MOVE);
			return;
		}

		if (I_Input.GetKeyCheck('A'))
		{
			m_pHero->SetAction(EVENT_MOVE);
			return;
		}

		if (I_Input.GetKeyCheck('D'))
		{
			m_pHero->SetAction(EVENT_MOVE);
			return;
		}

	}

	CBY_HeroIdle::CBY_HeroIdle(CBY_Hero* host) : CBY_HeroProcess(host)
	{
	}


	CBY_HeroIdle::~CBY_HeroIdle()
	{
	}
}
