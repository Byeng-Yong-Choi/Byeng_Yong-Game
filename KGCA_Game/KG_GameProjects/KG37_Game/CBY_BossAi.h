#pragma once
#include "CBY_BossFSM.h"
#include "CBY_EnemyAi.h"

namespace KYS
{
	class VFX_Effects;
}

namespace CBY
{
	class CBY_Hero;
	class CBY_BossAi;

	class CBY_BossProcess
	{
	protected:
		CBY_BossAi* m_pBoss;
		D3DXVECTOR3 m_vLook, m_vSide, m_vLookPush, m_vSidePush, m_vMove;
		D3DXVECTOR3 m_LookDir;
		D3DXVECTOR3 m_SideDir;
		float m_fYaw, m_fYawAfter, m_fPitch, m_fRoll;
		float m_fDirTime;		//�𷺼��� �ٲ� �ð�
		float m_fTime;			//������Ʈ�� ��ȯ �ð�
		float m_fSkillTime;

	public:

		virtual bool SetColDirection();
		virtual D3DXVECTOR3 GetLookDir();
		virtual void UpdateLook();
		virtual void BossWalk();

		virtual void Process() {};

	public:
		CBY_BossProcess(CBY_BossAi* host) : m_pBoss(host)
		{
			m_fYawAfter = 0.0f;
			m_fYaw = 0.0f;
			m_fPitch = 0.0f;
			m_fRoll = 0.0f;
			m_fTime = 0.0f;
			m_fDirTime = 3.0f;
			m_fSkillTime = 0.0f;
		};
		~CBY_BossProcess() {};
	};

	class CBY_BossAi :public CBY_EnemyAi
	{
	private:
		CBY_BossProcess* m_Action;
		CBY_BossProcess* m_ActionList[BOSS_STATE_COUNT];

	private:
		bool m_bSleep;				//���� ��� ����
		bool m_bFly;			//Boss Fly State
		float m_fRadian;		//������ ���� �Ÿ��񱳿� ��
		float m_fSkill;
		KGCA37_Ray m_FlameRay;


	public:
		std::shared_ptr<KYS::VFX_Effects> obj;
		std::shared_ptr<KYS::VFX_Effects> obj2;
		std::shared_ptr<KYS::VFX_Effects> obj3;

	public:
		virtual bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)override;
		virtual void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)override;
		virtual void SetAction(DWORD Event) override;
		virtual void Process()override;
		virtual bool Init()override;
		virtual bool Frame()override;
		virtual bool Render()override;
		virtual bool Release()override;

	public:
		void BossSleepState();
		bool CheckMonsterHit(bool bDamage);
		bool CheckFlameHit();

	public:
		inline void SetSleep(bool bSleep) { m_bSleep = bSleep; }
		inline void SetFly(bool bFly) { m_bFly = bFly; }
		inline void SetRadian(float fDis) { m_fRadian = fDis; }
		inline void SetSkillTime(float fskill) { m_fSkill = fskill; }

	public:
		inline bool GetSleep() { return m_bSleep; }
		inline bool GetFly() { return m_bFly; }
		inline float GetRadian() { return m_fRadian; }
		inline float GetSkillTime() { return m_fSkill; }
		inline KGCA37_Ray GetFlameRay() { return m_FlameRay; }

	public:
		CBY_BossAi();
		virtual ~CBY_BossAi();
	};
}

