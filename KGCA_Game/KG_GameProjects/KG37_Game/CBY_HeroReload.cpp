#include "CBY_HeroReload.h"
#include "KG_SoundMgr.h"
#include "UI_objMgr.h"

namespace CBY
{
	void CBY_HeroReload::Process()
	{
		if (m_pHero->GetMaxAmmo() <= 0)
		{
			m_pHero->SetAction(EVENT_IDLE);
			return;
		}

		m_pHero->SetState(CHAR_RELOAD);

		SetColDirection();
		if (m_pHero->GetStateAnimationStart(CHAR_RELOAD))
		{
			I_SOUND.list[m_pHero->m_dwSoundList[SOUND_RELOAD]]->EffectPlay(I_SOUND.m_pSystem);
		}

		if (m_pHero->GetStateAnimationEnd(CHAR_RELOAD))
		{
			m_pHero->SetReloading(); //ÀåÅº ¸®·Îµù
			m_pHero->SetAction(EVENT_IDLE);
			UI_MGR->find(UI_TYPE::UI_TYPE_WEAPON_PANEL)->SetType(UI_TYPE::UI_TYPE_AMMO_FONT, UI_VALUE::UI_VALUE_ADD_AMMO, 1, 30);
			UI_MGR->find(UI_TYPE::UI_TYPE_WEAPON_PANEL)->SetType(UI_TYPE::UI_TYPE_AMMO_FONT, UI_VALUE::UI_VALUE_ADD_AMMOMAX, 1, -30);
			return;
		}

		m_vMove = m_pHero->GetHeroPos();
		m_vLook = m_pHero->GetCamera()->m_LookDir;
		m_vSide = m_pHero->GetCamera()->m_SideDir;

		bool bMove;
		CharcterMove(false);

		m_pHero->SetHeroPos(m_vMove);
	}

	CBY_HeroReload::CBY_HeroReload(CBY_Hero* host) : CBY_HeroProcess(host)
	{
	}


	CBY_HeroReload::~CBY_HeroReload()
	{
	}
}

