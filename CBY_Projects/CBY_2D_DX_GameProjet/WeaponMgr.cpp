#include "WeaponMgr.h"
#include "C_EffectMgr.h"
#include "MonsterMgr.h"

bool WeaponMgr::Init()
{
	for (int i = 0; i < WEAPON_EFFECT_NUMBER; i++)
	{
		TowerWeapon* wea = new TowerWeapon;
		m_WeaponQueue.Push(wea);
	}
	return true;
}

bool WeaponMgr::Frame()
{
	for (int i = 0; i < m_pWeaponList.size(); i++)
	{
		m_pWeaponList[i]->Frame();
	}
	return true;
}

bool WeaponMgr::Render()
{
	for (int i = 0; i < m_pWeaponList.size(); i++)
	{
		m_pWeaponList[i]->SetWorldandMatrix(nullptr, &m_matView, &m_matProj);
		m_pWeaponList[i]->Render();
	}
	return true;
}

bool WeaponMgr::Release()
{
	/*for (int i = 0; i < m_pWeaponList.size(); i++)
	{
		m_pWeaponList[i]->Release();
	}*/
	m_pWeaponList.clear();
	m_WeaponQueue.Release();

	return true;
}

void WeaponMgr::SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
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
}

void WeaponMgr::WeaponTargetPositionSet(TowerWeapon* weapon, D3DXVECTOR3 pos)
{
	weapon->SetMonsterPosition(pos);
}

void WeaponMgr::WeaponMake(TowerWeapon* weapon, D3DXMATRIX* world,int imon, bool blv)
{
	TowerWeapon* wea = m_WeaponQueue.Pop();
	*wea = *weapon;
	wea->SetMonsterNum(imon);
	wea->m_World = *world;
	wea->bLV = blv;
	D3DXVECTOR3 pos = D3DXVECTOR3(wea->m_World._41, wea->m_World._42, wea->m_World._44);
	wea->RectSet(pos);
	m_pWeaponList.push_back(wea);
}

Weaiter WeaponMgr::WeaponPop(Weaiter iweapon)
{
	Weaiter wea;
	m_WeaponQueue.Push(*iweapon);
	wea = m_pWeaponList.erase(iweapon);
	return wea;
}

void WeaponMgr::WeaponPop(int iweapon)
{
	Weaiter iter = m_pWeaponList.begin() + iweapon;
	m_WeaponQueue.Push(*iter);
	m_pWeaponList.erase(iter);
}

void WeaponMgr::Process(std::shared_ptr<C_Tower>& tower)
{
	if (m_pWeaponList.size() > 0)							//날아가는 미사일이 존재할때 타겟을 향해 날아가게 위치 수정과 삭제를 위한 작업
	{
		bool bTarget = false;
		for (int i = 0; i < m_pWeaponList.size();)
		{
			for (int imon = 0; imon < MONSTER.GetMonsterList().size(); imon++)
			{
				if (m_pWeaponList[i]->iMonsternum == MONSTER.GetMonster(imon)->m_iNumber)
				{
					if (MONSTER.GetMonster(imon)->GetDeath())
					{
						break;
					}
					WeaponTargetPositionSet(m_pWeaponList[i], MONSTER.GetMonster(imon)->m_Monster->m_Center);
					fSphere weaponfS;
					fSphere monsterfS;

					weaponfS = I_Col.RecttoSphere(m_pWeaponList[i]->m_fRect);
					monsterfS = I_Col.RecttoSphere(MONSTER.GetMonster(imon)->m_Monster->m_fRect);

					if (I_Col.Spherecollision(weaponfS, monsterfS))				//충돌여부 참일시 bTarget이 false상태로 break해 무기 목록 삭제한다.
					{
						tower->m_Tower[m_pWeaponList[i]->m_iTowerChar]->SetLV(m_pWeaponList[i]->bLV);
						TOWER_EFFECT.EffectMake(tower->m_Tower[m_pWeaponList[i]->m_iTowerChar]->GetEffect(), &MONSTER.GetMonster(imon)->m_MatWorld);
						MONSTER.GetMonster(imon)->SetAttack(true, m_pWeaponList[i]->GetPower());		//해당 몬스터의 피격 판정
						break;
					}


					bTarget = true;
					break;
				}
			}

			if (bTarget)
			{
				bTarget = false;
				i++;
			}
			else
			{
				TOWER_WEAPON.WeaponPop(i);
			}
		}
	}

	TOWER_WEAPON.Frame();
	TOWER_EFFECT.Frame();
}

WeaponMgr::WeaponMgr()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

WeaponMgr::~WeaponMgr()
{
}
