#include "CBY_GameMgr.h"
#include "CBY_BulletMgr.h"
#include "KG_SoundMgr.h"
#include "UI_objMgr.h"
#include "VFX_ObjMgr.h"
#include "MyEffectParser.h"
#include "SceneMgr.h"

namespace CBY
{
	bool CBY_GameMgr::CameraInit()
	{
		m_CharCamera = std::make_shared<CBY::CBY_CharacterCamera>();
		m_CharCamera->SetViewProj(200.0f);

		m_pMainCamera = m_CharCamera.get();
		m_pMainCamera->m_orthoPos = D3DXVECTOR3(1000, 1000, 1000);
		m_pMainCamera->m_ortholook = D3DXVECTOR3(0, 0, 10);

		//shadow///
		m_Shadow = std::make_shared<KG_Shadow>();
		m_Shadow->m_DSFormat = DXGI_FORMAT_R32_TYPELESS;
		m_Shadow->Create(m_pDevice, 4096, 4096);
		/////

		return true;
	}

	bool CBY_GameMgr::Init()
	{
		SCENE_MGR.SetCameara(m_pMainCamera);

		m_Crosshair = std::make_shared<KG_Point>();
		m_Crosshair->Create(m_pDevice, m_pContext, L"../../data/shader/DefaultShader.txt",
			L"../../data/Char/crosshair.dds", "VSmat", "PS");

		//ui
		UI_MGR->setDevice(m_pDevice);
		UI_MGR->setContext(m_pContext);
		UI_MGR->SetCamera(m_pMainCamera);
		UI_MGR->Init();

		//effect
		VFX_MGR->setDevice(m_pDevice);
		VFX_MGR->setContext(m_pContext);
		VFX_MGR->setCamera(m_pMainCamera);
		EFFECT_PARSER->LoadEffectDataFromFile(L"../../data/effectData/LoadEffectDataList.txt", m_pDevice, m_pContext);

		m_Character = std::make_shared<CBY::CBY_FPSCharacter>();
		m_Character->Create(m_pDevice, m_pContext);
		m_Character->SetFireTime(0.5f);

		CBY::CBY_CHAR_BULLET.Init();
		
		m_Boss = std::make_shared<CBY::CBY_BossDragon>();
		m_Boss->Create(m_pDevice, m_pContext);
		m_Boss->SetCamera(m_pMainCamera);
		m_Boss->SetInitPos(D3DXVECTOR3(155, 0, -7));
		m_Boss->SetHero(m_Character.get());

		m_Character->SetCamera(m_pMainCamera);
		CBY::CBY_CHAR_BULLET.SetHero(m_Character.get());
		m_CharCamera->SetCameraSpeed(10);

		return true;
	}

	bool CBY_GameMgr::Frame()
	{
		m_CharCamera->CameraMove();

		D3DXVECTOR3 pos = m_Character->GetHeroPos();
		//pos.y = JH::I_MapMgr.GetCurrentMap()->).m_pMap->GetHeight(pos.x, pos.z);
		pos.y = 0;
		m_Character->SetHeroPos(pos);

		if (m_bBossScene)
		{
			pos = m_Boss->GetHeroPos();
			//pos.y = JH::I_MapMgr.GetCurrentMap()->).m_pMap->GetHeight(pos.x, pos.z);
			pos.y = 0;
			m_Boss->SetHeroPos(pos);
			m_Boss->Process();
		}

		m_Character->Process();
		D3DXMATRIX scale;
		float scl = m_Character->GetScale();
		D3DXMatrixScaling(&scale, scl, scl, scl);
		m_Character->SetMatrix(&(m_pMainCamera->m_World*scale), nullptr, nullptr);
		m_pMainCamera->m_Pos = m_Character->GetCamerPos();


		/////////////ammo////
		D3DXMatrixScaling(&scale, 0.4, 0.4, 0.4);
		CBY::CBY_CHAR_BULLET.SetMatrix(&scale, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
		CBY::CBY_CHAR_BULLET.Frame();
		I_SOUND.Frame();
		m_Crosshair->Frame();

		//effect
		VFX_MGR->Frame();

#pragma region UI
		UI_MGR->Frame();
#pragma endregion
		return true;
	}

	bool CBY_GameMgr::Render()
	{
		///////////////////Char//////////////////////
		m_pContext->PSSetShaderResources(6, 1, m_Shadow->m_Rt->m_pDsvSRV.GetAddressOf());
		//m_pContext->VSSetConstantBuffers(2, 1, m_Lightbuffer.GetAddressOf());
		//m_pContext->PSSetConstantBuffers(2, 1, m_Lightbuffer.GetAddressOf());
		m_pContext->VSSetConstantBuffers(2, 1, JH::I_LIGHT_MGR.m_pLightConstantBuffer[0].GetAddressOf());
		m_pContext->PSSetConstantBuffers(2, 1, JH::I_LIGHT_MGR.m_pLightConstantBuffer[0].GetAddressOf());

		D3DXMATRIX scale;
		float scl = m_Boss->GetScale();
		D3DXMatrixScaling(&scale, scl, scl, scl);
		m_Boss->SetMatrix(&scale, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
		m_Boss->Render();

		m_Character->SetMatrix(nullptr, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
		m_Character->Render();
		/////////////////////////////////////////////

		CBY::CBY_CHAR_BULLET.Render();
		//effect
		VFX_MGR->Render(m_pMainCamera);

#pragma region UI

		UI_MGR->Render(m_pMainCamera);

#pragma endregion
		CDX::ApplyBS(m_pContext, CDX::KG_DxState::g_pAlpahBlend);
		CDX::ApplyDSS(m_pContext, CDX::KG_DxState::g_pDSSDisable);

		m_Crosshair->SetMatrix(nullptr, nullptr, &m_pMainCamera->m_OrthoProj);
		m_Crosshair->Render();

		return true;
	}

	bool CBY_GameMgr::Release()
	{
		if(m_Character!=nullptr)
		m_Character->Release();
		if (m_CharCamera != nullptr)
		m_CharCamera->Release();
		CBY::CBY_CHAR_BULLET.Release();
		if (m_Shadow != nullptr)
		m_Shadow->Release();
		if (m_Crosshair != nullptr)
		m_Crosshair->Release();

		UI_MGR->ClearUiList();
		return true;
	}

	void CBY_GameMgr::LightInit()
	{
		for (int i = 0; i < JH::g_iLightNum; i++)
		{
			m_Light.vAmbinentLightColor[i] = JH::I_LIGHT_MGR.m_cbLight.vAmbinentLightColor[i];
			m_Light.vDiffuseLightColor[i] = JH::I_LIGHT_MGR.m_cbLight.vDiffuseLightColor[i];
			m_Light.vSpecularLightColor[i] = JH::I_LIGHT_MGR.m_cbLight.vSpecularLightColor[i];
			m_Light.vSpotLight[i] = JH::I_LIGHT_MGR.m_cbLight.vSpotLight[i];
			m_Light.vLightPos[i] = JH::I_LIGHT_MGR.m_cbLight.vLightPos[i];
			m_Light.vLightDirection[i] = JH::I_LIGHT_MGR.m_cbLight.vLightDirection[i];
		}

		m_Light.vAmbinentLightColor[1] = D3DXVECTOR4(0.1, 0.1, 0.1, 1);
		m_Light.vDiffuseLightColor[1] = D3DXVECTOR4(1, 1, 0, 1);
		m_Light.vSpecularLightColor[1] = D3DXVECTOR4(1, 1, 1, 1);
		m_Light.vSpotLight[1] = D3DXVECTOR4(cosf(D3DXToRadian(90.0f / 2.0f)), cosf(D3DXToRadian(120.0f / 2.0f)), 5, 20);
		m_Lightbuffer.Attach(CDXH::CreateConstantBuffer(m_pDevice, &m_Light, 1, sizeof(JH::LightConstantBuffer)));
	}

	void CBY_GameMgr::LightFrame()
	{
		for (int i = 0; i < JH::g_iLightNum; i++)
		{
			m_Light.vLightPos[i] = JH::I_LIGHT_MGR.m_cbLight.vLightPos[i];
			m_Light.vLightDirection[i] = JH::I_LIGHT_MGR.m_cbLight.vLightDirection[i];
			m_Light.vEyeDir[i].x = m_pMainCamera->m_LookDir.x;
			m_Light.vEyeDir[i].y = m_pMainCamera->m_LookDir.y;
			m_Light.vEyeDir[i].z = m_pMainCamera->m_LookDir.z;
			m_Light.vEyeDir[i].w = 1;

			m_Light.vEyePos[i].x = m_pMainCamera->m_Pos.x;
			m_Light.vEyePos[i].y = m_pMainCamera->m_Pos.y;
			m_Light.vEyePos[i].z = m_pMainCamera->m_Pos.z;
			m_Light.vEyePos[i].w = 1;
		}
	}

	void CBY_GameMgr::ShadowBegin()
	{
		//if (g_bDebug) return;
		//LightFrame();
		D3DXMATRIX matworld, matview, matproj;

		D3DXMatrixIdentity(&matworld);
		D3DXVECTOR3 vEye;
		//vEye.x = m_Light.vLightPos[0].x;
		//vEye.y = m_Light.vLightPos[0].y;
		//vEye.z = m_Light.vLightPos[0].z;
		vEye.x = JH::I_LIGHT_MGR.m_cbLight.vLightPos[0].x;
		vEye.y = JH::I_LIGHT_MGR.m_cbLight.vLightPos[0].y;
		vEye.z = JH::I_LIGHT_MGR.m_cbLight.vLightPos[0].z;
		D3DXVECTOR3 vLookat = D3DXVECTOR3(0, 0, 0);
		D3DXVECTOR3 vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		float fmapRow= JH::I_MapMgr.GetCurrentMap()->m_pMap->m_iRowNum;
		float fmapCol= JH::I_MapMgr.GetCurrentMap()->m_pMap->m_iColumNum;
		float fmapCellSize= JH::I_MapMgr.GetCurrentMap()->m_pMap->m_iCellCount;

		float MaxDistance = sqrt((fmapRow * fmapCellSize * fmapRow * fmapCellSize)
			+ (fmapCol * fmapCellSize * fmapCol * fmapCellSize));
		//MaxDistance *= 2;
		m_Shadow->SetLTM(&matworld, vEye, vLookat, MaxDistance, 1000);
		m_Shadow->GetLTM(matworld, matview, matproj, m_LTM);

		JH::I_LIGHT_MGR.m_cbLight.matLTM[0] = m_LTM;

		ID3D11ShaderResourceView* ppSRVNULL[1] = { NULL };
		ID3D11UnorderedAccessView* ppUAViewNULL[1] = { NULL };
		ID3D11Buffer* ppCBNULL[1] = { NULL };
		m_Shadow->Begin(m_pContext);
	}

	void CBY_GameMgr::ShadowRender()
	{
		D3DXMATRIX view, proj;
		view = m_Shadow->GetViewMatrix();
		proj = m_Shadow->GetProjMatrix();

		CDX::ApplyDSS(m_pContext, CDX::KG_DxState::g_pDSS);
		CDX::ApplyBS(m_pContext, CDX::KG_DxState::g_pAlpahBlend);
		CDX::ApplyRS(m_pContext, CDX::KG_DxState::g_pRSDepth);

		JH::I_MapMgr.GetCurrentMap()->m_pMap->SetMatrix(nullptr, &view,
			&proj);
		JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->NoneSkyBoxRender();

		for (auto Obj : JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->m_DrawObjectList)
		{
			Obj->GetObj()->SetMatrix(&Obj->GetObj()->m_matWorld,
				&view,
				&proj);
			Obj->GetObj()->Render();
		}

		D3DXMATRIX scale;
		float scl = m_Boss->GetScale();
		D3DXMatrixScaling(&scale, scl, scl, scl);
		m_Boss->SetMatrix(&scale, &view,
			&proj);
		m_Boss->Render();

		m_Character->SetMatrix(nullptr, &view,
			&proj);
		m_Character->Render();
	}

	void CBY_GameMgr::ShadowEnd()
	{
		m_Shadow->End(m_pContext);
		//m_pContext->UpdateSubresource(m_Lightbuffer.Get(), 0, NULL, &m_Light, 0, 0);
	}

	CBY_GameMgr::CBY_GameMgr()
	{
		m_bBossScene = false;
	}


	CBY_GameMgr::~CBY_GameMgr()
	{
	}
}
