#pragma once
#include "C_MonsterState.h"
#include "C_Intersection.h"
#include "C_TowerState.h"
#include "TowerPosition.h"
#include "CSprite.h"
#include "C_monPig.h"
#include "C_monRobo.h"
#include "C_MonsterHP.h"
#include "C_Boss.h"

class C_Monster;
class C_Tower;

typedef std::vector<C_Monster*>::iterator Monsteriter;

enum Monster
{
	MONSTER_PIG = 0,
	MONSTER_ROBO,
	MONSTER_BOSS=5
};

class C_MonsterStateProcess
{
protected:
	C_Monster* m_pMonster;

public:
	virtual void Process(std::vector<std::shared_ptr<C_Tower>>& Tower) {};
	float coldeffecttime;

public:
	C_MonsterStateProcess(C_Monster* host) : m_pMonster(host)
	{
		coldeffecttime = 2.0f;
	};
	//C_MonsterStateProcess();
	~C_MonsterStateProcess() {};

};

class C_Monster
{
private:
	bool bHide;						//몬스터의 모습 감춤
	bool bColdAttack;				//디버프 타워의 공격 유무
	bool bAttack;					//피격 여부
	bool bDeath;					//몬스터 죽음 여부
	bool bTarget;					//타켓이 될 수 있는지 여부
	float m_fMonspeed;
	float fColdTime;
	float m_fFirstHP;				//몬스터의 셋팅된 체력
	int m_iHp;						//현재 몬스터의 체력
	bool bBoss;
	std::vector<int> m_iDamageList;


public:
	std::shared_ptr<C_MonsterHP> m_HP;
	C_MonsterSrc* m_Monster;
	std::shared_ptr<C_monPig> pig;
	std::shared_ptr<C_monRobo> robo;
	std::shared_ptr<C_Boss> Boss;
	DWORD MonsterState;
	C_MonsterStateProcess* m_Action;
	C_MonsterStateProcess* m_ActionList[STATE_COUNT];
	D3DXMATRIX m_matRot;
	D3DXMATRIX m_MatWorld;
	D3DXMATRIX m_MatView;
	D3DXMATRIX m_MatProj;

public:
	int m_iRoot;
	int m_iNumber;					//몬스터의 번호
	float m_fStartTime;				//몬스터의 시작점 출발 시간
	float m_fTime;
	float fDebugDeath;				//디버깅중 시간지체되어 몬스터가 다른 방향으로 튈때 사망하게 할 시간

public:
	void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
	void GetMonsterSrc(C_monPig* ppig, C_monRobo* probo, C_Boss* pboss, C_MonsterHP* php);
	bool Init();
	void SetMonster(int monster,int level);
	bool Frame();
	bool Render();
	bool Release();
	void SetAction(DWORD Event);
	void Process(std::vector<std::shared_ptr<C_Tower>>& Tower);

public:
	void SetColdAttack(bool attack, float fslowvalue);
	bool GetColdAttack();
	bool GetColdEffectTime();
	void SetMonsterAttackHP();			//몬스터 피격시 hp셋팅
	void SetHP(int hp);
	void SetDeath(bool death);
	bool GetDeath();							//몬스터의 죽음 여부 파악
	void Sethide(bool hide);
	bool Gethide();
	void SetAttack(bool attack, int damage);
	bool GetAttack();
	void SetMonsterSpeed(float monspeed);
	float GetMonsterSpeed();
	bool HPZero();
	void SetTarget(bool target);
	bool GetTarget();
	void SetRotaion(float angle);
	void DeathClear();
	bool GetBoss();

public:
	C_Monster();
	~C_Monster();
};


class C_TowerStateProcess
{
protected:
	C_Tower* m_pTower;

public:
	virtual void Process() {};
	bool ColCheck(C_Monster* mon);
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
	bool Init();
	bool Release();
	void Process();
	void SetAction(DWORD Event);
				//몬스터와의 각도(?)
	void TowerRot(D3DXVECTOR3 monsterpos);
	void GameSet() override;

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

