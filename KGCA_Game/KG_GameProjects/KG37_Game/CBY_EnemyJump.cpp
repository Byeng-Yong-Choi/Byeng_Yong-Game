#include "CBY_EnemyJump.h"
#include "CBY_BulletMgr.h"
#include "KG_Collision.h"
#include "MyEffectParser.h"
#include "VFX_ObjMgr.h"

namespace CBY
{
	void CBY_EnemyJump::Process()
	{
		m_pEnemy->SetState(CHAR_RELOAD);

		if (CheckMonsterHit(false))
		{
			m_pEnemy->SetAction(EVENT_DAMAGE);
			return;
		}

		if (m_pEnemy->GetStateAnimationEnd(CHAR_RELOAD))
		{
			m_pEnemy->SetAction(EVENT_IDLE);
		}
	}

	CBY_EnemyJump::CBY_EnemyJump(CBY_EnemyAi* host) : CBY_EnemyProcess(host)
	{
	}


	CBY_EnemyJump::~CBY_EnemyJump()
	{
	}
}
