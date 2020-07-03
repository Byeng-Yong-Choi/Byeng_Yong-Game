#pragma once
#include "C_Clientsrc.h"				//인클루드 순서 중요
#include "C_InGame.h"
#include "C_Start.h"
#include "C_lobby.h"
#include "C_Lose.h"

class C_GameMain:public CBYCore
{
public:
	std::shared_ptr<C_InGame> m_InGame;
	std::shared_ptr<C_Clientsrc> m_Client;
	std::shared_ptr<C_Start> m_start;
	std::shared_ptr<C_lobby> m_Lobby;
	std::shared_ptr<C_Lose> m_Lose;

public:
	//////시작화면/////
	float StartUppos;
	float StartDownpos;		//시작화면 위치
	bool CreateWindowID;
	bool START;
	bool Login;//시작 화면과 로그인 유무
	bool m_bLose;
	int m_iStartSound;
	int m_iLobbySound;
	HWND m_hEdit[5];
	HWND m_hStatic[5];
	HWND m_hButton[3];			//로그인 창
	HWND m_hCreateID;			//회원가입 윈도우 창
	HINSTANCE m_IDhInstance;
	//////////////////////

public:
	bool m_bGame;			//게임진행 여부 판단.

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	bool GameLose();
	
public:
	HRESULT CreateDXrc();		//리사이즈시 DX소스의 변경후 생성
	//HRESULT	DeleteDXrc();		//리사이즈시 DX소스 변경을 위해 삭제

public:							//윈도우 and 서버
	void CreateLogin();
	void CreateIDWindow();
	void LoginResult();
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);		//게임 클라이언트 본체 프로시져

public:
	C_GameMain();
	~C_GameMain();
};

CBY_RUN(C_GameMain, C_DX, CBY_DX);

