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

#define CTL_CHARS		31
#define SINGLE_QUOTE	39 // ( ' )
#define ALMOST_ZERO 1.0e-4f

namespace KYS
{
	class VFX_EffectObj;
	class VFX_Effects;
}

namespace KYS
{

	class BattleScene :public Scene
	{
	public:
		BattleScene();
		virtual ~BattleScene();

///////////////CBY_START/////////////////////////////
		/////char//////////
	public:
		std::vector<std::shared_ptr<CBY::CBY_EnemyRobot>> m_EnemyList;
		std::vector<std::shared_ptr<CBY::CBY_EnemySpider>> m_EnemySpiderList;
		float m_GameTime;

	public:
		void Shadow();
		void ShadowRender();
		D3DXVECTOR3 SetMonsterPosinfo(int i);

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

		/////////Map////////
	public:
		std::shared_ptr<JH::KG_SkyBox>			m_SkyBox;
		//ComPtr<ID3D11ComputeShader>			m_pCS;

	public:
		JH::MAPDATA m_sMapData;
	public:
		//std::shared_ptr<JH::JH_Map>		m_Map;
		std::shared_ptr<JH::HQuadTree>	m_QuadTree;
		TCHAR		m_tmpBuffer[MAX_PATH];
	public:
		//Load
		int					m_iTemp;
		TCHAR				m_pBuffer[256];
		TCHAR				m_pString[256];
		//Save
		const TCHAR*		m_pSPTAFile;
	public:
		
		TCHAR* FixupName(T_STR name);
		//Load
	
	};


}

