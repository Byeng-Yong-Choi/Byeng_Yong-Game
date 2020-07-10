#include "CBY_EnemyAttack.h"
#include "KG_SoundMgr.h"
#include "CBY_Hero.h"
#include "CBY_BulletMgr.h"

namespace CBY
{
	void CBY_EnemyAttack::Process()
	{
		m_pEnemy->SetState(CHAR_FIRE);

		//SetColDirection();
		if (m_pEnemy->GetGunMonster())		//원거리
		{
			if (m_pEnemy->GetStateAnimationStart(CHAR_FIRE))
			{
				if (m_pEnemy->m_dwSoundList.size() > 0)
					I_SOUND.list[m_pEnemy->m_dwSoundList[SOUND_ATTACK]]->EffectPlay(I_SOUND.m_pSystem);

				D3DXVECTOR3 shootpos = m_pEnemy->GetFirePos(0);
				//shootpos += (m_pEnemy->GetFireSocketMove().x * m_pEnemy->GetCamera()->m_SideDir);
				//shootpos += (m_pEnemy->GetFireSocketMove().z * m_pEnemy->GetCamera()->m_LookDir);
				//shootpos.y += m_pEnemy->GetFireSocketMove().y;
				CBY::CBY_CHAR_BULLET.BulletMake(m_pEnemy->GetBullet(), shootpos,
					m_LookDir, &m_pEnemy->GetLookWorldMatrix());
			}
			float fLength = D3DXVec3Length(&(m_pEnemy->GetHero()->GetHeroPos()-m_pEnemy->GetHeroPos()));

			if (CheckMonsterHit(false))
			{
				m_pEnemy->SetAction(EVENT_DAMAGE);
				return;
			}

			D3DXVECTOR3 vpos;
			D3DXMATRIX matlook;
			vpos = m_pEnemy->GetHero()->GetCharBox().vCenter - m_pEnemy->GetHeroPos();
			D3DXVec3Normalize(&m_LookDir, &vpos);

			D3DXMatrixLookAtLH(&matlook, &m_pEnemy->GetHero()->GetHeroPos(), &m_pEnemy->GetHeroPos(), &D3DXVECTOR3(0, 1, 0));
			matlook._41 = 0;
			matlook._42 = 0;
			matlook._43 = 0;


			D3DXMatrixInverse(&matlook, NULL, &matlook);
			m_pEnemy->SetLookWorldMatrix(matlook);

			if (fLength > m_pEnemy->GetShootLenght())
			{
				if(m_pEnemy->GetStateAnimationEnd(CHAR_FIRE))
				{
					m_pEnemy->SetAction(EVENT_RUN);
				}
			}

			return;
		}
		else								//근거리
		{
			if (m_pEnemy->GetStateAnimationStart(CHAR_FIRE))
			{
				if (m_pEnemy->m_dwSoundList.size() > 0)
					I_SOUND.list[m_pEnemy->m_dwSoundList[SOUND_ATTACK]]->EffectPlay(I_SOUND.m_pSystem);
			}

			if (m_pEnemy->GetStateAnimationEnd(CHAR_FIRE))
			{
				if (SetColDirection())
				{
					m_pEnemy->GetHero()->SetDamage(1.0f);
				}
				m_pEnemy->SetAction(EVENT_RUN);
			}
		}
	}

	CBY_EnemyAttack::CBY_EnemyAttack(CBY_EnemyAi* host) : CBY_EnemyProcess(host)
	{
	}


	CBY_EnemyAttack::~CBY_EnemyAttack()
	{
	}
}
