#pragma once
#include "CBY_EnemyAi.h"

namespace CBY
{
	class CBY_EnemyReload : public CBY_EnemyProcess
	{
	public:
		void Process()override;

	public:
		CBY_EnemyReload(CBY_EnemyAi* host);
		virtual ~CBY_EnemyReload();
	};
}

