#include "CBY_BossAttack.h"
#include "CBY_GameMgr.h"
#include "KG_SoundMgr.h"

namespace CBY
{
	void CBY_BossAttack::Process()
	{
		if (m_dwState == BOSS_ATTACK_MOUTH)
		{
			m_fAttackTime += g_SecondTime;
		}

		if (m_fAttackTime > 3.0f && !m_bAttck)
		{
			m_pBoss->SetState(BOSS_ATTACK);
			m_fAttackTime = 0.0f;
			m_dwState = BOSS_ATTACK;
			m_bAttck = true;
		}
		else if(!m_bAttck)
		{
			m_pBoss->SetState(BOSS_ATTACK_MOUTH);
			m_dwState = BOSS_ATTACK_MOUTH;
			m_bAttck = true;
			I_SOUND.list[m_pBoss->m_dwSoundList[SOUND_BOSS_ATTACK_MOUTH]]->Play(I_SOUND.m_pSystem);
		}

		if (m_pBoss->GetStateAnimationEnd(m_dwState))
		{
			m_pBoss->SetAction(EVENT_IDLE);
			m_bAttck = false;
			m_bHit = false;
			return;
		}

		UpdateLook();

		if (m_dwState == BOSS_ATTACK_MOUTH)
		{

		}
		if (m_dwState == BOSS_ATTACK)
		{
			I_SOUND.list[m_pBoss->m_dwSoundList[SOUND_BOSS_ATTACK]]->Play(I_SOUND.m_pSystem);
		}

		if (!m_bHit)
		{
			if (SetColDirection())
			{
				CBY_GAMEMGR.GetCharacter()->SetDamage(1.0f);
				m_bHit = true;
			}
		}
	}

	CBY_BossAttack::CBY_BossAttack(CBY_BossAi* host) : CBY_BossProcess(host)
	{
		m_fAttackTime = 0;
		m_bAttck = false;
		m_dwState = BOSS_ATTACK_MOUTH;
	}


	CBY_BossAttack::~CBY_BossAttack()
	{
	}
}
