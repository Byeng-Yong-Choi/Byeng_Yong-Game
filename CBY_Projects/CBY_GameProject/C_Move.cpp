#include "C_Move.h"

void C_Move::Process(vector<C_Tower>& Tower)
{
	if (m_pMonster->monSpeed!=MONSTERSPPED)
	{

		if (coldeffecttime > 0.5f)
		{
			for (int i = 0; i < Tower.size(); i++)
			{
				if (Tower[i].TowerNumber == 2)
				{
					TowerEffect* Effect = new TowerEffect(Tower[i].m_Effect[2]);
					Effect->Frame();
					Effect->SetPosition(I_Col.Center(m_pMonster->BitMapRt));
					Tower[i].m_EffectList.push_back(Effect);
				}
			}
			coldeffecttime=0;
		}
		else
		{
			coldeffecttime += g_SecondTime;
		}
	}
	
	
	float x=0;
	x += g_SecondTime * m_pMonster->monSpeed;
	m_pMonster->Rotate = false;
	m_pMonster->Revers = false;

	switch (root)
	{
	case 0:
		m_pMonster->IX += x;
		m_pMonster->HPbar->IX = m_pMonster->BitMapRt.left;
		m_pMonster->Revers = true;
		if (m_pMonster->IX >= 279)
			root = 1;
		break;
	case 1:
		m_pMonster->IY -= x;
		m_pMonster->HPbar->IY = m_pMonster->BitMapRt.bottom+5;
		m_pMonster->Revers = true;
		if (m_pMonster->IY <= 290)
			root = 2;
		break;

	case 2:
		m_pMonster->IX += x;
		m_pMonster->HPbar->IX = m_pMonster->BitMapRt.left;
		m_pMonster->Revers = true;
		if (m_pMonster->IX >= 500)
			root = 3;
		break;

	case 3:
		m_pMonster->IY += x;
		m_pMonster->HPbar->IY = m_pMonster->BitMapRt.bottom + 5;
		m_pMonster->Revers = true;
		if (m_pMonster->IY >= 658)
			root = 4;
		break;

	case 4:
		m_pMonster->IX += x;
		m_pMonster->HPbar->IX = m_pMonster->BitMapRt.left;
		m_pMonster->Revers = true;
		if (m_pMonster->IX >= 800)
			root = 5;
		break;

	case 5:
		m_pMonster->Make = 1;
		m_pMonster->HPbar->Make = 1;
		m_pMonster->IY -= x;
		m_pMonster->HPbar->IY = m_pMonster->BitMapRt.bottom + 5;
		m_pMonster->GetRotationBitMap(90, m_pMonster->m_pBitMap->MemDC);
		m_pMonster->GetMaskRotationBitMap(90, m_pMonster->m_pMaskBitMap->MemDC);
		if (m_pMonster->IY <= 172)
			root = 6;
		if (645 >= m_pMonster->IY && 389 <= m_pMonster->IY)
		{
			m_pMonster->Make = 0;
			m_pMonster->HPbar->Make = 0;
		}
		break;

	case 6:
		m_pMonster->IX -= x;
		m_pMonster->HPbar->IX = m_pMonster->BitMapRt.left;
		if (m_pMonster->IX <= 280)
			root = 7;
		break;

	case 7:
		m_pMonster->IY -= x;
		m_pMonster->HPbar->IY = m_pMonster->BitMapRt.bottom + 5;
		m_pMonster->Revers = true;
		if (m_pMonster->IY <= 65)
			root = 8;
		break;

	case 8:
		m_pMonster->IX += x;
		m_pMonster->HPbar->IX = m_pMonster->BitMapRt.left;
		m_pMonster->Revers = true;
		if (m_pMonster->IX >= 975)
			root = 9;
		break;

	case 9:
		m_pMonster->IY += x;
		m_pMonster->HPbar->IY = m_pMonster->BitMapRt.bottom + 5;
		m_pMonster->Revers = true;
		if (m_pMonster->IY >= 174)
			root = 10;
		break;

	case 10:
		m_pMonster->IX += x;
		m_pMonster->HPbar->IX = m_pMonster->BitMapRt.left;
		m_pMonster->Revers = true;
		if (m_pMonster->IX >= 1156)
			root = 11;
		break;

	case 11:
		m_pMonster->IY += x;
		m_pMonster->HPbar->IY = m_pMonster->BitMapRt.bottom + 5;
		if (m_pMonster->IY >= 290)
			root = 12;
		break;

	case 12:
		m_pMonster->IX -= x;
		m_pMonster->HPbar->IX = m_pMonster->BitMapRt.left;
		if (m_pMonster->IX <= 968)
			root = 13;
		break;

	case 13:
		m_pMonster->IY += x;
		m_pMonster->HPbar->IY = m_pMonster->BitMapRt.bottom + 5;
		if (m_pMonster->IY >= 510)
			root = 14;
		break;

	case 14:
		m_pMonster->Make = 1;
		m_pMonster->HPbar->Make = 1;
		m_pMonster->IX -= x;
		m_pMonster->HPbar->IX = m_pMonster->BitMapRt.left;
		if (m_pMonster->IX <= 275)
			root = 15;
		if (682 >= m_pMonster->IX && 357 <= m_pMonster->IX)
		{
			m_pMonster->Make = 0;
			m_pMonster->HPbar->Make = 0;
		}
		break;

	case 15:
		m_pMonster->IY += x;
		m_pMonster->HPbar->IY = m_pMonster->BitMapRt.bottom + 5;
		if (m_pMonster->IY >= 676)
		{
			m_pMonster->SetAction(EVENT_END);
		}
		break;
	}

	m_pMonster->monSpeed = MONSTERSPPED;

	std::list<TowerWeapon*> wea;
	TowerWeapon* weanode;
	std::list<TowerWeapon*>::iterator weaiter;

	

	m_pMonster->Frame();
	m_pMonster->HPbar->Frame();
	

	for (int i = 0; i < Tower.size(); i++)
	{
		
		for (weaiter= Tower[i].m_WeaponList.begin(); weaiter!= Tower[i].m_WeaponList.end(); weaiter++)
		{
			weanode=weaiter.operator*();
			
			if (I_Col.collision(m_pMonster->BitMapRt, weanode->BitMapRt))
			{
				m_pMonster->SetAction(EVENT_HIT);
				return;
			}
		}
		
	}

}



C_Move::C_Move(C_Monster* host) :C_MonsterStateProcess(host)
{
	root = 0;
}


C_Move::~C_Move()
{
}
