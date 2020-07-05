#pragma once
//////
#include "CBY_BossDragon.h"
////////////
#include "KG_Point.h"
/////char////
#include "CBY_FPSCharacter.h"
#include "CBY_CharacterCamera.h"
//Shadow
#include "KG_Shadow.h"
#include "JH_MapMgr.h"

namespace CBY
{
	class CBY_GameMgr :public Singleton<CBY_GameMgr>
	{
	private:
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pContext;
		std::shared_ptr<KG_Point> m_Crosshair;
		std::shared_ptr<CBY::CBY_FPSCharacter> m_Character;
		std::shared_ptr<CBY::CBY_BossDragon> m_Boss;
		std::shared_ptr<CBY::CBY_CharacterCamera> m_CharCamera;
		std::shared_ptr<KG_Shadow> m_Shadow;
		KG_Camera* m_pMainCamera;
		KG_Camera** m_Camera;

	private:
		bool m_bBossScene;

	public:
		bool Init();
		bool Frame();
		bool Render();
		bool Release();
		bool CameraInit();

	public:
		void LightInit();
		void LightFrame();

		//Shadow//
	public:
		void ShadowBegin();
		void ShadowRender();
		void ShadowEnd();
		D3DXMATRIX m_LTM;

		//Light
	public:
		JH::LightConstantBuffer m_Light;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Lightbuffer;
		float m_t;


	public:			//Set
		inline void SetDevice(ID3D11Device* pdevice, ID3D11DeviceContext* pcontext)
		{
			m_pDevice = pdevice; m_pContext = pcontext;
		}
		inline void SetCameraProjUpdate(float fFar, float fNear = 1.0f) { m_CharCamera->SetViewProj(fFar, fNear); }
		inline void SetBossScene(bool bScene) { m_bBossScene = bScene; }

	public:			//Get
		inline CBY_FPSCharacter* GetCharacter() { return m_Character.get(); }
		inline CBY_BossDragon* GetBoss() { return m_Boss.get(); }
		inline KG_Camera* GetCamera() { return m_pMainCamera; }
		inline ID3D11Buffer** GetLightBuffer() { return m_Lightbuffer.GetAddressOf(); }
		inline KG_Shadow* GetShadow() { return m_Shadow.get(); }

	public:
		CBY_GameMgr();
		virtual ~CBY_GameMgr();
	};

#define CBY_GAMEMGR CBY_GameMgr::GetInstance()
}

