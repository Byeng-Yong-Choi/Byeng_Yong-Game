#pragma once
#include "Cstd_ex.h"
#include "gamestd.h"
#include "TowerPosition.h"
#include "TowerScope.h"
#include "TowerLose.h"
#include "TowerUI.h"
#include "GameMgr.h"
#include "GameRank.h"

float g_Money;					//게임 재화

typedef std::vector<RECT> RTLIST;

enum GameState
{
	GAME_START=0,
	GAME_QUIT,
	GAME_RANK,
	GAME_COUNT
};

class GameMain :public CBYCore
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Relese();
	void Position(int i);
	void UIPosition(int i);
	void MonsterRt();
	void UIRt();
	bool gamestart;
	bool GAMELOSE;
	void StartRtsrc(int i);

public:
	std::vector<C_Tower> m_Tower;
	std::vector<Char> m_Hero;
	std::vector<BackGround> m_Map;
	std::vector<TowerPosition> m_Position;
	std::list<C_Monster*> m_Monster;
	std::vector<TowerUI*> m_UI;
	std::vector<TowerUI> m_LoseUI;
	int EffectSound[3];
	//C_SoundManager m_sound;
	C_Monster m_copyMonster;
	Health m_HP;
	GameRank m_Rank;
	std::vector<TowerLose> m_Start;
	TowerLose lose;
	
public:
	int startmonsterindex;
	int monsterindex;
	int Levelmonster;							//게임 몬스터의 레벨에 따른 출현 수 조정
	int lvmonsternumber;					//레벨에 따른 스타트 인덱스의 증가값
	int Level;								//현재 레벨
	int Life;
	int hphit;
	float StartTime;				//몬스터 출현 텀
	float StartClickTime;			//스타트를 누르고 메인 스테이지 화면이 뜨기까지 시간
	float NextLevelTime;
	TCHAR TextBuffer[256];				//게임 재화 출력 텍스트 버퍼

	std::vector<RTLIST> m_rtList;
	std::vector<RTLIST> m_UirtList;
	fPOINT fPt;								//타워 포지션 위치를 받을 포인트 변수
	RECT MapRt = { 0,0,1280,1024 };
	RECT PositionRt = { 145,147,58,58 };
	//RECT MonsterRt = { 2,107,31,44 };
	RECT TowerRt;
	RECT TowerChoiceRt;
	RECT StartRt;
	RECT TextRt;
	fPOINT MonsterPosition = { 25.0f,422.0f };
	


public:
	GameMain();
	~GameMain();
};

bool GameMain::Init()
{

	//TextRt = { 1220,15,Winrt.right,Winrt.bottom };		//돈 화면 출력 rt

	/*EffectSound[0] = m_sound.Load(L"sound/son.wav", false);
	EffectSound[1] = m_sound.Load(L"sound/ve.wav", false);
	EffectSound[2] = m_sound.Load(L"sound/cold.wav", false);
	EffectSound[0] = m_sound.Load(L"sound/click.wav", false);*/

	RECT rankrt = { 27,1,460,357 };
	m_Rank.Init();
	m_Rank.GetRECT(rankrt);
	m_Rank.SetPosition(389.0f, 119.0f);
	m_Rank.Load(L"bitmap/board.bmp", L"bitmap/boardmask.bmp");

	TextRt = Winrt;
	TextRt.top += 15;

	g_Money = STARTMONEY;

	m_Start.resize(3);
	m_Map.resize(1);
	m_Tower.resize(POSITIONNUM);
	m_UI.resize(GAME_COUNT);
	m_LoseUI.resize(2);
	m_rtList.resize(1);
	m_rtList[0].resize(6);
	m_UirtList.resize(2);
	m_UirtList[0].resize(3);
	m_UirtList[1].resize(3);

	RECT LoseUIRt1 = { 14,326,199,55 };
	RECT LoseUIRt2 = { 14,35,199,55 };

	m_LoseUI[0].Init();
	m_LoseUI[0].GetRECT(LoseUIRt1);
	m_LoseUI[0].SetPosition(800.0f, 571.0f);
	m_LoseUI[0].Load(L"bitmap/UI.bmp", L"bitmap/UImask.bmp");

	m_LoseUI[1].Init();
	m_LoseUI[1].GetRECT(LoseUIRt2);
	m_LoseUI[1].SetPosition(1000.0f, 571.0f);
	m_LoseUI[1].Load(L"bitmap/UI.bmp", L"bitmap/UImask.bmp");

	

	UIRt();
	for (int i = 0; i < GAME_COUNT-1; i++)
	{
		m_UI[i] = new TowerUI;
		m_UI[i]->Init();
		UIPosition(i);
		m_UI[i]->GetRECT(m_UirtList[i]);
		m_UI[i]->SetPosition(fPt.x, fPt.y);
		m_UI[i]->Load(L"bitmap/UI.bmp", L"bitmap/UImask.bmp");
		m_UI[i]->splite = true;
	}
	RECT RankRT = { 88,40,55,56 };
	m_UI[GAME_RANK] = new TowerUI;
	m_UI[GAME_RANK]->Init();
	UIPosition(GAME_RANK);
	m_UI[GAME_RANK]->GetRECT(RankRT);
	m_UI[GAME_RANK]->SetPosition(fPt.x, fPt.y);
	m_UI[GAME_RANK]->Load(L"bitmap/rank.bmp", L"bitmap/rankmask.bmp");


	MonsterRt();
	
	for (int i = 0; i < POSITIONNUM; i++)
	{
		Position(i);
		m_Tower[i].GetRECT(PositionRt);
		m_Tower[i].SetPosition(fPt.x, fPt.y);
		m_Tower[i].Init();
		m_Tower[i].Load(L"bitmap/TowerPosition.bmp",L"bitmap/TowerPositionmask.bmp");
	}
	m_copyMonster.GetRECT(m_rtList[0], 0.3f);
	m_copyMonster.SetPosition(MonsterPosition.x, MonsterPosition.y - 20);
	m_copyMonster.Init();
	m_copyMonster.Load(L"bitmap/BlackPigcolor.bmp", L"bitmap/BlackPigcolormask.bmp");
	
	/*for (int i=0; i< MONSTERNUM; i++)
	{
		C_Monster* monster = new C_Monster;
		monster->index = i;
		monster->GetRECT(m_rtList[0],0.3f);
		monster->SetPosition(MonsterPosition.x, MonsterPosition.y-20);
		monster->Init();
		monster->Load(L"bitmap/BlackPigcolor.bmp", L"bitmap/BlackPigcolormask.bmp");
		m_Monster.push_back(monster);
	}*/

	RECT HPrt = { 0,0,Winrt.right,15 };
	m_HP.GetRECT(HPrt);
	m_HP.SetPosition(0, 0);
	m_HP.Make = 1;
	m_HP.HPNUM = GAMELIFE;
	m_HP.Init();

	RECT loseRt= { 0,0,Winrt.right,Winrt.bottom };
	lose.GetRECT(loseRt);
	lose.IX = 0;
	lose.IY = 0;
	lose.ShowTime = 50.0f;
	lose.Init();
	lose.Load(L"bitmap/lose.bmp", L"bitmap/losemask.bmp");

	//StartRtsrc(0);
	//m_Start[0].GetRECT(StartRt);
	//m_Start[0].IX = 286;
	//m_Start[0].IY = 153;
	////m_Start[0].ShowTime = 20.0f;
	//m_Start[0].Init();
	//m_Start[0].Death = 1;
	//m_Start[0].Load(L"bitmap/title.bmp", L"bitmap/titlemask.bmp");
	StartRtsrc(3);
	m_Start[0].GetRECT(StartRt);
	m_Start[0].IX = 376;
	m_Start[0].IY = 153;
	//m_Start[0].ShowTime = 20.0f;
	m_Start[0].Init();
	m_Start[0].Death = 1;
	m_Start[0].Load(L"bitmap/jeje.bmp", L"bitmap/jejemask.bmp");

	StartRtsrc(1);
	m_Start[1].GetRECT(StartRt);
	m_Start[1].IX = 136;
	m_Start[1].IY = 300;
	m_Start[1].ShowTime = 80.0f;
	m_Start[1].Init();
	m_Start[1].Load(L"bitmap/gokucolor.bmp", L"bitmap/gokucolormask.bmp");
	
	StartRtsrc(2);
	m_Start[2].GetRECT(StartRt);
	m_Start[2].IX = 859;
	m_Start[2].IY = 300;
	m_Start[2].ShowTime = 80.0f;
	m_Start[2].Init();
	m_Start[2].Load(L"bitmap/vegita.bmp", L"bitmap/vegitamask.bmp");


	m_Map[0].Init();
	m_Map[0].Load(L"bitmap/Level1map.bmp");
	m_Map[0].GetRECT(MapRt);
	m_Map[0].SetPosition(0, 0);

	return true;
}

bool GameMain::Frame()
{
	if (GAMELOSE)
	{
		g_gametime = 0.0f;
		StartTime = 0;
		Levelmonster = 1;
		NextLevelTime = 5.0f;
		startmonsterindex = 0;
		monsterindex = 0;
		lvmonsternumber = -1;
		Level = 0;
		Life = GAMELIFE;
		hphit = 0;
		g_Money = STARTMONEY;
		//StartClickTime = 0.0f;
		


		lose.Frame();
		if (!m_Monster.empty())														//몬스터가 모두 죽었을때 다음 레벨 몬스터 등장
		{
			C_Monster* monnode;
			std::list<C_Monster*>::iterator monster;

			for (monster = m_Monster.begin(); monster != m_Monster.end();)
			{
				monnode = monster.operator*();
				monnode->Release();
				delete monnode;
				monster = m_Monster.erase(monster);
			}
			
			/*for (int i = 0; i < m_Tower.size(); i++)
			{
				m_Tower[i].MakeTower = false;
				m_Tower[i].MakeLvTower = false;
				m_Tower[i].m_Choice[m_Tower[i].TowerNumber].TowerClick = false;
				m_Tower[i].m_LVChoice[m_Tower[i].TowerNumber].TowerClick = false;
			}*/
		}


		for (int i = 0; i < m_LoseUI.size(); i++)
		{
			m_LoseUI[i].Frame();
		}

		if (m_LoseUI[GAME_START].Click)
		{
			m_UI[GAME_START]->Click = false;
			m_LoseUI[GAME_START].Click = false;
			lose.Death = 0;
			lose.AlphaNum = 0;
			lose.alpha.SourceConstantAlpha = 0;
			m_LoseUI[GAME_QUIT].Click = false;
			GAMELOSE = false;
		}
		
		if (m_LoseUI[GAME_QUIT].Click)
		{
			PostMessage(g_hwnd, WM_QUIT, 0, 0);
			return true;
		}

	}
	else if (!m_UI[GAME_START]->Click)
	{
		for (auto UI : m_UI)
		{
			UI->Frame();
		}

		if (m_UI[GAME_RANK]->Click)
		{
			m_Rank.Click = true;
			m_Rank.Frame();
		}

		if (!m_UI[GAME_RANK]->Click)
		{
			m_Rank.Click = false;

		}


		if (m_UI[GAME_QUIT]->Click)
		{

			PostMessage(g_hwnd, WM_QUIT, 0, 0);
			return true;
		}

		for (int i = 0; i < m_Start.size(); i++)
		{
			m_Start[i].Frame();
		}
		g_gametime = 0.0f;
		
	}
	else
	{
		m_UI[GAME_RANK]->Click = false;
		m_Rank.Click = false;
		g_Money += g_SecondTime;

		if (StartClickTime <= 0.2f)
		{
			StartClickTime += g_SecondTime;
		}

		if (m_Monster.empty())														//몬스터가 모두 죽었을때 다음 레벨 몬스터 등장
		{
			NextLevelTime += g_SecondTime;
			if (NextLevelTime > NEXTLEVEL)
			{
				for (int i = 0; i < MONSTERNUM + Levelmonster; i++)
				{
					C_Monster* monster = new C_Monster;
					monster->index = monsterindex++;
					monster->GetRECT(m_rtList[0], 0.3f);
					monster->SetPosition(MonsterPosition.x, MonsterPosition.y - 20);
					monster->Init();
					monster->Load(L"bitmap/BlackPigcolor.bmp", L"bitmap/BlackPigcolormask.bmp");
					monster->HPNUMBER += Levelmonster + lvmonsternumber;							//레벨에 따른 HP증가량
					m_Monster.push_back(monster);
				}
				Levelmonster += MONSTERLEVEL;
				if (Levelmonster > 25)
				{
					Levelmonster = 25;
				}
				StartTime = 0.0f;
				lvmonsternumber++;
				Level++;
				if (Level > 2)
				{
					lvmonsternumber++;
				}
			}
		}
		else
		{
			StartTime += g_SecondTime;
			NextLevelTime = 0;
		}
		m_Map[0].Frame();

		if (StartTime >= MONSTERMAKETIME && startmonsterindex < monsterindex)
		{
			for (auto monster : m_Monster)
			{
				if (monster->index == startmonsterindex)
				{
					monster->SetAction(EVENT_START);
					break;
				}
			}
			StartTime = 0;
			startmonsterindex++;
		}

		C_Monster* monnode;
		std::list<C_Monster*>::iterator monster;

		for (monster = m_Monster.begin(); monster != m_Monster.end();)
		{
			monnode = monster.operator*();
			monnode->Process(m_Tower);
			if (monnode->ok)
			{
				monnode->Release();
				delete monnode;
				monster = m_Monster.erase(monster);
				g_Money += MONSTERDEATHMONEY;
			}
			else if (monnode->End)
			{
				Life--;
				hphit++;
				monnode->Release();
				delete monnode;
				monster = m_Monster.erase(monster);
			}
			else
				monster++;

			if (Life <= 0)
			{
				m_HP.Hit = hphit;
				m_HP.Frame();
				GAMELOSE = true;
				m_Rank.Frame();
				for (int i = 0; i < m_Tower.size(); i++)
				{
					m_Tower[i].MakeTower = false;
					m_Tower[i].MakeLvTower = false;
					m_Tower[i].m_Choice[m_Tower[i].TowerNumber].TowerClick = false;
					m_Tower[i].m_LVChoice[m_Tower[i].TowerNumber].TowerClick = false;
					m_Tower[i].SetAction(EVENT_NULL);
					m_Tower[i].Process(m_Monster);
				}
				return true;
			}
		}

		m_HP.Hit = hphit;
		m_HP.Frame();

		for (int i = 0; i < m_Tower.size(); i++)
		{
			m_Tower[i].Process(m_Monster);
		}

		std::list<TowerEffect*> wf;
		TowerEffect* Effectnode;
		std::list<TowerEffect*>::iterator efiter;

		for (int i = 0; i < m_Tower.size(); i++)
		{
			for (efiter = m_Tower[i].m_EffectList.begin(); efiter != m_Tower[i].m_EffectList.end(); )
			{
				Effectnode = efiter.operator*();
				Effectnode->Frame();
				//Effectnode->SetPosition(I_Col.Center(m_pMonster->BitMapRt));
				if (!Effectnode->EffectShow)
				{
					delete Effectnode;
					efiter = m_Tower[i].m_EffectList.erase(efiter);
				}
				else
				{
					efiter++;
				}
			}
		}

		ZeroMemory(TextBuffer, sizeof(TCHAR) * 256);
		swprintf_s(TextBuffer, L"%0.0f원", g_Money);		//시간이 지나면서 조금씩 돈이 추가되고 그것을 출력할 버퍼셋팅
	}

	

	return true;
}

bool GameMain::Render()
{
	m_Map[0].Render();

	
	if (!m_UI[GAME_START]->Click|| StartClickTime<0.2f)
	{
		for (auto UI : m_UI)
		{
			UI->Render();

		}
		for (int i = 0; i < m_Start.size(); i++)
		{
			m_Start[i].Render();
		}
		
		if (m_Rank.Click)
		{
			m_Rank.Render();
		}

	}
	else
	{
		for (auto Tower : m_Tower)
		{
			Tower.Render();
		}
		for (auto monster : m_Monster)
		{
			monster->Render();
		}
		for (int i = 0; i < m_Tower.size(); i++)
		{
			for (auto Effect : m_Tower[i].m_EffectList)
			{
				Effect->Render();
			}
		}

		if (GAMELOSE)
		{
			lose.Render();
			for (int i = 0; i < m_LoseUI.size(); i++)
			{
				m_LoseUI[i].Render();
			}
		}



		m_HP.Render();
		
		
		SetBkMode(g_offScreenDC, TRANSPARENT);
		SetTextColor(g_offScreenDC, RGB(0, 125, 255));
		DrawText(g_offScreenDC, TextBuffer, -1, &TextRt, DT_RIGHT | DT_VCENTER);
	}

	

	return true;
}

bool GameMain::Relese()
{
	m_HP.Release();
	m_Map[0].Release();

	TowerUI* UInode;
	std::vector<TowerUI*>::iterator ui;

	for (ui = m_UI.begin(); ui != m_UI.end();)
	{
			UInode = ui.operator*();
			UInode->Release();
			delete UInode;
			ui = m_UI.erase(ui);
	}

	for (int i = 0; i < m_Start.size(); i++)
	{
		m_Start[i].Release();
	}

	for (auto monster : m_Monster)
	{
		monster->Release();
	}

	for (auto Tower : m_Tower)
	{
		Tower.Release();
	}

	m_UI.clear();
	lose.Release();
	m_rtList.clear();
	m_UirtList.clear();
	m_Start.clear();
	return true;
}

GameMain::GameMain()
{
	StartTime = 0;
	Levelmonster = 1;
	NextLevelTime = 5.0f;
	startmonsterindex=0;
	monsterindex=0;
	lvmonsternumber = -1;
	Level = 0;
	Life = GAMELIFE;
	hphit = 0;
	gamestart = false;
	StartClickTime = 0.0f;
	GAMELOSE = false;
}


GameMain::~GameMain()
{
}


CBY_RUN(GameMain, C_Game, CBY_Game);

void GameMain::Position(int i)
{
	switch (i)
	{
	case 0:
		fPt = { 180,330 };
		break;
	case 1:
		fPt = { 387,341 };
		break;
	case 2:
		fPt = { 297,223 };
		break;
	case 3:
		fPt = { 469,223 };
		break;
	case 4:
		fPt = { 583,341 };
		break;
	case 5:
		fPt = { 563,591 };
		break;
	case 6:
		fPt = { 703,245 };
		break;
	case 7:
		fPt = { 1061,113 };
		break;
	case 8:
		fPt = { 1075,231 };
		break;
	case 9:
		fPt = { 1063,366 };
		break;
	case 10:
		fPt = { 197,528 };
		break;

	case 11:
		fPt = { 580,118 };
		break;
	}


}

void GameMain::UIPosition(int i)
{
	switch (i)
	{
	case 0:
		fPt = { 561,487 };
		break;
	case 1:
		fPt = { 564,547 };
		break;
	case 2:
		fPt = {1173,11};
		break;
	}


}

void GameMain::MonsterRt()
{
	m_rtList[0][0] = {1, 137, 55, 38};
	m_rtList[0][1] = {78, 202, 55, 38};
	m_rtList[0][2] = {135, 202, 60, 38};
	m_rtList[0][3] = {198, 203, 57, 36};

	m_rtList[0][4] = { 66,412,58,44 };

	m_rtList[0][5] = { 67,351,57,34 };
}

void GameMain::UIRt()
{
	m_UirtList[0][0] = { 14,326,199,55 };
	m_UirtList[0][1] = { 14,411,199,55 };
	m_UirtList[0][2] = { 18,489,199,55 };

	m_UirtList[1][0] = { 14,35,199,55 };
	m_UirtList[1][1] = { 14,127,199,55 };
	m_UirtList[1][2] = { 14,223,199,55 };

	
}


void GameMain::StartRtsrc(int i)
{
	switch (i)
	{
	case 0:
		StartRt = { 183,60,794,112};
		break;
	case 1:
		StartRt = { 0,0,350,404 };
		break;
	case 2:
		StartRt = { 0,0,300,420 };
		break;
	case 3:
		StartRt = { 80,109,673,84 };
		break;
	}
}