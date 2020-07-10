#pragma once
#include "CBY_Object.h"

namespace CBY
{
	class CBY_Bullet : public CBY_Object
	{
	private:
		float m_fSpeed;			//총알의 속도
		int m_iPower;			//총알의 세기
		D3DXVECTOR3 m_vAmmoDir;		//총알이 날아갈 방향
		D3DXVECTOR3 m_vAmmoPos;		//총알의 위치
		D3DXMATRIX m_matAmmoWorld;	//처음 쏠때 얻을 회전값이 들어있는 행렬
		bool m_bEnemyBullet;

	public:
		bool Init();
		bool Frame();

	public:
		virtual void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj) override;

	public:
		inline void SetSpeed(float fspeed){m_fSpeed = fspeed;}
		inline void SetPower(int i){m_iPower = i;}
		inline void SetPos(D3DXVECTOR3 pos){m_vAmmoPos = pos;}
		inline void SetDirection(D3DXVECTOR3 dir){m_vAmmoDir = dir;}
		inline void SetWorld(D3DXMATRIX world){m_matAmmoWorld = world;}
		inline void SetEnemy(bool ene) { m_bEnemyBullet = ene; }

	public:
		inline int GetPower(){return m_iPower;}
		inline D3DXVECTOR3 GetPos(){return m_vAmmoPos;}
		inline D3DXVECTOR3 GetDir() { return m_vAmmoDir; }
		inline bool GetEnemy() { return m_bEnemyBullet; }

	public:
		CBY_Bullet();
		virtual ~CBY_Bullet();
	};
}

