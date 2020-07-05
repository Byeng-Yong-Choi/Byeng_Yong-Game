#include "CBY_EnemyReload.h"


namespace CBY
{
	void CBY_EnemyReload::Process()
	{
		m_pEnemy->SetState(CHAR_DEATH);
		if (m_pEnemy->GetStateAnimationEnd(CHAR_DEATH))
		{
			m_pEnemy->SetEndClip(CHAR_DEATH, true);
		}
	}

	CBY_EnemyReload::CBY_EnemyReload(CBY_EnemyAi* host) : CBY_EnemyProcess(host)
	{
	}


	CBY_EnemyReload::~CBY_EnemyReload()
	{
	}
}

