#pragma once
#include "Monster.h"
#include "Health.h"
#include "C_MonsterState.h"
#include "C_Intersection.h"
#include "C_TowerState.h"
#include "C_ObjectMT.h"
#include "TowerPosition.h"
#include "GameMgr.h"

class C_Monster;
class C_Tower;

class C_MonsterStateProcess
{
protected:
	C_Monster* m_pMonster;

public:
	virtual void Process(vector<C_Tower>& Tower) {};
	float coldeffecttime;
	
	

public:
	C_MonsterStateProcess(C_Monster* host) : m_pMonster(host)
	{
		coldeffecttime = 2.0f;
	};
	//C_MonsterStateProcess();
	~C_MonsterStateProcess() {};

};


class C_Monster : public Monster
{
public:
	DWORD MonsterState;
	Health* HPbar;
	C_MonsterStateProcess* m_Action;
	C_MonsterStateProcess* m_ActionList[STATE_COUNT];
	int HPNUMBER;
	int index;
	bool End;
	bool ok;
	float monSpeed;
	

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	void Process(vector<C_Tower>& Tower);
	void SetAction(DWORD Event);

public:
	C_Monster();
	~C_Monster();
};


class C_TowerStateProcess
{
protected:
	C_Tower* m_pTower;

public:
	virtual void Process(list<C_Monster*>& obj) {};
	float dgr=0.0f;
	bool cold;

public:
	C_TowerStateProcess(C_Tower* host) : m_pTower(host)
	{
		//dgr = 0.0f;
		cold = false;
	};
	~C_TowerStateProcess() {};

};

class C_Tower : public TowerPosition
{
public:
	DWORD TowerState;
	C_TowerStateProcess* m_Action;
	C_TowerStateProcess* m_ActionList[STATE_TOWERCOUNT];
public:
	bool Init() override;
	bool Release();
	void Process(list<C_Monster*>& obj);
	void SetAction(DWORD Event);
				//몬스터와의 각도

public:
	C_Tower();
	~C_Tower();
};


class C_ObjectMT
{
public:
	C_ObjectMT();
	~C_ObjectMT();
};

