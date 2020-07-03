#pragma once
#include "TowerPosition.h"
#include "C_ObjectMT.h"
#include "C_SoundManager.h"
#include "C_Menu.h"
#include "CBYCore.h"

class C_InGame
{
public:
	std::shared_ptr<C_Menu> m_Menu;
	//std::vector<C_Monster*> m_pMonsterList;
	std::vector<std::shared_ptr<C_Tower>> m_TowerList;
	int m_iLevel;
	int bgm;
	int m_iBossSound;
	int selectbgm;
	float fMoneyTime;
	bool m_bBoss;

public:
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pContext;
	CCamera* m_pMainCamera;

public:
	bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	bool Frame();
	bool Render();
	bool Release();

public:
	void SetCamera(CCamera* camera);
	void GetGameMoney();
	void GameSet();

public:
	bool GameLose();

public:
	HRESULT CreateDXrc();

public:
	D3DXMATRIX TowerPositionSet(int i);

public:
	C_InGame();
	~C_InGame();
};


