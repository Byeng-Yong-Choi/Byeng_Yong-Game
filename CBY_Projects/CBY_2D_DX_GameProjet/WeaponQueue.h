#pragma once
#include "TowerWeapon.h"
#include "QueueStd.h"

class WeaponQueue :public TowerWeapon
{
public:
	std::queue<TowerWeapon*> m_List;
	void Push(TowerWeapon* pobj);
	TowerWeapon* Pop();
	void Relese();

public:
	WeaponQueue();
	virtual ~WeaponQueue();
};