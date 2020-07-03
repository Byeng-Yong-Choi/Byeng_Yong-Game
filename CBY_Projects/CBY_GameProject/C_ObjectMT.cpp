#include "C_ObjectMT.h"
#include "C_MonsterFsm.h"
#include "C_STANDBY.h"
#include "C_Make.h"
#include "C_Move.h"
#include "C_Damage.h"
#include "C_DEATH.h"
#include "C_TowerFsm.h"
#include "C_ATTACK.h"
#include "C_Create.h"
#include "C_Demolish.h"
#include "C_LevelUP.h"
#include "C_Null.h"
#include "C_TowerStandBy.h"
#include "C_MonClear.h"

//int C_Monster::index=-1;

void C_Monster::Process(vector<C_Tower>& Tower)
{
	m_Action->Process(Tower);
}

void C_Monster::SetAction(DWORD Event)
{
	MonsterState = C_MonsterFsm::Get().Output(MonsterState, Event);
	m_Action = m_ActionList[MonsterState];
}

bool C_Monster::Init()
{
	HPNUMBER = HP;
	Monster::Init();
	C_MonsterFsm::Get().Init();

	MonsterState = STATE_STANDBY;

	m_ActionList[STATE_STANDBY] = new C_STANDBY(this);
	m_ActionList[STATE_MAKE] = new C_Make(this);
	m_ActionList[STATE_MOVE] = new C_Move(this);
	m_ActionList[STATE_DAMAGE] = new C_Damage(this);
	m_ActionList[STATE_DEATH] = new C_DEATH(this);
	m_ActionList[STATE_CLEAR] = new C_MonClear(this);

	m_Action = m_ActionList[MonsterState];


	return true;
}

bool C_Monster::Frame()
{
	Monster::Frame();
	
	return true;
}

bool C_Monster::Render()
{
	Monster::Render();
	HPbar->Render();
	

	return true;
}

bool C_Monster::Release()
{
	Monster::Release();
	HPbar->Release();
	

	return true;
}


C_Monster::C_Monster()
{
	monSpeed = MONSTERSPPED;
	End = false;
	ok = false;
	HPbar = new Health;
}


C_Monster::~C_Monster()
{
	
}

void C_Tower::Process(list<C_Monster*>& obj)
{
	m_Action->Process(obj);
}

void C_Tower::SetAction(DWORD Event)
{
	TowerState = C_TowerFsm::Get().Output(TowerState, Event);
	m_Action = m_ActionList[TowerState];
}

bool C_Tower::Init()
{
	C_TowerFsm::Get().Init();
	TowerPosition::Init();
	TowerState = STATE_NULL;

	m_ActionList[STATE_NULL] = new C_Null(this);
	m_ActionList[STATE_TOWERSTANDBY] = new C_TowerStandBy(this);
	m_ActionList[STATE_CREAT] = new C_Create(this);
	m_ActionList[STATE_ATTACK] = new C_ATTACK(this);
	m_ActionList[STATE_DEMOLISH] = new C_Demolish(this);
	m_ActionList[STATE_LEVELUP] = new C_LevelUP(this);

	m_Action = m_ActionList[TowerState];
	return true;
}

bool C_Tower::Release()
{
	TowerPosition::Release();
	return true;
}


C_Tower::C_Tower()
{
	
}


C_Tower::~C_Tower()
{
}



C_ObjectMT::C_ObjectMT()
{
}


C_ObjectMT::~C_ObjectMT()
{
}


