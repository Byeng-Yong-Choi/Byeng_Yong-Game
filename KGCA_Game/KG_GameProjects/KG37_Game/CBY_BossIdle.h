#pragma once
#include "CBY_BossAi.h"

namespace CBY
{
	class CBY_BossIdle : public CBY_BossProcess
	{
	public:
		void Process()override;

	public:
		float m_fTime;

	public:
		CBY_BossIdle(CBY_BossAi* host);
		virtual ~CBY_BossIdle();
	};
}
