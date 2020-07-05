#include "CBY_EnemyRun.h"
#include "CBY_BulletMgr.h"
#include "KG_Collision.h"
#include "MyEffectParser.h"
#include "VFX_ObjMgr.h"
#include "CBY_Hero.h"

namespace CBY
{
	void CBY_EnemyRun::Process()
	{
		m_pEnemy->SetState(CHAR_RUN);

		if (CheckMonsterHit(false))
		{
			m_pEnemy->SetAction(EVENT_DAMAGE);
			return;
		}

		D3DXVECTOR3 vmove, vpos;
		D3DXMATRIX matlook;
		vpos = m_pEnemy->GetHero()->GetHeroPos() - m_pEnemy->GetHeroPos();
		D3DXVec3Normalize(&m_LookDir, &vpos);

		D3DXMatrixLookAtLH(&matlook, &m_pEnemy->GetHero()->GetHeroPos(), &m_pEnemy->GetHeroPos(), &D3DXVECTOR3(0, 1, 0));
		//m_LookDir = -(m_pEnemy->GetCamera()->m_LookDir);
		//m_SideDir = -(m_pEnemy->GetCamera()->m_SideDir);
		matlook._41 = 0;
		matlook._42 = 0;
		matlook._43 = 0;

		if (m_pEnemy->GetGunMonster())
		{
			float fLength = D3DXVec3Length(&(m_pEnemy->GetHero()->GetHeroPos() - m_pEnemy->GetHeroPos()));
			if (fLength <= m_pEnemy->GetShootLenght())
			{
				m_pEnemy->SetAction(EVENT_ATTACK);
				return;
			}
		}
		else
		{
			if (SetColDirection())
			{
				m_pEnemy->SetAction(EVENT_ATTACK);
				return;
			}
		}

		vmove = m_pEnemy->GetHeroPos();
		vmove += m_LookDir * (m_pEnemy->GetMoveSpeed() + 5) *g_SecondTime;


		m_pEnemy->SetYaw(m_fYawy);
		m_pEnemy->SetLookDir(m_LookDir);
		m_pEnemy->SetHeroPos(vmove);

		D3DXMatrixInverse(&matlook, NULL, &matlook);
		m_pEnemy->SetLookWorldMatrix(matlook);

	}

	CBY_EnemyRun::CBY_EnemyRun(CBY_EnemyAi* host) : CBY_EnemyProcess(host)
	{
		m_fYawy = 0.0;
	}


	CBY_EnemyRun::~CBY_EnemyRun()
	{
	}
}
