#include "CBY_BossWalk.h"
#include "CBY_GameMgr.h"
#include"KG_SoundMgr.h"

namespace CBY
{
	void CBY_BossWalk::Process()
	{
		if (m_pBoss->GetFly())
		{
			m_pBoss->SetState(BOSS_FLY_WALK);
		}
		else
		{
			m_pBoss->SetState(BOSS_WALK);
		}

		UpdateLook();
		BossWalk();

		I_SOUND.list[m_pBoss->m_dwSoundList[SOUND_BOSS_MOVES]]->Play(I_SOUND.m_pSystem);

		D3DXVECTOR3 pos = m_pBoss->GetHeroPos() - CBY_GAMEMGR.GetCharacter()->GetHeroPos();
		float fDistance = D3DXVec3Length(&pos);

		if (fDistance <= m_pBoss->GetRadian()/2.0f)
		{
			m_pBoss->SetAction(EVENT_ATTACK);
		}

		m_fTime += g_SecondTime;
		if (m_fTime > 8.0f)
		{
			m_pBoss->SetAction(EVENT_IDLE);
			m_fTime = 0.0f;
		}
	}

	CBY_BossWalk::CBY_BossWalk(CBY_BossAi* host) : CBY_BossProcess(host)
	{
	}


	CBY_BossWalk::~CBY_BossWalk()
	{
	}
}