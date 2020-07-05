#include "CBY_BossFlame.h"
#include "VFX_ObjMgr.h"
#include "CBY_GameMgr.h"
#include"KG_SoundMgr.h"

namespace CBY
{
	void CBY_BossFlame::Process()
	{
		m_pBoss->SetState(BOSS_FLAME);
		D3DXMATRIX mat;
		D3DXVECTOR3 shootpos;
		if (m_pBoss->GetStateAnimationStart(BOSS_FLAME))
		{
			shootpos = m_pBoss->GetHeroPos();
			m_pBoss->obj3->setParticleScale(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
			m_pBoss->obj3->setParticleLifeTimeLimit(2.0f);
			m_pBoss->obj3->setParticleSpeed(3.0f);

			mat = m_pBoss->GetLookWorldMatrix();
			mat._41 = shootpos.x;
			mat._42 = shootpos.y+0.5f;
			mat._43 = shootpos.z;
			m_pBoss->obj3->Execute(&mat);
		}

		if (m_pBoss->GetStateAnimationEnd(BOSS_FLAME))
		{
			
			if (!m_bFlame)
			{
				m_pBoss->SetEndClip(BOSS_FLAME, true);
				shootpos = m_pBoss->GetFirePos(0);
				mat = m_pBoss->GetLookWorldMatrix();
				m_pBoss->obj->setParticleScale(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
				m_pBoss->obj->setParticleLifeTimeLimit(2.0f);
				m_pBoss->obj->setParticleSpeed(3.0f);

				

				m_pBoss->obj2->setParticleScale(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
				m_pBoss->obj2->setParticleLifeTimeLimit(2.0f);
				m_pBoss->obj2->setParticleSpeed(3.0f);

				//D3DXMatrixRotationY(&mat, 180.0f);
				mat._41 = shootpos.x;
				mat._42 = shootpos.y;
				mat._43 = shootpos.z;
				m_pBoss->obj->Execute(&mat);
				m_pBoss->obj->SetRotate(D3DXVECTOR3(0, 180.0f, 0));

				mat = m_pBoss->GetLookWorldMatrix();
				D3DXMatrixRotationY(&mat, 180.0f);
				mat._41 = shootpos.x;
				mat._42 = shootpos.y;
				mat._43 = shootpos.z;
				m_pBoss->obj2->Execute(&mat);

				m_bFlame = true;
			}
			else
			{
				m_fPlayerHit += g_SecondTime;
				if (m_fPlayerHit>=1.0f)
				{
					if (m_pBoss->CheckFlameHit())
					{
						CBY_GAMEMGR.GetCharacter()->SetDamage(1.0f);
						m_fPlayerHit = 0.0f;
					}
				}
				/*shootpos = D3DXVECTOR3(0, 0, 1);
				D3DXVec3TransformCoord(&shootpos, &shootpos, &m_pBoss->GetLookWorldMatrix());
				m_vBallPos += shootpos * g_SecondTime * 10;
				m_pBoss->obj2->setParticlePos(m_vBallPos);*/

			}
		}
		m_fTime += g_SecondTime;
		if (m_fTime > m_pBoss->GetSkillTime())
		{
			m_pBoss->obj->Shutdown();
			m_pBoss->obj2->Shutdown();
			m_pBoss->obj3->Shutdown();
			m_pBoss->SetAction(EVENT_IDLE);
			m_pBoss->SetEndClip(BOSS_FLAME, false);
			m_bFlame = false;
			m_fTime = 0.0;
			m_fPlayerHit = 1.0f;
		}

		I_SOUND.list[m_pBoss->m_dwSoundList[SOUND_BOSS_FLAME]]->Play(I_SOUND.m_pSystem);
	}

	CBY_BossFlame::CBY_BossFlame(CBY_BossAi* host) : CBY_BossProcess(host)
	{
		m_bFlame = false;
		m_vBallPos = D3DXVECTOR3(0, 0, 0);
		m_fTime = 0.0f;
		m_fPlayerHit = 1.0f;
	}


	CBY_BossFlame::~CBY_BossFlame()
	{
	}
}
