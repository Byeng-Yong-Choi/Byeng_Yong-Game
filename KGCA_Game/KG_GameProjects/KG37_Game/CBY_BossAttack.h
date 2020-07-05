#pragma once
#include "CBY_BossAi.h"

namespace CBY
{
	class CBY_BossAttack : public CBY_BossProcess
	{
	public:
		void Process()override;
		float m_fAttackTime;
		bool m_bHit;	//ĳ���� ���� ���� ����
		bool m_bAttck;	//������ ���� ��� ������
		DWORD m_dwState;

	public:
		CBY_BossAttack(CBY_BossAi* host);
		virtual ~CBY_BossAttack();
	private:
		std::shared_ptr<KYS::VFX_Effects> obj1;
	};
}


