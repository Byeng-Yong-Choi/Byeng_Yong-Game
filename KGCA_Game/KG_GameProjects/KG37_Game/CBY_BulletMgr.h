#pragma once
#include "CBY_BulletQueue.h"
#include "KG_Select.h"

namespace KYS
{
	class VFX_EffectObj;
	class VFX_Effects;
}

namespace CBY
{
	class CBY_Hero;
	typedef std::vector<CBY_Bullet*>::iterator Bulletiter;

	class CBY_BulletMgr :public Singleton<CBY_BulletMgr>
	{
	private:
		std::vector<CBY_Bullet*> m_pBulletList;
		CBY_BulletQueue m_BulletQueue;
		D3DXMATRIX m_matWorld;
		D3DXMATRIX m_matView;
		D3DXMATRIX m_matProj;
		KG_Select m_Select;
		CBY_Hero* m_pHero;

	//effect
		std::shared_ptr<KYS::VFX_Effects> obj1;
		
	public:
		bool Init();
		bool Frame();
		bool Render();
		bool Release();

	public:
		void BulletMake(CBY_Bullet* weapon, D3DXVECTOR3 vpos, D3DXVECTOR3 vdir,D3DXMATRIX* world);
		void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
		Bulletiter BulletPop(Bulletiter iweapon);
		void BulletPop(int iweapon);

	public:
		inline void SetHero(CBY_Hero* Hero) { m_pHero = Hero; }

	public:
		inline int GetBulletNum(){return m_pBulletList.size();}
		inline bool GetEnemyBullet(int i) { return m_pBulletList[i]->GetEnemy(); }
		inline KG_Box GetBulletBox(int i){return m_pBulletList[i]->GetCharBox();}

	public:
		CBY_BulletMgr();
		virtual ~CBY_BulletMgr();
	};

#define CBY_CHAR_BULLET CBY_BulletMgr::GetInstance()
}

