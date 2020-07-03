#include "C_lobby.h"
HRESULT C_lobby::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;

	m_pd3dDevice = pd3dDevice;
	m_pContext = Context;
	m_Lobby = std::make_shared<CShapeObject>();
	m_Lobby->Create(m_pd3dDevice, m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/start/lobby.png", "VSmat", "PS");
	
	return hr;
}

bool C_lobby::Render()
{
	m_Lobby->Render();

	return true;
}

C_lobby::C_lobby()
{
}


C_lobby::~C_lobby()
{
}
