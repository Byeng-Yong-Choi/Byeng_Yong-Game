#include "LobbyScene.h"
#include "SceneMgr.h"

KYS::LobbyScene::LobbyScene()
{
	OutputDebugString(L"create LobbyScene");
}

KYS::LobbyScene::~LobbyScene()
{
}

bool KYS::LobbyScene::Init()
{
	return true;
}

bool KYS::LobbyScene::Render()
{
	return true;
}

bool KYS::LobbyScene::Release()
{
	return true;
}

bool KYS::LobbyScene::Frame()
{
	SHORT key = I_Input.KeyCheck('1');
	if (key == KEY_PUSH)					//ī�޶��� �̵�
	{
		SCENE_MGR.SetNextSceneType(SCENE_TYPE::SCENE_BATTLE);
	}
	return true;
}
