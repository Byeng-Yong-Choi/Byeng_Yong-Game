#include "CBY_EnemyDamage.h"
#include "CBY_BulletMgr.h"
#include "KG_Collision.h"
#include "MyEffectParser.h"
#include "VFX_ObjMgr.h"
#include "KG_SoundMgr.h"

namespace CBY
{
	void CBY_EnemyDamage::Process()
	{
		m_pEnemy->SetState(CHAR_DAMAGE);
		if (m_pEnemy->GetStateAnimationStart(CHAR_DAMAGE))
		{
			if(m_pEnemy->m_dwSoundList.size()>0)
			I_SOUND.list[m_pEnemy->m_dwSoundList[SOUND_HIT]]->EffectPlay(I_SOUND.m_pSystem);

			D3DXVECTOR3 zpos;
			D3DXMATRIX mat;
			
			m_pEnemy->obj2 = VFX_MGR->find(VFX_EFFECT_TYPE::VFX_EFFECT_CHARACTER_BLOOD_IMPACT);
			m_pEnemy->obj2->setParticleScale(D3DXVECTOR3(10, 10, 0));
			mat = m_pEnemy->m_matWorld;
			zpos = D3DXVECTOR3(0, 0, 0);
			zpos.y += 5;
			m_pEnemy->obj2->Execute(&mat, zpos);
		}

		if(m_pEnemy->GetStateAnimationEnd(CHAR_DAMAGE))
		{
			int hp = m_pEnemy->GetHP();
			hp -= 1;
			m_pEnemy->SetHP(hp);

			if (hp <= 0)
			{
				m_pEnemy->SetAction(EVENT_DEATH);
				return;
			}
			m_pEnemy->SetAction(EVENT_RUN);
			return;
		}

		CheckMonsterHit(true);
	}

	CBY_EnemyDamage::CBY_EnemyDamage(CBY_EnemyAi* host) : CBY_EnemyProcess(host)
	{
	}


	CBY_EnemyDamage::~CBY_EnemyDamage()
	{
	}
}
