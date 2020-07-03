#include "WeaponQueue.h"

void WeaponQueue::Push(TowerWeapon* pobj)
{
	m_List.push(pobj);
}

TowerWeapon* WeaponQueue::Pop()
{
	TowerWeapon* pobj = nullptr;
	if (m_List.empty() == false)
	{
		pobj = m_List.front();
		m_List.pop();
	}

	return pobj;
}

void WeaponQueue::Relese()
{
	TowerWeapon* pobj = nullptr;
	while (m_List.size() > 0)
	{
		pobj = m_List.front();
		delete pobj;
		m_List.pop();
	}
}


WeaponQueue::WeaponQueue()
{
}


WeaponQueue::~WeaponQueue()
{
}
