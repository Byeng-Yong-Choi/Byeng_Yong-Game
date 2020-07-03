#include "CBYCore.h"

HDC g_ScreenDC;
HDC g_offScreenDC;


bool CBYCore::Init()
{
	return true;
}
bool CBYCore::Frame()
{
	return true;
}
bool CBYCore::Render()
{
	return true;
}
bool CBYCore::Release()
{
	return true;
}


bool CBYCore::GameRun()
{
	GetGameRun = CoreInit();											//런함수 실행시 Init실행하고 Frame과 Render함수를 실행 어떠한 이벤트로 종료시 Release로 메모리를 비워주고 종료한다.
	if (GetGameRun == false)
	{
		return false;
	}

	MSG msg;															//메세지를 받을 MSG 선언
	ZeroMemory(&msg, sizeof(msg));
	while (GetGameRun)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))				//PeekMessage는 메세지가 오는지 대기상태없이 계속 돌아가며 발생시1 없을시 0을 반환
		{															//그러므로 메세지큐에 메세지가 쌓여 있으면 계속 메세지 처리하다가 없을때 Frame과 Render가 돌아가 1프레임이 돌아가는 것

			TranslateMessage(&msg);									//메세지큐에 있는 메세지를 받아서 키보드의 눌림이 발생시 WN_CHAR로 가공해서 메세지큐에 넣는다
			DispatchMessage(&msg);									//WndProc로 메세지큐에있는 메세지를 넘겨준다.
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			if (CoreFrame() == false)
			{
				return false;
			}
			if (CoreRender() == false)
			{
				return false;
			}
		}
	}
	if (CoreRelease() == false)
	{
		return false;
	}
	return true;
}

bool  CBYCore::CoreInit()
{
	if (time.Init()|| Input.Init())
	{
		GetGameRun = true;
		ScreenDC = GetDC(g_hwnd);														//윈도우 화면에 띄울 DC를 얻기위해 윈도우의 핸들정보를 받아 ScreenDC를 생성
		offScreenDC = CreateCompatibleDC(ScreenDC);										//더블 버퍼링 진행을 위해 offScreenDC를 ScreenDC와 호환되는 DC로 생성
		offScreenDCBitMap=CreateCompatibleBitmap(ScreenDC, Winrt.right, Winrt.bottom);	//offScreen에 띄울 비트맵핸들을 생성
		SelectObject(offScreenDC, offScreenDCBitMap);									//offScreenDC에 띄우기 위해 offScreen비트맵 핸들을 SelectObject한다

		COLORREF BkColor = RGB(125, 126, 200);												// 윈도우에 생상을 표현하기 위한 변수 선언
		BackColor = CreateSolidBrush(BkColor);											// 윈도우에 뿌릴 색의 물감핸들을 생성한다
		SelectObject(offScreenDC, BackColor);											// offScreenDC에 띄우기 위해 물감핸들을 SelectObject한다.

		g_ScreenDC = ScreenDC;															//다른 클래스에서 이용하기 편하게 전역으로 둔다
		g_offScreenDC = offScreenDC;													//다른 클래스에서 이용하기 편하게 전역으로 둔다

		return Init();
	}
	else
	return false;
};
bool  CBYCore::CoreFrame()
{
	if (time.Frame())
	{
		if (Input.Frame())
		{
			Bitmap.Frame();
			return Frame();
		}
	}
	else
		return false;
};
bool CBYCore::PreRender()													//렌더가 진행된후 다음 렌더로 넘어갈때 화면을 깨끗하게 배경으로 클리어해주는 역할
{
	PatBlt(offScreenDC, 0, 0, Winrt.right, Winrt.bottom, PATCOPY);		//PatBlt는 지정된화면을 DC의 패턴(브러쉬)로 칠한다
	return true;
};

bool  CBYCore::CoreRender()
{
	PreRender();
	Render();
  	time.Render();
	PostRender();
	return true;

};

bool CBYCore::PostRender()															//저장된 g_offScreenDC정보를 넘겨받아 메인 DC에서 출력하는것
{
	BitBlt(ScreenDC,0,0, Winrt.right, Winrt.bottom,g_offScreenDC,0,0, SRCCOPY);		//g_offScreenDC에 입력된 버퍼내용을 ScreenDC가 Copy하여 출력한다.
	return true;
};

bool  CBYCore::CoreRelease()
{
	DeleteObject(BackColor);
	DeleteObject(offScreenDCBitMap);
	ReleaseDC(g_hwnd, ScreenDC);
	ReleaseDC(g_hwnd, offScreenDC);
	time.Release();
	Input.Release();
	Release();
	return true;
};

CBYCore::CBYCore()
{
	GetGameRun = false;
}


CBYCore::~CBYCore()
{
}
