#include "C_ObjectMT.h"
#include "C_MonsterFsm.h"
#include "C_TowerFsm.h"
#include "C_StateStd.h"
#include "WeaponMgr.h"
#include "C_GameStd.h"
#include "C_EffectMgr.h"

//int C_Monster::index=-1;

void C_Monster::Process(std::vector<std::shared_ptr<C_Tower>>& Tower)
{
	fColdTime += g_SecondTime;
	m_HP->SetMonsterPos(m_Monster->m_Center);
	m_Action->Process(Tower);
	m_Monster->Frame();
}

void C_Monster::SetAction(DWORD Event)
{
	MonsterState = C_MonsterFsm::Get().Output(MonsterState, Event);
	m_Action = m_ActionList[MonsterState];
}

void C_Monster::SetRotaion(float angle)
{
	//D3DXMatrixIdentity(&m_MatWorld);
	D3DXMATRIX rot;
	D3DXMatrixRotationY(&rot, angle);
	m_matRot *= rot;
}

bool C_Monster::GetBoss()
{
	return bBoss;
}

void C_Monster::SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	if (world != nullptr)
	{
		m_MatWorld = *world;
	}

	if (view != nullptr)
	{
		m_MatView = *view;
	}

	if (proj != nullptr)
	{
		m_MatProj = *proj;
	}
	D3DXMATRIX matMove;
	D3DXMatrixTranslation(&matMove, m_Monster->m_Pos.x, m_Monster->m_Pos.y, m_Monster->m_Pos.z);
	
	m_MatWorld = m_matRot* matMove;

	/*m_MatWorld._41 = m_Monster->m_Pos.x;
	m_MatWorld._42 = m_Monster->m_Pos.y;
	m_MatWorld._43 = m_Monster->m_Pos.z;*/
	m_Monster->SetWorldandMatrix(&m_MatWorld, &m_MatView, &m_MatProj);
	m_HP->SetWorldandMatrix(nullptr, &m_MatView, &m_MatProj);
}

void C_Monster::SetMonster(int monster, int level)
{
	D3DXMatrixIdentity(&m_MatWorld);
	D3DXMatrixIdentity(&m_matRot);
	bBoss = false;

	switch (monster)
	{
	case MONSTER_PIG:
	{
		*m_Monster = *pig->m_Src;
		m_Monster->SetMonsterHP(PIG_LVUPHP * level);
		break;
	}

	case MONSTER_ROBO:
	{
		*m_Monster = *robo->m_Src;
		m_Monster->SetMonsterHP(PIG_LVUPHP * level);
		break;
	}
	case MONSTER_BOSS:
	{
		*m_Monster = *Boss->m_Src;
		m_Monster->SetMonsterHP(PIG_LVUPHP * level * 10);
		bBoss = true;
		break;
	}
	}

	DeathClear();
}

void C_Monster::SetDeath(bool death)
{
	bDeath = death;
}

bool C_Monster::GetDeath()
{
	return bDeath;
}

void C_Monster::SetColdAttack(bool attack, float fslowvalue)
{
	bColdAttack = attack;
	m_fMonspeed = MONSTERSPEED - fslowvalue;
	if (bBoss)
	{
		m_fMonspeed += 25;
	}
}

bool C_Monster::GetColdEffectTime()
{
	if (fColdTime >= 0.5f)
	{
		fColdTime = 0.0f;
		return true;
	}

	return false;
}

bool C_Monster::GetColdAttack()
{
	if (bColdAttack)
	{
		return true;
	}

	return false;
}

void C_Monster::SetMonsterAttackHP()
{
	std::vector<int>::iterator iter;

	if (m_iDamageList.size() > 0)
	{
		iter = m_iDamageList.begin();
		m_iHp -= *iter;

		m_iDamageList.erase(iter);

		if (m_iHp <= 0)
		{
			m_iHp = 0;
		}
	}
	else
	{
		bAttack = false;
	}
}

void C_Monster::DeathClear()
{
	Sethide(true);
	MonsterState = STATE_STANDBY;
	SetHP(m_Monster->m_iHp);
	SetDeath(false);
	m_Action = m_ActionList[MonsterState];
	SetAttack(false, 0);
	m_iDamageList.clear();
}

void C_Monster::SetHP(int i)
{
	m_fFirstHP = i;
	m_iHp = i;
}

bool C_Monster::HPZero()
{
	if (m_iHp == 0)
	{
		return true;
	}



	return false;
}

void C_Monster::Sethide(bool hide)
{
	bHide = hide;
}

bool C_Monster::Gethide()
{
	return bHide;
}

void C_Monster::SetAttack(bool attack, int damage)
{
	bAttack = attack;
	if (bAttack)
	{
		m_iDamageList.push_back(damage);
	}
}

bool C_Monster::GetAttack()
{
	return bAttack;
}

void C_Monster::SetMonsterSpeed(float monspeed)
{
	m_fMonspeed = monspeed;
}

float C_Monster::GetMonsterSpeed()
{
	return m_fMonspeed;
}

void C_Monster::SetTarget(bool target)
{
	bTarget = target;
}

bool C_Monster::GetTarget()
{
	return bTarget;
}

void C_Monster::GetMonsterSrc(C_monPig* ppig, C_monRobo* probo, C_Boss* pboss, C_MonsterHP* php)
{
	*pig = *ppig;
	*robo = *probo;
	*Boss = *pboss;
	*m_HP = *php;
}

bool C_Monster::Init()
{
	C_MonsterFsm::Get().Init();

	pig = std::make_shared<C_monPig>();
	robo = std::make_shared<C_monRobo>();
	Boss = std::make_shared<C_Boss>();

	m_Monster = new C_MonsterSrc;

	MonsterState = STATE_STANDBY;

	m_ActionList[STATE_STANDBY] = new C_STANDBY(this);
	m_ActionList[STATE_MAKE] = new C_Make(this);
	m_ActionList[STATE_MOVE] = new C_Move(this);
	m_ActionList[STATE_DAMAGE] = new C_Damage(this);
	m_ActionList[STATE_DEATH] = new C_DEATH(this);
	m_ActionList[STATE_CLEAR] = new C_MonClear(this);

	m_Action = m_ActionList[MonsterState];


	m_HP = std::make_shared<C_MonsterHP>();
	return true;
}

bool C_Monster::Frame()
{

	return true;
}

bool C_Monster::Render()
{
	m_Monster->Render();
	m_HP->SetHPIndex(m_fFirstHP, m_iHp);
	m_HP->Render();
	return true;
}

bool C_Monster::Release()
{
	m_Monster->Release();
	m_HP->Release();
	return true;
}


C_Monster::C_Monster()
{
	m_fMonspeed = MONSTERSPEED;
	bBoss = false;
	m_iRoot = 0;
	m_fStartTime = 0.0f;
	m_fTime = 0.0f;
	bHide = true;
	bTarget = false;
	bAttack = false;
	bDeath = false;
	fDebugDeath = 0.0f;
	fColdTime = EFFECT_ANIMATION_TIME;
	m_iHp = 0;
	D3DXMatrixIdentity(&m_MatWorld);
	D3DXMatrixIdentity(&m_MatView);
	D3DXMatrixIdentity(&m_MatProj);
	D3DXMatrixIdentity(&m_matRot);
	
}


C_Monster::~C_Monster()
{

}

bool C_TowerStateProcess::ColCheck(C_Monster* mon)
{
	fSphere towerfS;
	fSphere monsterfS;

	towerfS = I_Col.RecttoSphere(m_pTower->m_frTowerScope);
	monsterfS = I_Col.RecttoSphere(mon->m_Monster->m_fRect);

	if (I_Col.Spherecollision(towerfS, monsterfS))
	{
		if (mon->GetTarget())
		{
			if (m_pTower->MakeLvTower)
			{
				if (m_pTower->bHidden())		//히든 타워면 공격 스테이트가 번호가 일반과 같아 이렇게 적용
				{
					m_pTower->m_Tower[m_pTower->MakeTowerNum]->SetState(TOWER_ATTACK);
				}
				else
				{
					m_pTower->m_Tower[m_pTower->MakeTowerNum]->SetState(TOWER_LEVELUP_ATTACK);
				}
			}
			else
			{
				m_pTower->m_Tower[m_pTower->MakeTowerNum]->SetState(TOWER_ATTACK);
			}
		}
		else
		{
			if (m_pTower->MakeLvTower)
			{
				if (m_pTower->bHidden())
				{
					m_pTower->m_Tower[m_pTower->MakeTowerNum]->SetState(TOWER_STANDBY);
				}
				else
				{
					m_pTower->m_Tower[m_pTower->MakeTowerNum]->SetState(TOWER_LEVELUP_STATE);
				}
			}
			else
			{
				m_pTower->m_Tower[m_pTower->MakeTowerNum]->SetState(TOWER_STANDBY);
			}
		}

		return true;
	}
	else
	{
		return false;
	}

}

void C_Tower::Process()
{
	m_Action->Process();
	Frame();
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

void C_Tower::TowerRot(D3DXVECTOR3 monsterpos)
{
	float dot;
	dot = D3DXVec3Dot(&m_Center, &monsterpos);

	D3DXQUATERNION qRot;
	D3DXQuaternionNormalize(&qRot, &qRot);
	D3DXQuaternionToAxisAngle(&qRot, &m_Center, &dot);

	D3DXMATRIX rot;
	D3DXMatrixRotationQuaternion(&rot, &qRot);
	m_Rot = rot;
}

void C_Tower::GameSet()
{
	SetAction(EVENT_NULL);
	TowerPosition::GameSet();
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