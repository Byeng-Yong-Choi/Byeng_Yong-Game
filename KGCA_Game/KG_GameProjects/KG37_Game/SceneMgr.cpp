#include "SceneMgr.h"
#include "Scene.h"
#include "BattleScene.h"
#include "TitleScene.h"
#include "LobbyScene.h"
#include "KG_SoundMgr.h"


void  KYS::SceneMgr::SetSound()
{
	m_dwSoundList.resize(SENE_SOUND_COUNT);
	m_dwSoundList[SOUND_TITLE] = I_SOUND.Load(L"../../data/Sound/BGM/Title.wav", false, 0.5f);
	m_dwSoundList[SOUND_BATTLE] = I_SOUND.Load(L"../../data/Sound/BGM/Battle.wav", false, 0.5f);
	m_dwSoundList[SOUND_BOSS] = I_SOUND.Load(L"../../data/Sound/BGM/Boss.wav", false, 1.0f);
	m_dwSoundList[SOUND_END] = I_SOUND.Load(L"../../data/Sound/BGM/mp3.mp3", false, 1.0f);
}

bool KYS::SceneMgr::init()
{
	return true;
}

bool KYS::SceneMgr::frame()
{
	I_SOUND.Frame();
	m_Select.SetMarix(nullptr, &_mainCamera->m_View, &_mainCamera->m_Proj);
	_currentScene->Frame();
	if (!m_dwSoundList.empty())
	{
		if (_sceneType == SCENE_TYPE::SCENE_TITLE)
		{
			I_SOUND.list[m_dwSoundList[SOUND_TITLE]]->Play(I_SOUND.m_pSystem);
		}
		if (_sceneType == SCENE_TYPE::SCENE_BATTLE)
		{
			I_SOUND.list[m_dwSoundList[SOUND_BATTLE]]->Play(I_SOUND.m_pSystem);
		}
		if (_sceneType == SCENE_TYPE::SCENE_BOSS)
		{
			I_SOUND.list[m_dwSoundList[SOUND_BOSS]]->Play(I_SOUND.m_pSystem);
		}
		if (_sceneType == SCENE_TYPE::SCENE_END)
		{
			I_SOUND.list[m_dwSoundList[SOUND_END]]->Play(I_SOUND.m_pSystem);
		}
	}
	return false;
}

bool KYS::SceneMgr::render()
{
	_currentScene->Render();
	return false;
}

bool KYS::SceneMgr::release()
{
	for (auto& scene : _sceneList)
	{
		scene.second->Release();
	}
	_sceneList.clear();
	return false;
}

bool KYS::SceneMgr::nextScene(SCENE_TYPE nextScene, bool create )
{
	if (_sceneType == nextScene || nextScene == SCENE_TYPE::SCENE_NONE) return false;
	 
	Scene* next = nullptr;
	
	//scene ±³Ã¼
	next = FindScene(nextScene);

	if ( next != nullptr )
	{
		if (_currentScene)
			_currentScene->Release();
		_currentScene = next;
		_currentScene->Init();
		_sceneType = nextScene;
		_nextSceneType = SCENE_TYPE::SCENE_NONE;
	}

	return true;
}



void KYS::SceneMgr::AddScene(SCENE_TYPE type, std::shared_ptr<Scene> scene)
{
	std::shared_ptr<KYS::Scene> reScene = nullptr;
	KYS::Scene* find = nullptr;

	if (_sceneList.size() > 0)
	{
		find = FindScene(type);
	}

	if (find == nullptr)
	{
		_sceneList.insert(std::make_pair(type, scene));
	}

	return;
}

KYS::Scene * KYS::SceneMgr::FindScene(SCENE_TYPE type_)
{
	std::shared_ptr<KYS::Scene> findScene = nullptr;
	KYS::Scene* reScene = nullptr;

	_iter = _sceneList.find(type_);
	if (_iter != _sceneList.end())
		findScene = _iter->second;

	if (findScene != nullptr)
	{
		reScene = findScene.get();
	}

	return reScene;
}

KYS::SceneMgr::SceneMgr()
{
	_sceneType = SCENE_TYPE::SCENE_NONE;
	_nextSceneType = SCENE_TYPE::SCENE_NONE;
}


KYS::SceneMgr::~SceneMgr()
{
}
