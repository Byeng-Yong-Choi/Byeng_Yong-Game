#include "C_GameMain.h"
#include "CBY_Input.h"
C_GameMain* g_GameMain = nullptr;

LRESULT CALLBACK WndProcNew(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_DESTROY:
		g_GameMain->CreateWindowID = false;
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 4000:
			SendMessageA(g_GameMain->m_hEdit[2], WM_GETTEXT, sizeof(g_GameMain->m_Client->m_szID), (LPARAM)g_GameMain->m_Client->m_szID);
			SendMessageA(g_GameMain->m_hEdit[3], WM_GETTEXT, sizeof(g_GameMain->m_Client->m_szPS), (LPARAM)g_GameMain->m_Client->m_szPS);
			std::stringstream pack;
			std::stringstream aa;
			pack << g_GameMain->m_Client->m_szID << std::endl << g_GameMain->m_Client->m_szPS;
			aa = MakePacket(PACKET_CREATID, &pack);
			USERPAKET upack;
			aa >> upack;
			upack.ph.ioType = INDIVIDUAL;
			{
				C_Lock Lock(&g_GameMain->m_Client->m_Lock);
				g_GameMain->m_Client->m_SendPool.push_back(upack);
			}
			ZeroMemory(g_GameMain->m_Client->m_szID, sizeof(g_GameMain->m_Client->m_szID));
			ZeroMemory(g_GameMain->m_Client->m_szPS, sizeof(g_GameMain->m_Client->m_szPS));
			break;
		}
		break;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

bool C_GameMain::Init()
{
	CDX::ApplyBS(m_pContext, CDX::CDxState::g_pAlpahBlend);
	CDX::ApplySS(m_pContext, CDX::CDxState::g_pSampler);
	CDX::ApplyRS(m_pContext, CDX::CDxState::g_pRSSold);
	CDX::ApplyDSS(m_pContext, CDX::CDxState::g_pDSSDisable);

	I_SOUND.Init();
	m_iStartSound = I_SOUND.Load(L"start.mp3", false, 0.5f);
	m_iLobbySound = I_SOUND.Load(L"lobby.mp3", false, 0.5f);

	m_InGame= std::make_shared<C_InGame>();
	m_InGame->Create(m_pd3dDevice, m_pContext);
	m_InGame->SetCamera(m_pMainCamera);

	m_start = std::make_shared<C_Start>();
	m_start->Create(m_pd3dDevice, m_pContext);

	m_Lobby = std::make_shared<C_lobby>();
	m_Lobby->Create(m_pd3dDevice, m_pContext);

	m_Lose = std::make_shared<C_Lose>();
	m_Lose->Create(m_pd3dDevice, m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/UI/Lose.png", "VSmat", "PS");

	m_Client = std::make_shared<C_Clientsrc>();
	m_Client->Init();
	CreateLogin();				//로그인 에디트와 버튼 생성


	return true;
}

bool C_GameMain::Frame()
{
	I_SOUND.Frame();
	m_Client->Run();

	if (!START)				//로그인창이 아닐때
	{
		if (!m_bGame)		//게임이 진행중이 아닌 로비일때
		{
			I_SOUND.list[m_iLobbySound]->Play(I_SOUND.m_pSystem);
			if (I_Input.KeyCheck(VK_SPACE))
			{
				I_SOUND.list[m_iLobbySound]->Stop();
				m_bLose = false;
				m_bGame = true;
			}
		}
		else
		{
			if (GameLose())
			{
				m_bLose = true;
				m_Lose->Frame();
				m_Lose->SetMatrix(nullptr, nullptr, &m_pMainCamera->m_OrthoProj);
				if (m_Lose->bContinue)
				{
					m_bGame = false;
					m_InGame->GameSet();
				}
			}
			if (m_bGame&&!m_bLose)
			{
				m_InGame->Frame();
			}
		}
	}
	else
	{
		I_SOUND.list[m_iStartSound]->Play(I_SOUND.m_pSystem);
		if (m_Client->CreateID)
		{
			DestroyWindow(m_hCreateID);
			m_Client->CreateID = false;
		}
		D3DXMATRIX startup, startdown;
		D3DXMatrixIdentity(&startup);
		D3DXMatrixIdentity(&startdown);
		LoginResult();
		if (Login)
		{
			if (1.0f > g_SecondTime)		//메세지 박스때문에 올라간 시간이 다시 돌아오기까지의 조건
			{
				StartUppos += 120.0f * g_SecondTime;
				StartDownpos -= 120.0f * g_SecondTime;
				if (StartUppos > 360.0f && StartDownpos < -360.0f)
				{
					StartUppos = 0.0f;
					StartDownpos = 0.0f;
					START = false;
					I_SOUND.list[m_iStartSound]->Stop();
				}
			}
		}
		startup._42 = StartUppos;
		startdown._42 = StartDownpos;
		m_start->m_Start[0]->SetMatrix(&startup, nullptr, &m_pMainCamera->m_OrthoProj);
		m_start->m_Start[1]->SetMatrix(&startdown, nullptr, &m_pMainCamera->m_OrthoProj);
	}
	return true;
}

bool C_GameMain::Render()
{
	if (!START)
	{
		if (!m_bGame)
		{
			m_Lobby->Render();
		}
		else
		{
			if (GameLose())
			{
				m_Lose->Render();
			}
			else
			{
				m_InGame->Render();
			}
		}
	}
	else
	{
		m_Lobby->Render();
		m_start->Render();
	}
	return true;
}

bool C_GameMain::Release()
{
	m_InGame->Release();
	return true;
}

bool C_GameMain::GameLose()
{
	return m_InGame->GameLose();
}

HRESULT C_GameMain::CreateDXrc()
{
	HRESULT hr = S_OK;

	for (int i = 0; i < 2; i++)
	{
		DestroyWindow(m_hEdit[i]);
		DestroyWindow(m_hStatic[i]);
		DestroyWindow(m_hButton[i]);
	}

	CreateLogin();

	m_InGame->CreateDXrc();
	CBYCore::CreateDXrc();
	return hr;
}


C_GameMain::C_GameMain()
{
	StartUppos = 0.0f;
	StartDownpos = 0.0f;
	START = true;				//시작 상태값
	Login = false;				//로그인 상태값
	CreateWindowID = false;		//회원가입 윈도우 열림 유무
	m_bGame = false;			//인게임 여부 판단

	m_bLose = false;

	g_GameMain = this;
}


C_GameMain::~C_GameMain()
{
}

void C_GameMain::CreateLogin()
{
	//윈도우 에디트
	DWORD sytle = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER;
	m_hEdit[0] = CreateWindow(_T("edit"), NULL, sytle, (Winrt.right / 2) - 100, (Winrt.bottom / 2) + (Winrt.bottom / 4), 200, 25, g_hwnd, (HMENU)100, g_hInstance, NULL);
	m_hEdit[1] = CreateWindow(_T("edit"), NULL, sytle, (Winrt.right / 2) - 100, (Winrt.bottom / 2) + (Winrt.bottom / 4) + 30, 200, 25, g_hwnd, (HMENU)200, g_hInstance, NULL);

	//윈도우 스태틱
	sytle = WS_CHILD | WS_VISIBLE | SS_CENTER;
	m_hStatic[0] = CreateWindow(L"STATIC", L"계    정", sytle, (Winrt.right / 2) - 180, (Winrt.bottom / 2) + (Winrt.bottom / 4), 80, 25, g_hwnd, (HMENU)-1, g_hInstance, NULL);
	m_hStatic[1] = CreateWindow(L"STATIC", L"비밀번호", sytle, (Winrt.right / 2) - 180, (Winrt.bottom / 2) + (Winrt.bottom / 4) + 30, 80, 25, g_hwnd, (HMENU)-2, g_hInstance, NULL);

	// 윈도우 컨트롤(button)
	sytle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
	m_hButton[0] = CreateWindow(L"button", L"회원가입", sytle, (Winrt.right / 2) - 100, (Winrt.bottom / 2) + (Winrt.bottom / 4) + 60, 100, 25, g_hwnd, (HMENU)1000, g_hInstance, NULL);
	m_hButton[1] = CreateWindow(L"button", L"로그인", sytle, (Winrt.right / 2), (Winrt.bottom / 2) + (Winrt.bottom / 4) + 60, 100, 25, g_hwnd, (HMENU)2000, g_hInstance, NULL);
}

void C_GameMain::CreateIDWindow()
{
	WNDCLASSEXW wcex;
	wcex.lpfnWndProc = &WndProcNew;
	wcex.lpszClassName = L"new";
	wcex.hInstance = m_IDhInstance;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_DEMOWINDOW);
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	RegisterClassExW(&wcex);

	m_hCreateID = CreateWindow(L"new", L"회원 가입", WS_OVERLAPPEDWINDOW, Winrt.right / 4, Winrt.bottom / 4, Winrt.right / 2, Winrt.bottom / 2, g_hwnd, (HMENU)0, m_IDhInstance, NULL);
	ShowWindow(m_hCreateID, SW_SHOW);
	//////////////////////////////////////////////////////////////////////// 위로는 회원가입 윈도우 새창을 띄운 부분이다

	D3DXVECTOR2 rectsize = { (float)Winrt.right / 2 ,(float)Winrt.bottom / 2 };
	DWORD sytle = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER;
	m_hEdit[2] = CreateWindow(_T("edit"), NULL, sytle, (rectsize.x / 2) - 100, (rectsize.y / 4), 200, 25, m_hCreateID, (HMENU)100, m_IDhInstance, NULL);
	m_hEdit[3] = CreateWindow(_T("edit"), NULL, sytle, (rectsize.x / 2) - 100, (rectsize.y / 4) + 30, 200, 25, m_hCreateID, (HMENU)200, m_IDhInstance, NULL);
	m_hEdit[4] = CreateWindow(_T("edit"), NULL, sytle, (rectsize.x / 2) - 100, (rectsize.y / 4) + 60, 200, 25, m_hCreateID, (HMENU)200, m_IDhInstance, NULL);

	sytle = WS_CHILD | WS_VISIBLE | SS_CENTER;
	m_hStatic[2] = CreateWindow(L"STATIC", L"계    정", sytle, (rectsize.x / 2) - 180, (rectsize.y / 4), 80, 25, m_hCreateID, (HMENU)-1, m_IDhInstance, NULL);
	m_hStatic[3] = CreateWindow(L"STATIC", L"비밀번호", sytle, (rectsize.x / 2) - 180, (rectsize.y / 4) + 30, 80, 25, m_hCreateID, (HMENU)-2, m_IDhInstance, NULL);
	m_hStatic[4] = CreateWindow(L"STATIC", L"닉네임", sytle, (rectsize.x / 2) - 180, (rectsize.y / 4) + 60, 80, 25, m_hCreateID, (HMENU)-2, m_IDhInstance, NULL);

	sytle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
	m_hButton[2] = CreateWindow(L"button", L"가    입", sytle, (rectsize.x / 2), (rectsize.y / 4) + 90, 100, 25, m_hCreateID, (HMENU)4000, m_IDhInstance, NULL);

	CreateWindowID = true;
}

LRESULT C_GameMain::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1000:
			if (CreateWindowID == false)
				CreateIDWindow();
			break;

		case 2000:
			/*SendMessageA(m_hEdit[0], WM_GETTEXT, sizeof(m_Client->m_szID), (LPARAM)m_Client->m_szID);
			SendMessageA(m_hEdit[1], WM_GETTEXT, sizeof(m_Client->m_szPS), (LPARAM)m_Client->m_szPS);
			std::stringstream pack;
			std::stringstream aa;
			pack << m_Client->m_szID << std::endl << m_Client->m_szPS;
			aa = MakePacket(PACKET_LOGIN, &pack);
			USERPAKET upack;
			aa >> upack;
			upack.ph.ioType = INDIVIDUAL;
			{
				C_Lock Lock(&m_Client->m_Lock);
				m_Client->m_SendPool.push_back(upack);
			}
			ZeroMemory(m_Client->m_szID, sizeof(m_Client->m_szID));
			ZeroMemory(m_Client->m_szPS, sizeof(m_Client->m_szPS));*/
			m_Client->Login = true;
			LoginResult();
			break;
		}
		break;

	case WM_SIZE:
		if (SIZE_MINIMIZED != wParam)
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			GetClientRect(g_hwnd, &Winrt);
			if (!ResizeDevice(width, height))
			{
				while (1)
				{
					if (ResizeDevice(width, height))
					{
						break;
					}
				}
			}
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);

}



void C_GameMain::LoginResult()
{
	if (m_Client->Login)
	{
		Login = true;
		DestroyWindow(m_hEdit[0]);
		DestroyWindow(m_hEdit[1]);
		DestroyWindow(m_hStatic[0]);
		DestroyWindow(m_hStatic[1]);
		DestroyWindow(m_hButton[0]);
		DestroyWindow(m_hButton[1]);
	}
}