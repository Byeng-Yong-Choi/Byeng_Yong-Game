#pragma once
#include "CBY_BossAi.h"

namespace CBY
{
	class CBY_BossAttack : public CBY_BossProcess
	{
	public:
		void Process()override;
		float m_fAttackTime;
		bool m_bHit;	//캐릭터 공격 성공 유무
		bool m_bAttck;	//보스가 공격 모션 중인지
		DWORD m_dwState;

	public:
		CBY_BossAttack(CBY_BossAi* host);
		virtual ~CBY_BossAttack();
	private:
		std::shared_ptr<KYS::VFX_Effects> obj1;
	};
}


