#pragma once
#include "CBY_BossAi.h"

namespace CBY
{
	class CBY_BossWalk : public CBY_BossProcess
	{
	public:
		void Process()override;

	public:
		CBY_BossWalk(CBY_BossAi* host);
		virtual ~CBY_BossWalk();
	};
}
