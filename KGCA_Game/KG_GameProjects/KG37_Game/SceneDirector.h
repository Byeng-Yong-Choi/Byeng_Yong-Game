#pragma once
#pragma warning( disable:4005 )
#include "KG_Core.h"
#pragma comment(lib, "KG_Engine_D")

namespace CBY
{
	class CBY_CharacterCamera;
}
enum class SCENE_TYPE;
namespace KYS
{
	class Scene;
}

namespace KYS
{
	class SceneDirector : public KG_Core
	{
	public:
		
	public:
		SceneDirector();
		virtual ~SceneDirector();
	public:
		void NextScene(SCENE_TYPE type_);
		std::shared_ptr<KYS::Scene> CreateScene(SCENE_TYPE type_);
	public:
		LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
		virtual bool Init() override;
		virtual bool Frame() override;
		virtual bool Render() override;
		virtual bool Release() override;
	public:

	public:

	private:
		std::shared_ptr<CBY::CBY_CharacterCamera> m_CharCamera;
	};

}

KGCA37_RUN(KYS::SceneDirector, KGCA_37_Game, KGCA_37_Game)