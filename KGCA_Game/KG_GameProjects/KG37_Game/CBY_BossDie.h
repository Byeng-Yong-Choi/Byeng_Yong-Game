#pragma once
#include "CBY_BossAi.h"

namespace CBY
{
	class CBY_BossDie : public CBY_BossProcess
	{
	public:
		void Process()override;

	public:
		CBY_BossDie(CBY_BossAi* host);
		virtual ~CBY_BossDie();
	};
}


