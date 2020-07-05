#include "CBY_HeroMove.h"
#include "KG_SoundMgr.h"
#include "JH_MapMgr.h"
namespace CBY
{
	void CBY_HeroMove::Process()
	{
		m_pHero->SetState(CHAR_MOVE);

		//SetColDirection();
		//I_SOUND.list[m_pHero->m_dwSoundList[SOUND_MOVE]]->Play(I_SOUND.m_pSystem);
		if (m_pHero->GetStateAnimationEnd(CHAR_MOVE))
		{
			I_SOUND.list[m_pHero->m_dwSoundList[SOUND_MOVE]]->EffectPlay(I_SOUND.m_pSystem);
		}

		/*if (m_pHero->GetStateAnimationEnd(CHAR_MOVE))
		{
			I_SOUND.list[m_pHero->m_dwSoundList[SOUND_MOVE]]->EffectPlay(I_SOUND.m_pSystem);
		}*/
	
		bool bMove = false;

		if (I_Input.KeyCheck(VK_LBUTTON))
		{
			if (m_pHero->GetAmmo())		//총알이 있을시
			{
				m_pHero->SetAction(EVENT_ATTACK);
				return;
			}
			I_SOUND.list[m_pHero->m_dwSoundList[SOUND_DIE]]->Play(I_SOUND.m_pSystem);
		}

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

		m_vMove = m_pHero->GetHeroPos();
		m_vLook = m_pHero->GetCamera()->m_LookDir;
		m_vSide = m_pHero->GetCamera()->m_SideDir;

		SetColDirection();
		D3DXVECTOR3 vBeginMove = m_vMove;

		bMove = CharcterMove(false);
		
		int iColSize = 0;

		D3DXVECTOR3 vRef = D3DXVECTOR3(0, 0, 0);
		iColSize = CheckColSize(vRef);

		if (bMove)
		{
			if (iColSize > 1)
			{
				m_pHero->SetHeroPos(vBeginMove+ vRef);
			}
			else
			{
				m_pHero->SetHeroPos(m_vMove+ vRef);
			}
		}
		else
		{
			m_pHero->SetAction(EVENT_IDLE);
		}
	}

	CBY_HeroMove::CBY_HeroMove(CBY_Hero* host) : CBY_HeroProcess(host)
	{
	}


	CBY_HeroMove::~CBY_HeroMove()
	{
	}
}
