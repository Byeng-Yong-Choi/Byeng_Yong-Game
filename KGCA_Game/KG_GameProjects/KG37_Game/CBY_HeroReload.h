#pragma once

#include "CBY_Hero.h"

namespace CBY
{
	class CBY_HeroReload : public CBY_HeroProcess
	{
	public:
		void Process()override;

	public:
		CBY_HeroReload(CBY_Hero* host);
		virtual ~CBY_HeroReload();
	};
}

