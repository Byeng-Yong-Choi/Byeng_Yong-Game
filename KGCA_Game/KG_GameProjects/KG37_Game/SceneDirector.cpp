#include "SceneDirector.h"
#include "SceneMgr.h"
#include "BattleScene.h"
#include "BossScene.h"
#include "LobbyScene.h"
#include "TitleScene.h"
#include "GameEnd.h"


KYS::SceneDirector::SceneDirector()
{
}

KYS::SceneDirector::~SceneDirector()
{
}

void KYS::SceneDirector::NextScene(SCENE_TYPE type_)
{
	KYS::Scene* findScene = nullptr;
	std::shared_ptr<KYS::Scene> reScene = nullptr;

	findScene = SCENE_MGR.FindScene(type_);

	if (findScene)
	{
		SCENE_MGR.nextScene(type_);
	}
	else
	{
		reScene = CreateScene(type_);
		SCENE_MGR.AddScene(type_ , reScene);
		SCENE_MGR.nextScene(type_);
	}
}

std::shared_ptr<KYS::Scene> KYS::SceneDirector::CreateScene(SCENE_TYPE type_)
{
	std::shared_ptr<KYS::Scene> reScene = nullptr;

	switch (type_)
	{

	case SCENE_TYPE::SCENE_TITLE:
	{
		reScene = std::make_shared<KYS::TitleScene>();
	
	}break;

	case SCENE_TYPE::SCENE_LOBBY:
	{
		reScene = std::make_shared<KYS::LobbyScene>();
	
	}break;

	case SCENE_TYPE::SCENE_BATTLE:
	{
		reScene = std::make_shared<KYS::BattleScene>();
		reScene->setViewPort(&m_ViewPort);
	}break;

	case SCENE_TYPE::SCENE_BOSS:
	{
		reScene = std::make_shared<KYS::BossScene>();
		reScene->setViewPort(&m_ViewPort);
	}break;

	case SCENE_TYPE::SCENE_END:
	{
		reScene = std::make_shared<KYS::GameEnd>();
		reScene->setViewPort(&m_ViewPort);
	}break;
	}

	reScene->setDevice(m_pd3dDevice);
	reScene->setDeviceContext(m_pContext);
	reScene->setSwapChain(m_pSwapChain);
	reScene->setMainCamera(&m_pMainCamera);

	return reScene; 
}

bool KYS::SceneDirector::Init()
{
	NextScene(SCENE_TYPE::SCENE_TITLE);
	SCENE_MGR.init();
	return true;
}

bool KYS::SceneDirector::Frame()
{
	if (SCENE_MGR.GetNextSceneType() != SCENE_TYPE::SCENE_NONE)
	{
		NextScene(SCENE_MGR.GetNextSceneType());
	}
	SCENE_MGR.frame();
	return true;
}

bool KYS::SceneDirector::Render()
{
	SCENE_MGR.render();
	return true;
}

bool KYS::SceneDirector::Release()
{
	return true;
}

LRESULT KYS::SceneDirector::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	/*if (m_Character != nullptr)
	{
		if (message == WM_LBUTTONDOWN)
		{
			m_bFire = true;
		}

		if (message == WM_LBUTTONUP)
		{
			m_bFire = false;
		}
	}*/
	return KG_Core::WndProc(hWnd, message, wParam, lParam);
}