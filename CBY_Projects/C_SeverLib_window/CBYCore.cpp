#include "CBYCore.h"

HRESULT CBYCore::CreateDXrc()
{
	HRESULT hr = S_OK;



	return hr;
}

HRESULT	CBYCore::DeleteDXrc()
{
	HRESULT hr = S_OK;

	float fAspect = (float)Winrt.right / (float)Winrt.bottom;

	m_pMainCamera->CreateProjMatrix(
		m_pMainCamera->m_fNear,
		m_pMainCamera->m_fFar,
		m_pMainCamera->m_fFovY,
		fAspect);

	return hr;
}

void CBYCore::SendMSGClick()
{

}

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
	GetGameRun = true;
	m_Timer.Init();
	if (FAILED(DeviceInit()))
	{
		return false;
	}
	CDX::CDxState::SetState(m_pd3dDevice);
	m_DebugCamera.SetViewProj();
	m_pMainCamera = &m_DebugCamera;
	return Init();
	
};
bool  CBYCore::CoreFrame()
{
	m_Timer.Frame();
	m_pMainCamera->Frame();
	if(Frame())
	{
		return true;
	}
	else
		return false;
};
bool CBYCore::PreRender()													//렌더가 진행된후 다음 렌더로 넘어갈때 화면을 깨끗하게 배경으로 클리어해주는 역할
{
	float ClearColor[] = { 0.0f, 0.7f, 1.0f,1 };
	m_pContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);							//더블 버퍼링에서 preRender에 해당하는 부분
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
};

bool  CBYCore::CoreRender()
{
	//PreRender();
	Render();
	//PostRender();
	return true;

};

bool CBYCore::PostRender()															
{
	m_pSwapChain->Present(0, 0);																//최종적으로 윈도우의 영역에 그려주는 함수이다.
	return true;
};

bool  CBYCore::CoreRelease()
{
	m_Timer.Release();
	Release();
	DeviceRelease();
	return true;
};

CBYCore::CBYCore()
{
	GetGameRun = false;
}


CBYCore::~CBYCore()
{
}
