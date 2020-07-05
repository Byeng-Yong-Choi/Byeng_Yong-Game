#pragma once
#include "CBY_BossAi.h"

namespace CBY
{
	class CBY_BossHit : public CBY_BossProcess
	{
	public:
		void Process()override;

	public:
		CBY_BossHit(CBY_BossAi* host);
		virtual ~CBY_BossHit();
	};
}

