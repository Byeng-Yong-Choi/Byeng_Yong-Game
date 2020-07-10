#pragma once
#include "CBY_CharacterSrc.h"
#include "CBY_EnemyFSM.h"
#include <stdlib.h>
#include <time.h>

namespace KYS
{
	class VFX_Effects;
}

namespace CBY
{
	class CBY_Hero;
	class CBY_EnemyAi;

	class CBY_EnemyProcess
	{
	protected:
		CBY_EnemyAi* m_pEnemy;
		D3DXVECTOR3 m_vLook, m_vSide, m_vLookPush, m_vSidePush, m_vMove;
		D3DXVECTOR3 m_LookDir;
		D3DXVECTOR3 m_SideDir;
		float m_fYaw, m_fPitch, m_fRoll;
		float m_fDirTime;		//�𷺼��� �ٲ� �ð�
		float m_fTime;			//������Ʈ�� ��ȯ �ð�

	public:
		virtual bool SetColDirection();
		virtual D3DXVECTOR3 GetLookDir();
		virtual bool CheckMonsterHit(bool bDamage);
		
		virtual void Process() {};

	public:
		CBY_EnemyProcess(CBY_EnemyAi* host) : m_pEnemy(host)
		{
			m_fYaw = 0.0f;
			m_fPitch = 0.0f;
			m_fRoll = 0.0f;
			m_fTime = 0.0f;
			m_fDirTime = 3.0f;
		};
		~CBY_EnemyProcess() {};
	};

	class CBY_EnemyAi:public CBY_CharacterSrc
	{
	private:
		CBY_EnemyProcess* m_Action;
		CBY_EnemyProcess* m_ActionList[CHAR_STATECOUNT];
		bool m_bDie;

	protected:
		D3DXVECTOR3 m_LookDir;
		D3DXMATRIX m_matLook;
		float m_fYaw;
		float m_fEffetLookVal;	//������ �� ����Ʈ�� �� ������ ���ͼ� ���� �Ÿ�
		bool m_bGunMonster;		//���Ÿ����� �Ǻ�
		int m_iIndex;			//�ڽ��� ���� ��ȣ
		float m_fShootLenght;	//���� ��Ÿ�

	public:
		CBY_Hero* m_pHero;
		D3DXMATRIX m_matWorld;


	public:
		std::shared_ptr<KYS::VFX_Effects> obj2;

	public:
		virtual bool Init()override;
		virtual bool Frame()override;
		virtual bool Render()override;
		virtual bool Release()override;

	public:
		virtual bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context) override;
		virtual void Process();
		virtual void SetAction(DWORD Event);
		virtual void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)override;

	public:
		inline virtual void SetEffectVal(float f)//������ �� ����Ʈ�� �� ������ ���ͼ� ���� �Ÿ�
		{m_fEffetLookVal = f;}
		inline virtual void SetHero(CBY_Hero* hero){m_pHero = hero;}
		inline virtual void SetYaw(float yaw) { m_fYaw = yaw; }
		inline virtual void SetLookDir(D3DXVECTOR3 lookdir) { m_LookDir = lookdir; }
		inline virtual void SetLookWorldMatrix(D3DXMATRIX matlook) { m_matLook = matlook; }
		inline virtual void SetIndex(int index) { m_iIndex = index; }
		inline virtual void SetGunMonster(bool gun) { m_bGunMonster = gun; }
		inline virtual void SetShootLenght(float gun) { m_fShootLenght = gun; }
		inline virtual void SetDie(bool bDie) { m_bDie = bDie; }
		

	public:
		inline virtual float GetEffectVal()//������ �� ����Ʈ�� �� ������ ���ͼ� ���� �Ÿ�
		{
			return m_fEffetLookVal;
		}
		inline virtual CBY_Hero* GetHero(){return m_pHero;}
		inline virtual D3DXVECTOR3 GetLookDir(){ return m_LookDir; }
		inline virtual D3DXMATRIX GetLookWorldMatrix() { return m_matLook; }
		inline virtual float GetYaw() { return m_fYaw; }
		inline virtual int GetIndex() { return m_iIndex; }
		inline virtual bool GetGunMonster() { return m_bGunMonster; }
		inline virtual float GetShootLenght() { return m_fShootLenght; }
		inline virtual bool GetDie() { return m_bDie; }

	public:
		CBY_EnemyAi();
		virtual ~CBY_EnemyAi();
	};
}

