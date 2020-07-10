#pragma once
#include "CBY_BossAi.h"

namespace CBY
{
	class CBY_BossFlyLand : public CBY_BossProcess
	{
	public:
		void Process()override;

	public:
		CBY_BossFlyLand(CBY_BossAi* host);
		virtual ~CBY_BossFlyLand();
	};
}

