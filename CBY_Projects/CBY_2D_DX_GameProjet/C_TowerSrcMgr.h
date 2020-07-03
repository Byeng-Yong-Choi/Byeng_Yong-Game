#pragma once
#include "C_TowerGoku.h"
#include "C_TowerVegeta.h"
#include "C_TowerCold.h"
#include "C_HiddenTower.h"
#include "TowerCoice.h"

class C_TowerSrcMgr :public Singleton<C_TowerSrcMgr>
{
public:
	std::shared_ptr<C_TowerGoku> m_TowerGoku;
	std::shared_ptr<C_TowerVegeta> m_TowerVegeta;
	std::shared_ptr<C_TowerCold> m_TowerCold;
	std::shared_ptr<C_HiddenTower> m_TowerHidden;

public:
	std::vector<std::shared_ptr<TowerCoice>> m_TowerChoice;

public:
	HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	bool Release();
	void TextArray(int a);

public:
	TowerCoice* GetTowerChoice(int i);

public:
	C_TowerGoku* GetGoku();
	C_TowerVegeta* GetVegeta();
	C_TowerCold* GetCold();
	C_HiddenTower* GetHidden();

public:
	C_TowerSrcMgr();
	~C_TowerSrcMgr();
};

#define TOWERSRC C_TowerSrcMgr::GetInstance()

