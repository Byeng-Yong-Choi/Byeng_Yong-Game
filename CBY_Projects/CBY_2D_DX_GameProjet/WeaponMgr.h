#pragma once
#include "WeaponQueue.h"
#include "C_ObjectMT.h"
#include <memory>

typedef std::vector<TowerWeapon*>::iterator Weaiter;

class WeaponMgr :public Singleton<WeaponMgr>
{
public:
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	std::vector<TowerWeapon*> m_pWeaponList;
	WeaponQueue m_WeaponQueue;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	void WeaponTargetPositionSet(TowerWeapon* weapon, D3DXVECTOR3 pos);
	void WeaponMake(TowerWeapon* weapon, D3DXMATRIX* world,int imon, bool blv);
	void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
	Weaiter WeaponPop(Weaiter iweapon);
	void WeaponPop(int iweapon);
	void Process(std::shared_ptr<C_Tower>& tower);

public:
	WeaponMgr();
	~WeaponMgr();
};

#define TOWER_WEAPON WeaponMgr::GetInstance()
