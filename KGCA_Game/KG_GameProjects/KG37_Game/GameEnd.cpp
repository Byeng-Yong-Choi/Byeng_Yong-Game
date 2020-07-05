#include "GameEnd.h"
#include "SceneMgr.h"
#include "UI_panel.h"
#include "UI_button.h"
#include "UI_objMgr.h"
#include "SceneMgr.h"
#include "KG_SoundMgr.h"
#include "LightMgr.h"
#include "JH_MapMgr.h"
#include "CBY_GameMgr.h"

KYS::GameEnd::GameEnd()
{
	OutputDebugString(L"create TitleScene");
}

KYS::GameEnd::~GameEnd()
{
}

bool KYS::GameEnd::Init()
{
	std::shared_ptr<KYS::UI_panel> titlePanel = nullptr;

	//	(*_camera)->SetViewProj();
	m_pMainCamera = *_camera;
	m_pMainCamera->m_orthoPos = D3DXVECTOR3(1000, 1000, 1000);
	m_pMainCamera->m_ortholook = D3DXVECTOR3(0, 0, 10);

	SCENE_MGR.SetCameara(m_pMainCamera);

	D3DXMATRIX parentWorld;
	D3DXMatrixIdentity(&parentWorld);

	parentWorld._41 = m_pMainCamera->m_orthoPos.x;
	parentWorld._42 = m_pMainCamera->m_orthoPos.y;
	parentWorld._43 = m_pMainCamera->m_orthoPos.z;

	//titlePanel
	titlePanel = std::make_shared<KYS::UI_panel>();
	titlePanel->Create(_device, _deviceContext, L"../../data/shader/DefaultShader.hlsl", L"../../data/ui/0xe62b9a571100fcc1.dds", "VS", "PS");
	titlePanel->SetPos(D3DXVECTOR3(D3DXVECTOR3(-Winrt.right / 2.0f, Winrt.bottom / 2.0f, 1005)));
	//titlePanel->SetPos(D3DXVECTOR3(1000,1000, 1005));
	//titlePanel->SetScale(D3DXVECTOR3(200,300, 0.0f));
	titlePanel->SetScale(D3DXVECTOR3(Winrt.right / 2.0f, Winrt.bottom / 2.0f, 0.0f));
	titlePanel->SetUiType(UI_TYPE::UI_TYPE_TITLE_PANEL);
	titlePanel->SetParentMatWorld(parentWorld);
	UI_MGR->add(titlePanel);

	return false;
}

bool KYS::GameEnd::Frame()
{
	UI_MGR->Frame();
	SHORT key = I_Input.KeyCheck('1');
	if (key == KEY_PUSH)					//카메라의 이동
	{
		
	}

	return false;
}

bool KYS::GameEnd::Render()
{
	UI_MGR->Render(m_pMainCamera);
	return false;
}

bool KYS::GameEnd::Release()
{
	I_SOUND.Release();
	UI_MGR->ClearUiList();
	return false;
}
