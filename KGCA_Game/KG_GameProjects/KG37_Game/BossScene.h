#pragma once
#include "Scene.h"
#include "KG_GameStd.h"
//enemy///
#include "CBY_EnemySpider.h"
#include "CBY_EnemyRobot.h"

////////////

#include"QuadTree.h"
#include"KG_ShapeBox.h"
#include"KG_Minimap.h"
#include"KG_BackViewCamera.h"
#include"KG_DebugCamera.h"
//#include"MaxObj.h"
#include"KG_SkyBox.h"
#include "JH_MapMgr.h"

namespace KYS
{
	class VFX_EffectObj;
	class VFX_Effects;
}

namespace KYS
{

	class BossScene :public Scene
	{
	private:
		bool m_bClear;
		float m_fEndTime;

	public:
		BossScene();
		virtual ~BossScene();

		///////////////CBY_START/////////////////////////////
				/////char//////////
		float m_GameTime;

	public:
		void Shadow();
		void ShadowRender();
		////////////////////////////////CBY_END////////////////////////////////////////

				//effect/////
	public:
		std::shared_ptr<KYS::VFX_Effects> obj1;
		std::shared_ptr<KYS::VFX_Effects> obj2;
		//////////////
	public:


	public:
		virtual bool Init() override;
		virtual bool Frame() override;
		virtual bool Render() override;
		virtual bool Release() override;

		/////////////////


	};


}

class BossScene
{
public:
	BossScene();
	virtual ~BossScene();
};

