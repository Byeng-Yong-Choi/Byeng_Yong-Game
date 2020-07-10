#include "TitleScene.h"
#include "SceneMgr.h"
#include "UI_panel.h"
#include "UI_button.h"
#include "UI_objMgr.h"
#include "SceneMgr.h"
#include "KG_SoundMgr.h"
#include "LightMgr.h"
#include "JH_MapMgr.h"
#include "CBY_GameMgr.h"

KYS::TitleScene::TitleScene()
{
	OutputDebugString(L"create TitleScene");
}

KYS::TitleScene::~TitleScene()
{
}

bool KYS::TitleScene::Init()
{
	CBY::CBY_GAMEMGR.SetDevice(_device, _deviceContext);
	CBY::CBY_GAMEMGR.CameraInit();

	I_SOUND.Init();
	SCENE_MGR.SetSound();

	std::shared_ptr<KYS::UI_panel> titlePanel = nullptr;
	std::shared_ptr<KYS::UI_button> btnStart = nullptr;

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
	titlePanel->Create(_device, _deviceContext, L"../../data/shader/DefaultShader.hlsl", L"../../data/ui/apex_ui/title.png", "VS", "PS");
	titlePanel->SetPos(D3DXVECTOR3(D3DXVECTOR3(-Winrt.right / 2.0f, Winrt.bottom /2.0f , 1005)));
	//titlePanel->SetPos(D3DXVECTOR3(1000,1000, 1005));
	//titlePanel->SetScale(D3DXVECTOR3(200,300, 0.0f));
	titlePanel->SetScale(D3DXVECTOR3(Winrt.right / 2.0f, Winrt.bottom / 2.0f, 0.0f));
	titlePanel->SetUiType(UI_TYPE::UI_TYPE_TITLE_PANEL);
	titlePanel->SetParentMatWorld(parentWorld);
	UI_MGR->add(titlePanel);

	btnStart = std::make_shared<KYS::UI_button>();
	btnStart->Create(_device, _deviceContext, L"../../data/shader/DefaultShader.hlsl", L"../../data/ui/apex_ui/Btn_Start.png", "VS", "PS");
	btnStart->SetScale(D3DXVECTOR3(100, 40, 0.0f));
	btnStart->SetPos(D3DXVECTOR3(D3DXVECTOR3(-btnStart->GetScale().x / 2.0f - 10  , btnStart->GetScale().y / 2.0f, 1005)));
	btnStart->SetUiType(UI_TYPE::UI_TYPE_BTN_START);
	btnStart->SetParentMatWorld(parentWorld);
	UI_MGR->add(btnStart);

	
	UI_MGR->setDevice(_device);
	UI_MGR->setContext(_deviceContext);
	UI_MGR->SetCamera(m_pMainCamera);

	/////////////Map//////////////////
	JH::I_LIGHT_MGR.GetDevice(_device);
	JH::I_LIGHT_MGR.GetContext(_deviceContext);
	JH::I_LIGHT_MGR.SetCamera(CBY::CBY_GAMEMGR.GetCamera());
	//JH::I_LIGHT_MGR.Create(L"../../data/Shader/JHMapShader.txt", L"../../data/LightSrc/LightInfo.txt");

	JH::I_MapMgr.Set(_device, _deviceContext, CBY::CBY_GAMEMGR.GetCamera(), &SCENE_MGR.GetSelect());
	JH::I_MapMgr.AddMap(L"../../data/MapSave/Stage1_BN.Map");
	return false;
}

bool KYS::TitleScene::Frame()
{
	SHORT key = I_Input.KeyCheck('1');
	if (key == KEY_PUSH)					//카메라의 이동
	{
		SCENE_MGR.SetNextSceneType(SCENE_TYPE::SCENE_BATTLE);
	}
	UI_MGR->Frame();
	return false;
}

bool KYS::TitleScene::Render()
{
	UI_MGR->Render(m_pMainCamera);
	return false;
}

bool KYS::TitleScene::Release()
{
	UI_MGR->ClearUiList();
	return false;
}
