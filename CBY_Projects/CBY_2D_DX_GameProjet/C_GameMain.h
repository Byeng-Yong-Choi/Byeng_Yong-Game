#pragma once
#include "C_Clientsrc.h"				//��Ŭ��� ���� �߿�
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
	//////����ȭ��/////
	float StartUppos;
	float StartDownpos;		//����ȭ�� ��ġ
	bool CreateWindowID;
	bool START;
	bool Login;//���� ȭ��� �α��� ����
	bool m_bLose;
	int m_iStartSound;
	int m_iLobbySound;
	HWND m_hEdit[5];
	HWND m_hStatic[5];
	HWND m_hButton[3];			//�α��� â
	HWND m_hCreateID;			//ȸ������ ������ â
	HINSTANCE m_IDhInstance;
	//////////////////////

public:
	bool m_bGame;			//�������� ���� �Ǵ�.

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	bool GameLose();
	
public:
	HRESULT CreateDXrc();		//��������� DX�ҽ��� ������ ����
	//HRESULT	DeleteDXrc();		//��������� DX�ҽ� ������ ���� ����

public:							//������ and ����
	void CreateLogin();
	void CreateIDWindow();
	void LoginResult();
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);		//���� Ŭ���̾�Ʈ ��ü ���ν���

public:
	C_GameMain();
	~C_GameMain();
};

CBY_RUN(C_GameMain, C_DX, CBY_DX);

