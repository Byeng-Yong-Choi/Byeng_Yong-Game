#pragma once
#include "CBY_EnemyAi.h"

namespace CBY
{
	class CBY_EnemyIdle : public CBY_EnemyProcess
	{
	public:
		void Process()override;

	public:
		CBY_EnemyIdle(CBY_EnemyAi* host);
		virtual ~CBY_EnemyIdle();
	};
}
