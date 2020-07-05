#include "CBY_BulletMgr.h"
#include "KG_Collision.h"
#include "MyEffectParser.h"
#include "VFX_ObjMgr.h"
#include "JH_MapMgr.h"
#include "CBY_Hero.h"

namespace CBY
{
	bool CBY_BulletMgr::Init()
	{
		for (int i = 0; i < BULLET_NUM; i++)
		{
			CBY_Bullet* wea = new CBY_Bullet;
			m_BulletQueue.Push(wea);
		}
		return true;
	}

	bool CBY_BulletMgr::Frame()
	{
		for (int i = 0; i < m_pBulletList.size(); i++)
		{
			m_pBulletList[i]->Frame();

			KG_Box AmmoBox = m_pBulletList[i]->GetCharBox();

			if (m_pBulletList[i]->GetEnemy())
			{
				if (KG_COLLOSION::ChkOBBToOBB(m_pHero->GetCharBox(), AmmoBox))
				{
					m_pHero->SetDamage(m_pBulletList[i]->GetPower());
					BulletPop(i);
					i--;
					continue;
				}
			}

			for (int ibox = 0; ibox < JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->m_DrawObjectList.size(); ibox++)
			{
				DWORD dwFlag = JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->m_DrawObjectList[ibox]->GetFlag();
				if (dwFlag == JH::WALL)
				{
					if (KG_COLLOSION::ChkOBBToOBB(
						JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->m_DrawObjectList[ibox]->GetObj()->GetCharBox(),
						AmmoBox))
					{
						D3DXVECTOR3 zpos = AmmoBox.vCenter;
						//JH::I_MapMgr.GetCurrentMap()->).m_pQuadTree->m_ObjectList[ibox]->GetObj()->GetCharBox().vCenter;
						//zpos.y = 
						//JH::I_MapMgr.GetCurrentMap()->).m_pQuadTree->m_ObjectList[ibox]->GetObj()->GetCharBox().fExtent[1]/2.0f;
						//zpos += m_pBulletList[i]->GetDir() *
						//-(JH::I_MapMgr.GetCurrentMap()->).m_pQuadTree->m_ObjectList[ibox]->GetObj()->GetCharBox().fExtent[2]);
						//obj1 = VFX_MGR->find(VFX_EFFECT_TYPE::VFX_EFFECT_CHARACTER_BLOOD_IMPACT);
						//obj1->SetParentMatWorld(JH::I_MapMgr.GetCurrentMap()->).m_pQuadTree->m_ObjectList[ibox]->GetObj()->m_matWorld);
						//obj1->SetScale(D3DXVECTOR3(5, 5, 0));
						//zpos = D3DXVECTOR3(0, 0, 0);
						//obj1->Execute(zpos);

						BulletPop(i);
						i--;
						break;
					}
				}
			}
		
		}
		return true;
	}

	bool CBY_BulletMgr::Render()
	{
		for (int i = 0; i < m_pBulletList.size(); i++)
		{
			m_pBulletList[i]->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
			m_pBulletList[i]->Render();
		}
		return true;
	}

	bool CBY_BulletMgr::Release()
	{
		for (int i = 0; i < m_pBulletList.size();)
		{
			m_pBulletList[0]->Release();
			Bulletiter iter = m_pBulletList.begin();
			BulletPop(iter);
		}
		m_pBulletList.clear();
		m_BulletQueue.Release();
		m_pHero = nullptr;

		return true;
	}

	void CBY_BulletMgr::SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
	{
		if (world != nullptr)
		{
			m_matWorld = *world;
		}

		if (view != nullptr)
		{
			m_matView = *view;
		}

		if (proj != nullptr)
		{
			m_matProj = *proj;
		}

		m_Select.SetMarix(nullptr, view, proj);
	}

	void CBY_BulletMgr::BulletMake(CBY_Bullet* weapon, D3DXVECTOR3 vpos, D3DXVECTOR3 vdir,D3DXMATRIX* world)
	{
		CBY_Bullet* wea = m_BulletQueue.Pop();
		*wea = *weapon;
		wea->SetPos(vpos);
		//wea->SetDirection(m_Select.m_Ray.m_Dir);
		wea->SetDirection(vdir);
		wea->SetWorld(*world);
		wea->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
		m_pBulletList.push_back(wea);
	}

	Bulletiter CBY_BulletMgr::BulletPop(Bulletiter iweapon)
	{
		Bulletiter wea;
		m_BulletQueue.Push(*iweapon);
		wea = m_pBulletList.erase(iweapon);
		return wea;
	}

	void CBY_BulletMgr::BulletPop(int iweapon)
	{
		//m_pBulletList[iweapon]->Release();
		Bulletiter iter = m_pBulletList.begin() + iweapon;
		m_BulletQueue.Push(*iter);
		m_pBulletList.erase(iter);
	}


	CBY_BulletMgr::CBY_BulletMgr()
	{
		D3DXMatrixIdentity(&m_matWorld);
		D3DXMatrixIdentity(&m_matView);
		D3DXMatrixIdentity(&m_matProj);
		m_pHero = nullptr;
	}


	CBY_BulletMgr::~CBY_BulletMgr()
	{
		
	}
}