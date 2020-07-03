#pragma once
#include "Cstd.h"
#include "C_ObjectMT.h"

class C_TowerHiddenMgr :public Singleton<C_TowerHiddenMgr>
{
public:
	bool bGoku;
	bool bVegeta;
	bool bHidden;
	int m_HiddenPrice;
	std::vector<int> m_goku;
	std::vector<int> m_vegeta;

public:
	void HiddenMgrFrame(std::vector<std::shared_ptr<C_Tower>>& Tower);

public:
	void CheckHidden(std::vector<std::shared_ptr<C_Tower>>& Tower);
	bool Hidden();
	void MakeHiddenTower(std::vector<std::shared_ptr<C_Tower>>& Tower);
	bool Release();

	
public:
	C_TowerHiddenMgr();
	virtual ~C_TowerHiddenMgr();
};

#define HIDDEN C_TowerHiddenMgr::GetInstance()

