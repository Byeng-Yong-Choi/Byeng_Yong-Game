#include "CBY_EnemyMove.h"
#include "CBY_BulletMgr.h"
#include "KG_Collision.h"
#include "MyEffectParser.h"
#include "VFX_ObjMgr.h"

namespace CBY
{
	void CBY_EnemyMove::Process()
	{
		m_pEnemy->SetState(CHAR_MOVE);

		if (CheckMonsterHit(false))
		{
			m_pEnemy->SetAction(EVENT_DAMAGE);
			return;
		}

		D3DXVECTOR3 vmove;
		GetLookDir();
		SetColDirection();
		
		vmove = m_pEnemy->GetHeroPos();
		vmove += m_LookDir * m_pEnemy->GetMoveSpeed() *g_SecondTime;
		//vmove += m_Look * m_pEnemy->GetMoveSpeed() *g_SecondTime;
		m_pEnemy->SetHeroPos(vmove);

		m_fTime += g_SecondTime;
		if (m_fTime > 4.0f)
		{
			m_pEnemy->SetAction(EVENT_IDLE);
			m_fTime = 0.0f;
		}
	}

	CBY_EnemyMove::CBY_EnemyMove(CBY_EnemyAi* host) : CBY_EnemyProcess(host)
	{
	}


	CBY_EnemyMove::~CBY_EnemyMove()
	{
	}
}
