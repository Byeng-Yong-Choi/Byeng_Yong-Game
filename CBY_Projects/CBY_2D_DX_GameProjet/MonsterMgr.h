#pragma once
#include "MonsterQueue.h"

typedef std::vector<C_Monster*>::iterator Moniter;

class MonsterMgr :public Singleton<MonsterMgr>
{
private:
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	int m_iLevel;
	bool m_bBoss;

private:
	std::shared_ptr<C_Monster> m_MonsterSrc;
	std::shared_ptr<C_monPig> m_PigSrc;
	std::shared_ptr<C_monRobo> m_RoboSrc;
	std::shared_ptr<C_Boss> m_BossSrc;
	std::shared_ptr<C_MonsterHP> m_HPSrc;
	std::vector<C_Monster*> m_pMonsterList;
	MonsterQueue m_MonsterQueue;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	void SetMatrix(D3DXMATRIX* World, D3DXMATRIX* View, D3DXMATRIX* Proj);
	HRESULT Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	void InitMonsterList();
	void PosUpdate();
	void MonsterMake(int iMonstertype, int iLevel, DWORD index);
	Moniter MonsterPop(Moniter iweapon);
	void MonsterPop(int imon);
	void MonsterClear();

public:
	inline std::vector<C_Monster*> GetMonsterList() { return m_pMonsterList; }
	inline C_Monster* GetMonster(int i) { return m_pMonsterList[i]; }

public:
	MonsterMgr();
	~MonsterMgr();
};

#define MONSTER MonsterMgr::GetInstance()


