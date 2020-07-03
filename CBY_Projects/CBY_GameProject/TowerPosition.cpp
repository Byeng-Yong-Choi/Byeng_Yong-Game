#include "TowerPosition.h"

bool TowerPosition::Init()
{
	C_Object::Init();
	Scope.resize(TOWERNUM);
	m_Choice.resize(TOWERCHOICENUM);			//선택지 생성
	m_LVChoice.resize(TOWERCHOICENUM);

	m_Tower.resize(TOWERNUM);					//타워 소스 생성
	m_LVTower.resize(TOWERNUM);
	
	m_rtList.resize(TOWERNUM);
	
	m_Weapon.resize(WEAPONNUM);					//무기 소스
	
	

	m_rtList[0].resize(4);
	m_rtList[1].resize(3);
	m_rtList[2].resize(2);
	
	m_Effect.resize(TOWERNUM);
	m_EffectrtList.resize(TOWERNUM);
	m_EffectrtList[0].resize(5);
	m_EffectrtList[1].resize(7);
	m_EffectrtList[2].resize(7);

	EffectSrc();

	


	CenterPT = I_Col.Center(BitMapRt);

	for (int i = 0; i < TOWERCHOICENUM; i++)
	{
		m_Choice[i].Init();
		m_Choice[i].Load(L"bitmap/Tower.bmp", L"bitmap/Towermask.bmp");
		TowerChoicert(i);										//타워를 선택할 이미지 미리보기의 위치 좌표
		m_Choice[i].GetRECT(ChoiceRt);
	}
	for (int i = 0; i < TOWERCHOICENUM; i++)
	{
		m_LVChoice[i].Init();
		m_LVChoice[i].Load(L"bitmap/Tower.bmp", L"bitmap/Towermask.bmp");
		TowerLVChoicert(i);										//타워를 선택할 이미지 미리보기의 위치 좌표
		m_LVChoice[i].GetRECT(ChoiceRt);
	}
	
	TowerRtsrc();				//타워 소스 RT제공

	for (int i = 0; i < TOWERNUM; i++)												//타워 소스 생성
	{
		float j = 0;
		j = TOWERSPEED + ((float)i*1.5f);
		
		ScopeSize(i);
		m_Tower[i].Init();
		m_Tower[i].Load(L"bitmap/Tower.bmp", L"bitmap/Towermask.bmp");
		m_Tower[i].GetRECT(m_rtList[i], j);
		if (i < 2)
		{
			m_Tower[i].power = POWER*(i+1);
		}
		
		Scope[i].SetScorpeScale(ScopeScaleSize);
		Scope[i].Init();
		Scope[i].ScopeSetPosition(CenterPT.x, CenterPT.y);
	}
	m_Tower[0].price = 50;
	m_Tower[1].price = 60;
	m_Tower[2].price = 80;


	LVTowerRtsrc();

	for (int i = 0; i < TOWERNUM; i++)												//레벨 업 타워 소스 생성
	{
		float j = TOWERSPEED+ ((float)i*1.2f);;
		if (i == 0)
		{
			j -= 0.3f;
		}
		m_LVTower[i].Init();
		m_LVTower[i].Load(L"bitmap/Tower.bmp", L"bitmap/Towermask.bmp");
		m_LVTower[i].GetRECT(m_rtList[i], j);
		if (i < 2)
		{
			m_LVTower[i].power = POWER * (i + 1);
			if (i == 1)
			{
				m_LVTower[i].power = POWER * (i + 1)+3;
			}
		}
		if (i < 2)
		{
			m_LVTower[i].power = m_Tower[i].power * (i + 1);
		}
	}
	m_LVTower[0].price = 100;
	m_LVTower[1].price = 100;
	m_LVTower[2].price = 80;

	for (int i = 0; i < WEAPONNUM-1; i++)											//무기 소스 생성
	{
		TowerWeaponrt(i);
		Weaponspeed(i);
		m_Weapon[i].Init();
		m_Weapon[i].Load(L"bitmap/Tower.bmp", L"bitmap/Towermask.bmp");
		m_Weapon[i].GetRECT(TowerWeaponRT);
		m_Weapon[i].SetPosition( BitMapRt.right,CenterPT.y);
		m_Weapon[i].WeaponSpeed = WEAPONSPEED;
		m_Weapon[i].Power = m_Tower[i].power;
	}
	
	m_Weapon[2].Init();
	m_Weapon[2].Cold = true;
	
	m_Effect[0].Init();
	m_Effect[0].Load(L"bitmap/Effectscolor.bmp", L"bitmap/Effectscolormask.bmp");
	m_Effect[0].GetRECT(m_EffectrtList[0], EFFECTSHOWTIME);

	m_Effect[1].Init();
	m_Effect[1].Load(L"bitmap/Effectscolor.bmp", L"bitmap/Effectscolormask.bmp");
	m_Effect[1].GetRECT(m_EffectrtList[1], EFFECTSHOWTIME);

	m_Effect[2].Init();
	m_Effect[2].Load(L"bitmap/Icecolor.bmp", L"bitmap/Icecolormask.bmp");
	m_Effect[2].GetRECT(m_EffectrtList[2], EFFECTSHOWTIME);

	return true;
}


bool TowerPosition::Frame()
{
	C_Object::Frame();
	ClickTime += g_SecondTime;

	if (BitMapRt.left < m_mousepos.x && m_mousepos.x < BitMapRt.right)					//타워 지정 위치를 클릭하는지 확인하는 조건문
	{
		if (BitMapRt.top < m_mousepos.y && m_mousepos.y < BitMapRt.bottom)
		{
			SetCursor(LoadCursor(NULL, IDC_HAND));
			if (Input.KeyCheck(VK_LBUTTON))
			{
					if (Click&&ClickTime >= 0.3f)
					{
						Click = false;
						ClickTime = 0.0f;
						Ren = 0.0f;
						LvRen = 0.0f;
					}
					
					if (ClickTime >= 0.3f)
					{
						Click = true;
						ClickTime = 0.0f;
					}
			}
		}
	}
	
	if (Click&&!MakeTower)																//지정 위치 클릭시 아이콘을 띄울 조건문
	{
		Ren += g_SecondTime * 80;
		if (Ren >= 50.0f)
		{
			Ren = 50.0f;
		}

		for (int i = 0; i < TOWERCHOICENUM; i++)
		{
			switch (i)
			{
			case 0:
				m_Choice[i].SetPosition(IX - Ren, IY - Ren);
				if (Ren == 50.0f)
				{
					m_Choice[i].Frame();
					if (m_Choice[i].TowerClick && ( m_Tower[i].price <= g_Money ))
					{
						MakeTower = true;
						MakeTowerNum = 1;
						g_Money -= m_Tower[i].price;
						
					}
				}
				break;

			case 1:
				m_Choice[i].SetPosition(IX - Ren, IY + Ren);
				if (Ren == 50.0f)
				{
					m_Choice[i].Frame();
					if (m_Choice[i].TowerClick && (m_Tower[i].price <= g_Money))
					{
						MakeTower = true;
						MakeTowerNum = 2;
						g_Money -= m_Tower[i].price;
						
					}
				}
				break;

			case 2:
				m_Choice[i].SetPosition(IX + Ren, IY - Ren);
				if (Ren == 50.0f)
				{
					m_Choice[i].Frame();
					if (m_Choice[i].TowerClick && (m_Tower[i].price <= g_Money))
					{
						MakeTower = true;
						MakeTowerNum = 3;
						g_Money -= m_Tower[i].price;
						
					}
				}
				break;

			case 3:
				m_Choice[i].SetPosition(IX + Ren, IY + Ren);
				if (Ren == 50.0f)
				{
					m_Choice[i].Frame();
					if (m_Choice[i].TowerClick && (m_Tower[i].price <= g_Money))
					{
						MakeTower = true;
						MakeTowerNum = 4;
						g_Money -= m_Tower[i].price;
					}
				}
				break;
			}
		}
	
	}

	if (Click&&MakeTower&&!MakeLvTower&&Ren==0)																		//타워 설치후 타워 레벨업과 UI
	{
		LvRen += g_SecondTime * 80;
		if (LvRen >= 50.0f)
		{
			LvRen = 50.0f;
		}
		m_LVChoice[TowerNumber].SetPosition(IX+ LvRen, IY);
		if (LvRen == 50.0f)
		{
			m_LVChoice[TowerNumber].Frame();
			if (m_LVChoice[TowerNumber].TowerClick && (m_LVTower[TowerNumber].price <= g_Money))
			{
				MakeLvTower = true;
				m_LVTower[TowerNumber].LevelUP = true;
				g_Money -= m_LVTower[TowerNumber].price;
				m_Weapon[TowerNumber].Power = m_LVTower[TowerNumber].power;
			}
		}
	}

	if (BitMapRt.left > m_mousepos.x || m_mousepos.x > BitMapRt.right||
		BitMapRt.top > m_mousepos.y|| m_mousepos.y > BitMapRt.bottom)							//지정 위치가 아닌 다른곳을 클릭하면 아이콘 출력을 지운다
	{
		
			if (Input.KeyCheck(VK_LBUTTON))
			{
				Click = false;
				ClickTime = 0.5f;
				Ren = 0.0f;
				LvRen = 0.0f;
			}
		
	}

	if (MakeTower)																			//타워가 생성되었으면 그 타워에 대한 프레임이 회전한다.
	{
		if (MakeLvTower)
		{
			m_LVTower[TowerNumber].state = TowerStateNumber;
			m_LVTower[TowerNumber].Frame();
			m_LVTower[TowerNumber].SetPosition(CenterPT.x, CenterPT.y);
			ScopeSize(TowerNumber);
			m_LVTower[TowerNumber].GetSphere(CenterPT.x, CenterPT.y, ScopeScaleSize);
		}
		else
		{
			TowerNumber = MakeTowerNum - 1;
			m_Tower[TowerNumber].state = TowerStateNumber;
			m_Tower[TowerNumber].Frame();
			m_Tower[TowerNumber].SetPosition(CenterPT.x, CenterPT.y);
			ScopeSize(TowerNumber);
			m_Tower[TowerNumber].GetSphere(CenterPT.x, CenterPT.y, ScopeScaleSize);
		}

		/*for (auto Effect : m_EffectList)
		{
			Effect->Frame();
		}*/

	}

	for (auto weapon : m_WeaponList)
	{
		weapon->Frame();
	}
	return true;
}

bool TowerPosition::Render()
{
	if (!MakeTower)
	{
		BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCAND);
		BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
		BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);

		if (Click)
		{
			for (int i = 0; i < TOWERCHOICENUM; i++)
			{
				m_Choice[i].Render();
			}
		}
	}
	else
	{
		if (Click)
		{
			Scope[TowerNumber].Render();
			if (!MakeLvTower)
			{
				m_LVChoice[TowerNumber].Render();
			}
		}
		if (MakeLvTower)
		{
			m_LVTower[TowerNumber].Render();
		}
		else
		{
			m_Tower[TowerNumber].Render();
		}
		
		for (auto Weapon1 : m_WeaponList)
		{
			Weapon1->Render();
			if (Weapon1->Buu)
			{
				BuuAttackTime = g_SecondTime;
			}
		}
		
		/*for (auto Effect : m_EffectList)
		{
			Effect->Render();
		}*/
	
	}


	return true;
}

bool TowerPosition::Release()
{
	for (int i = 0; i < TOWERCHOICENUM; i++)
	{
		m_Choice[i].Release();
	}
	for (int i = 0; i < TOWERNUM; i++)
	{
		m_Tower[i].Release();
		Scope[i].Release();
	}
	for (int i = 0; i < WEAPONNUM; i++)
	{
		m_Weapon[i].Release();
	}
	for (auto Effect : m_Effect)
	{
		Effect.Release();
	}
	
	m_Effect.clear();
	m_rtList.clear();
	m_Choice.clear();
	m_Tower.clear();
	Scope.clear();
	m_Weapon.clear();
	m_WeaponList.clear();

	C_Object::Release();
	return true;
}


TowerPosition::TowerPosition()
{
	TowerStateNumber = 0;
	Ren = 0.0f;
	LvRen = 0.0f;
	MakeTowerNum = 0;
	MakeTower = false;
	Click = false;
	TowerNumber = 0;
	ClickTime = 0.3f;
	MakeLvTower = false;
	MakeLVTowerNum = 5;
	BuuAttackTime = 1.1f;
}


TowerPosition::~TowerPosition()
{
}

void TowerPosition::Weaponspeed(int i)
{
	switch (i)
	{
	case 0:
		WEAPONSPEED = 700.0f;
		break;
	case 1:
		WEAPONSPEED = 450.0f;
		break;
	case 2:
		WEAPONSPEED = 600.0f;
		break;
	case 3:
		WEAPONSPEED = 600.0f;
		break;
	}
}

void TowerPosition::TowerWeaponrt(int i)
{
	switch (i)
	{
	case 0:
		TowerWeaponRT = { 36, 322, 42, 14 };
		break;
	case 1:
		TowerWeaponRT = { 971,113,74,60 };
		break;
	case 2:
		TowerWeaponRT = { 36, 322, 42, 14 };
		break;
	case 3:
		TowerWeaponRT = { 36, 322, 42, 14 };
		break;
	}
}

void TowerPosition::ScopeSize(int i)
{
	switch (i)
	{
	case 0:
		ScopeScaleSize = 190.0f;
		break;

	case 1:
		ScopeScaleSize = 190.0f;
		break;

	case 2:
		ScopeScaleSize = 200.0f;
		break;

	case 3:
		ScopeScaleSize = 230.0f;
		break;
	}
}

void TowerPosition::TowerChoicert(int i)
{
	switch (i)
	{
	case 0:
		ChoiceRt = { 48,35,40,70 };
		break;
	case 1:
		ChoiceRt = { 46,151,40,70 };
		break;
	case 2:
		ChoiceRt = { 272,39,40,70 };
		break;
	case 3:
		ChoiceRt = { 276,149,40,70 };
		break;
	}
}

void TowerPosition::TowerLVChoicert(int i)
{
	switch (i)
	{
	case 0:
		ChoiceRt = { 135,31,42,69 };
		break;
	case 1:
		ChoiceRt = { 136,148,42,69 };
		break;
	case 2:
		ChoiceRt = { 272,39,40,70 };
		break;
	case 3:
		ChoiceRt = { 348,145,40,70 };
		break;
	}
}

void TowerPosition::TowerRtsrc()				//타워 소스
{
	m_rtList[0][0] = { 199,302,35,51 };
	m_rtList[0][1] = { 115,270,71,60 };
	m_rtList[0][2] = { 39,321,42,66 };
	m_rtList[0][3] = { 86,339,36,51 };

	m_rtList[1][0] = { 866,118,36,45 };
	m_rtList[1][1] = { 847,182,71,60 };
	m_rtList[1][2] = { 931,124,43,48 };

	m_rtList[2][0] = { 633,42,35,47 };
	m_rtList[2][1] = { 381,30,65,58 };

}

void TowerPosition::LVTowerRtsrc()				//타워 레벨 소스
{
	m_rtList[0][0]={178, 457, 69, 61};
	m_rtList[0][1] = { 183,386,66,58 };
	m_rtList[0][2] = { 34,413,52,48 };
	m_rtList[0][3] = { 34,473,51,48 };

	m_rtList[1][0] = { 760,29,69,61 };
	m_rtList[1][1] = { 845,32,64,58 };
	m_rtList[1][2] = { 946,38,45,49 };

	m_rtList[2][0] = { 448,32,64,55 };
	m_rtList[2][1] = { 518,32,64,55 };

}

void TowerPosition::EffectSrc()								//이펙트 소스
{
	m_EffectrtList[0][0] = { 156,5347,40,39 };
	m_EffectrtList[0][1] = { 238,5331,72,73 };
	m_EffectrtList[0][2] = { 337,5330,72,73 };
	m_EffectrtList[0][3] = { 434,5331,72,73 };
	m_EffectrtList[0][4] = { 282,1945,47,46 };

	m_EffectrtList[1][0] = { 430,481,42,38 };
	m_EffectrtList[1][1] = { 546,466,72,73 };
	m_EffectrtList[1][2] = { 665,456,94,93 };
	m_EffectrtList[1][3] = { 773,447,125,106 };
	m_EffectrtList[1][4] = { 1,580,125,106 };
	m_EffectrtList[1][5] = { 127,578,125,106 };
	m_EffectrtList[1][6] = { 254,577,132,106 };

	m_EffectrtList[2][0] = { 11,215,43,21 };
	m_EffectrtList[2][1] = { 74,212,50,27 };
	m_EffectrtList[2][2] = { 133,209,56,32 };
	m_EffectrtList[2][3] = { 201,208,56,32 };
	m_EffectrtList[2][4] = { 343,209,44,32 };
	m_EffectrtList[2][5] = { 407,208,44,36 };
	m_EffectrtList[2][6] = { 468,210,44,36 };

}

