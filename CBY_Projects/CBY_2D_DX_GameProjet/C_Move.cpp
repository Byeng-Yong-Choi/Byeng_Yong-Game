#include "C_Move.h"
#include "C_GameStd.h"

void C_Move::Process(std::vector<std::shared_ptr<C_Tower>>& Tower)
{
	if (m_pMonster->m_Monster->GetSpliteEnd())
	{
		m_pMonster->m_Monster->m_StateList[m_pMonster->m_Monster->GetState()]->m_Index = 0;
		m_pMonster->m_Monster->m_StateList[m_pMonster->m_Monster->GetState()]->End = false;
		m_pMonster->m_Monster->SetState(MONSTER_MOVE);
	}
	float minusY, plusY;
	plusY = (Winrt.bottom / 11.0f);
	minusY = (Winrt.bottom / 5.0f);

	float fMove = 0;
	fMove = g_SecondTime * m_pMonster->GetMonsterSpeed();

	switch (m_pMonster->m_iRoot)
	{
	case 0:
		m_pMonster->m_Monster->m_Pos.x += fMove;
		if (m_pMonster->m_Monster->m_Pos.x >= -(Winrt.right/3.88f))
			m_pMonster->m_iRoot = 1;
		break;
	case 1:
		m_pMonster->m_Monster->m_Pos.y += fMove;
		if (m_pMonster->m_Monster->m_Pos.y >= ((Winrt.bottom / 14.11f)+ plusY))
			m_pMonster->m_iRoot = 2;
		break;

	case 2:
		m_pMonster->m_Monster->m_Pos.x += fMove;
		if (m_pMonster->m_Monster->m_Pos.x >= -(Winrt.right / 11.74f))
			m_pMonster->m_iRoot = 3;
		break;

	case 3:
		m_pMonster->m_Monster->m_Pos.y -= fMove;
		if (m_pMonster->m_Monster->m_Pos.y <= -(Winrt.bottom / 2.27f) + minusY)
			m_pMonster->m_iRoot = 4;
		break;

	case 4:
		m_pMonster->m_Monster->m_Pos.x += fMove;
		if (m_pMonster->m_Monster->m_Pos.x >= (Winrt.right / 6.70f))
		{
			m_pMonster->m_iRoot = 5;
			m_pMonster->SetRotaion(135.0f);
		}
		break;

	case 5:
		m_pMonster->m_Monster->m_Pos.y += fMove;
		if (m_pMonster->m_Monster->m_Pos.y >= ((Winrt.bottom / 4.5f) + plusY))
		{
			m_pMonster->m_iRoot = 6;
			
		}
		break;

	case 6:
		m_pMonster->m_Monster->m_Pos.x -= fMove;
		if (m_pMonster->m_Monster->m_Pos.x <= -(Winrt.right / 3.89f))
		{
			m_pMonster->m_iRoot = 7;
			m_pMonster->SetRotaion(135.0f);
		}
		break;

	case 7:
		m_pMonster->m_Monster->m_Pos.y += fMove;
		if (m_pMonster->m_Monster->m_Pos.y >= ((Winrt.bottom / 3.0f) + plusY))
			m_pMonster->m_iRoot = 8;
		break;

	case 8:
		m_pMonster->m_Monster->m_Pos.x += fMove;
		if (m_pMonster->m_Monster->m_Pos.x >= (Winrt.right / 3.49f))
		{
			m_pMonster->m_iRoot = 9;
		}
		break;

	case 9:
		m_pMonster->m_Monster->m_Pos.y -= fMove;
		if (m_pMonster->m_Monster->m_Pos.y <= ((Winrt.bottom / 4.51f) + plusY))
			m_pMonster->m_iRoot = 10;
		break;

	case 10:
		m_pMonster->m_Monster->m_Pos.x += fMove;
		if (m_pMonster->m_Monster->m_Pos.x >= (Winrt.right / 2.34f))
		{
			m_pMonster->m_iRoot = 11;
			m_pMonster->SetRotaion(135.0f);
		}
		break;

	case 11:
		m_pMonster->m_Monster->m_Pos.y -= fMove;
		if (m_pMonster->m_Monster->m_Pos.y <= ((Winrt.bottom / 14.11f) + plusY))
		{
			m_pMonster->m_iRoot = 12;
			
		}
		break;

	case 12:
		m_pMonster->m_Monster->m_Pos.x -= fMove;
		if (m_pMonster->m_Monster->m_Pos.x <= (Winrt.right / 3.56f))
			m_pMonster->m_iRoot = 13;
		break;

	case 13:
		m_pMonster->m_Monster->m_Pos.y -= fMove;
		if (m_pMonster->m_Monster->m_Pos.y <= -(Winrt.bottom / 3.8f) + minusY)
			m_pMonster->m_iRoot = 14;
		break;

	case 14:
		m_pMonster->m_Monster->m_Pos.x -= fMove;
		if (m_pMonster->m_Monster->m_Pos.x <= -(Winrt.right / 3.83f))
			m_pMonster->m_iRoot = 15;
		break;

	case 15:
		m_pMonster->m_Monster->m_Pos.y -= fMove;
		if (m_pMonster->m_Monster->m_Pos.y <= -(Winrt.bottom / 2.14f) + minusY)
		{
			m_pMonster->m_iRoot = 0;
			m_pMonster->SetAction(EVENT_END);
		}
		break;
	}

	if (m_pMonster->GetAttack())
	{
		m_pMonster->SetAction(EVENT_HIT);
	}

	m_pMonster->SetColdAttack(false, 0);
	
	/*m_pMonster->fDebugDeath += g_SecondTime;
	if (m_pMonster->fDebugDeath > 50.0f)
	{
		m_pMonster->fDebugDeath = 0.0f;
		m_pMonster->SetAction(EVENT_END);
	}*/


}



C_Move::C_Move(C_Monster* host) :C_MonsterStateProcess(host)
{
}


C_Move::~C_Move()
{
}
