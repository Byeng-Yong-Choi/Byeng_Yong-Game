#include "CBY_HeroJump.h"
#include "KG_SoundMgr.h"
namespace CBY
{
	void CBY_HeroJump::Process()
	{
		m_pHero->SetState(CHAR_JUMP);

		//SetColDirection();
		

		if (m_pHero->GetStateAnimationEnd(CHAR_JUMP))
		{
			 m_pHero->SetAction(EVENT_IDLE);
			return;
		}

		m_vMove = m_pHero->GetHeroPos();
		m_vLook = m_pHero->GetCamera()->m_LookDir;
		m_vSide = m_pHero->GetCamera()->m_SideDir;

		bool bMove;
		CharcterMove(false);

		m_pHero->SetHeroPos(m_vMove);
	}

	CBY_HeroJump::CBY_HeroJump(CBY_Hero* host) : CBY_HeroProcess(host)
	{
	}


	CBY_HeroJump::~CBY_HeroJump()
	{
	}
}
