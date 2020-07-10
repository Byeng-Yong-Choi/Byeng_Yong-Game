#include "CBY_EnemyIdle.h"
#include "CBY_BulletMgr.h"
#include "KG_Collision.h"
#include "MyEffectParser.h"
#include "VFX_ObjMgr.h"
#include "CBY_Hero.h"
#include <stdlib.h> 

namespace CBY
{
	void CBY_EnemyIdle::Process()
	{
		m_pEnemy->SetState(CHAR_IDLE);
	
		if (CheckMonsterHit(false))
		{
			m_pEnemy->SetAction(EVENT_DAMAGE);
			m_fTime = 0.0f;
			return;
		}

		float fLength = D3DXVec3Length(&(m_pEnemy->GetHero()->GetHeroPos() - m_pEnemy->GetHeroPos()));
		if (fLength <= m_pEnemy->GetShootLenght()*1.5f)
		{
			m_pEnemy->SetAction(EVENT_RUN);
		}


		/*if (SetColDirection())
		{
			return;
		}*/

		//if (m_fTime > 2.0f)
		//{
		//	srand((unsigned int)time(NULL));
		//	int iCase = (rand()*m_pEnemy->GetIndex()) % 10;
		//	
		//	if (iCase < 7)
		//	{
		//		m_pEnemy->SetAction(EVENT_MOVE);
		//	}
		//	else if (iCase == 8)
		//	{
		//		m_fTime = 0.0f;
		//		return;
		//	}
		//	else
		//	{
		//		m_pEnemy->SetAction(CHAR_JUMP);
		//	}
		//	m_fTime = 0.0f;
		//}
		//else
		//{
		//	m_fTime += g_SecondTime;
		//}
	}

	CBY_EnemyIdle::CBY_EnemyIdle(CBY_EnemyAi* host) : CBY_EnemyProcess(host)
	{
	}


	CBY_EnemyIdle::~CBY_EnemyIdle()
	{
	}
}
