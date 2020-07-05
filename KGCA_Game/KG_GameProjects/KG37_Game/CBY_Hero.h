#pragma once
#include "CBY_CharacterSrc.h"
#include "CBY_HeroFSM.h"
namespace KYS
{
	class VFX_Effects;
}
namespace CBY
{
	enum CBY_CRASH
	{
		CRASH_FRONTLEFT=1,
		CRASH_FRONTRIGHT,
		CRASH_BACKLEFT=4,
		CRASH_BACKRIGHT,
	};

	class CBY_EnemyAi;
	class CBY_Hero;

	class CBY_HeroProcess
	{
	protected:
		CBY_Hero* m_pHero;
		D3DXVECTOR3 m_vLook, m_vSide, m_vLookPush, m_vSidePush, m_vMove;
	
	public:
		virtual void SetColDirection();
		virtual int CheckColSize(D3DXVECTOR3& vReflection);
		virtual bool CharcterMove(bool bRun);
		virtual void Process() {};

	public:
		CBY_HeroProcess(CBY_Hero* host) : m_pHero(host)
		{
		};
		~CBY_HeroProcess() {};
	};

	class CBY_Hero : public CBY_CharacterSrc
	{
	protected:
		CBY_HeroProcess* m_Action;
		CBY_HeroProcess* m_ActionList[CHAR_STATECOUNT];
		D3DXVECTOR3 m_vCameraPos;
		D3DXVECTOR3 m_vMovePos;

	protected:
		DWORD m_dwCameraSocket;
		DWORD m_dwAmmoMaxSize;
		DWORD m_dwAmmoSize;
		DWORD m_dwAmmoNum;
		float m_fDamageTime;
		bool m_bStateChange;

	public:
		CBY_EnemyAi* m_pEnemy;
		std::shared_ptr<KYS::VFX_Effects> obj2;

	public:
		virtual bool Init()override;
		virtual bool Frame()override;
		virtual bool Render()override;
		virtual bool Release()override;

	public:
		virtual void InitSet();		//�ʱ�ȭ �Լ�

	public:
		virtual bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context) override;
		virtual void Process();
		virtual void SetAction(DWORD Event);
		virtual void SetState(int i);
		inline virtual void SetCameraSocket(DWORD socket) { m_dwCameraSocket = socket; }
		inline virtual void SetCameraMovePos(D3DXVECTOR3 pos) { m_vCameraPos = pos; }	//ī�޶� ���� ���Ͽ��� �󸶳� �̵� �������� ���ϴ� �Լ�

		//Set
	public:
		virtual void SetReloading();
		void SetAmmoSize(DWORD dwsize);	//�� źâ �� ����
		void SetRayCrahClear();
		inline void SetEnemy(CBY_EnemyAi* enemy) { m_pEnemy = enemy; }
		inline virtual void SetAmmo(DWORD dwUse) { m_dwAmmoNum -= dwUse; }	//�Ѿ� ���縸ŭ ����
		inline virtual void SetMaxAmmo(DWORD dwmax) { m_dwAmmoMaxSize = dwmax; }	//�Ѿ� ���縸ŭ ����

		//Get
	public:
		inline DWORD GetCameraSocket() { return m_dwCameraSocket; }
		inline bool GetStateChange() {return m_bStateChange; }
		virtual D3DXVECTOR3 GetCamerPos();		//ī�޶��� ��ġ�� ��ȯ���� �Լ�
		virtual bool GetAmmo();					//���� �Ѿ��� �ֳ� �˻�
		inline virtual DWORD GetMaxAmmo() { return m_dwAmmoMaxSize; }

	public:
		CBY_Hero();
		virtual ~CBY_Hero();
	};
}

