#pragma once
#include "Scene.h"
#include "KG_Select.h"
#ifdef _DEBUG
#pragma comment(lib, "KG_Engine_D")
#endif
enum class SCENE_TYPE
{ 
	SCENE_NONE = 0,
	SCENE_LOGIN, 
	SCENE_TITLE, 
	SCENE_LOBBY,
	SCENE_BATTLE,
	SCENE_BOSS,
	SCENE_END,
	SCENE_COUNT
};
class KG_Camera;
namespace KYS
{
	enum CJH_SCENE_SOUNDTRACK
	{
		SOUND_TITLE = 0,
		SOUND_LOBY,
		SOUND_BATTLE,
		SOUND_BOSS,
		SOUND_END,
		SENE_SOUND_COUNT,

	};
	
	class SceneMgr
	{


	private:
		SceneMgr();
	public:
		virtual ~SceneMgr();
	public:
		static SceneMgr& getSceneMgr()
		{
			static SceneMgr manager;
			return manager;
		}

	public:
		// 다음 씬, 없을경우 생성여부
		bool nextScene(SCENE_TYPE nextScene, bool create = false);
		void AddScene(SCENE_TYPE type, std::shared_ptr<Scene> scene);
		Scene* FindScene(SCENE_TYPE type_);

	public:
		std::vector<DWORD> m_dwSoundList;

	public:
		void SetSound();

	public:
		bool init();
		bool frame();
		bool render();
		bool release();
		
	public:
		inline Scene* getScene(SCENE_TYPE type) { return _sceneList[type].get(); }
		inline KG_Select GetSelect() {return m_Select;}

		void SetCurrentScene(Scene* scene) { _currentScene = scene; }
		Scene* GetCurrentScene() { return _currentScene; };

		void SetNextSceneType(SCENE_TYPE sceneType) { _nextSceneType = sceneType; }
		SCENE_TYPE GetNextSceneType() { return _nextSceneType; }
		void SetCameara(KG_Camera* camera) { _mainCamera = camera; }

	private: //scene에서 받아서 활용할 공통사항.
		ID3D11Device*			_device;
		ID3D11DeviceContext*	_deviceContext;
		IDXGISwapChain*			_swapChain;
		KG_Camera*				_mainCamera;
		KG_Select				m_Select;

	private:
		Scene* _currentScene;
		SCENE_TYPE _sceneType;
		SCENE_TYPE _nextSceneType;
		std::unordered_map<SCENE_TYPE, std::shared_ptr<Scene>> _sceneList;
		std::unordered_map<SCENE_TYPE, std::shared_ptr<Scene>>::iterator _iter;

	};

#define SCENE_MGR KYS::SceneMgr::getSceneMgr()

}

