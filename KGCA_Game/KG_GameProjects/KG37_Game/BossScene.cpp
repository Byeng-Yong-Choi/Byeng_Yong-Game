#include "BossScene.h"
#include "CBY_GameMgr.h"
#include "LightMgr.h"
#include "KG_DxState.h"
#include "KG_Collision.h"
#include "MyEffectParser.h"
#include "VFX_ObjMgr.h"
#include "UI_objMgr.h"
#include <stdlib.h>       //srand
#include <time.h>     //time
#include "UI_minimap.h"
#include "UI_panel.h"
#include "SceneMgr.h"
#include "KG_SoundMgr.h"


bool KYS::BossScene::Init()
{
	//CBY::CBY_GAMEMGR.SetDevice(_device, _deviceContext);
	//CBY::CBY_GAMEMGR.Init();
	CBY::CBY_GAMEMGR.SetBossScene(true);

	///////////////Camera//////////////////////
	CBY::CBY_GAMEMGR.SetCameraProjUpdate(1000.0f);
	ShowCursor(false);

	KG_Camera* camera = CBY::CBY_GAMEMGR.GetCamera();
	*_camera = camera;
	m_pMainCamera = camera;
	SCENE_MGR.SetCameara(m_pMainCamera);
	//////////////////////////////////////////
	JH::I_LIGHT_MGR.GetDevice(_device);
	JH::I_LIGHT_MGR.GetContext(_deviceContext);
	JH::I_LIGHT_MGR.SetCamera(m_pMainCamera);
	//JH::I_LIGHT_MGR.Create(L"../../data/Shader/JHMapShader.txt", L"../../data/LightSrc/LightInfo.txt");
	JH::I_MapMgr.Set(_device, _deviceContext, m_pMainCamera, &SCENE_MGR.GetSelect());
	JH::I_MapMgr.AddMap(L"../../data/MapSave/Boss_BN.Map", L"../../data/LightSrc/BossmapLight.txt");
	
	UI_MGR->BossUi();

	CBY::CBY_GAMEMGR.GetCharacter()->InitSet();

	//hp
	UI_MGR->find(UI_TYPE::UI_TYPE_BOSS_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_BOSS_HP_BAR, UI_VALUE::UI_VALUE_SET_HPMAX, 1, 100.0f);
	UI_MGR->find(UI_TYPE::UI_TYPE_BOSS_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_BOSS_HP_BAR, UI_VALUE::UI_VALUE_SET_HP, 1, 100.0f);
	//hp frame
	UI_MGR->find(UI_TYPE::UI_TYPE_BOSS_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_BOSS_HP_BAR_FRAME, UI_VALUE::UI_VALUE_SET_LENGTHMAX, 1, 100.0f);
	UI_MGR->find(UI_TYPE::UI_TYPE_BOSS_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_BOSS_HP_BAR_FRAME, UI_VALUE::UI_VALUE_SET_LENGTH, 1, 100.0f);

	CBY::CBY_GAMEMGR.GetCharacter()->SetInitPos(JH::I_MapMgr.GetCurrentMap()->m_pMap->GetCharPos());
	//CBY::CBY_GAMEMGR.LightInit();
	return true;
}

bool KYS::BossScene::Frame()
{
	if (!m_bClear)
	{
		//////////////////Map/////////////////
		if (JH::I_MapMgr.GetCurrentMap()->m_pMap == nullptr)return true;

		JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->Frame();


		//////////////char//////////////////////
		if (CBY::CBY_GAMEMGR.GetCharacter() == nullptr)return true;

		CBY::CBY_GAMEMGR.Frame();

		SHORT key = I_Input.KeyCheck(VK_OEM_PLUS);
		if (key == KEY_PUSH)					//카메라의 이동
		{

			//UI_MGR->find(UI_TYPE::UI_TYPE_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_HP_BAR, UI_VALUE::UI_VALUE_ADD_HP, 1, 40.0f);
			//UI_MGR->find(UI_TYPE::UI_TYPE_BOSS_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_BOSS_HP_BAR, UI_VALUE::UI_VALUE_ADD_HP, 1, 40.0f);
		}

		key = I_Input.KeyCheck('2');
		if (key == KEY_PUSH)					//카메라의 이동
		{

			//UI_MGR->find(UI_TYPE::UI_TYPE_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_HP_BAR, UI_VALUE::UI_VALUE_ADD_HP, 1, -10.0f);
			//UI_MGR->find(UI_TYPE::UI_TYPE_BOSS_HP_PANEL)->SetType(UI_TYPE::UI_TYPE_BOSS_HP_BAR, UI_VALUE::UI_VALUE_ADD_HP, 1, -10.0f);
		}

		if (CBY::CBY_GAMEMGR.GetBoss()->GetDie())
		{
			m_bClear = true;
		}
	}
	else
	{
		I_SOUND.list[SCENE_MGR.m_dwSoundList[SOUND_END]]->Play(I_SOUND.m_pSystem);
		m_fEndTime += g_SecondTime;
		CBY::CBY_GAMEMGR.GetCharacter()->SetState(CBY::CHAR_DEATH);
		CBY::CBY_GAMEMGR.GetCharacter()->Frame();
		CBY::CBY_GAMEMGR.GetBoss()->Frame();
		if (m_fEndTime > 7.0f)
		{
			SCENE_MGR.SetNextSceneType(SCENE_TYPE::SCENE_END);
		}
	}
	return true;
}

bool KYS::BossScene::Render()
{
	if (!g_bDebug)
	Shadow();

	CDX::ApplyBS(_deviceContext, CDX::KG_DxState::g_pAlpahBlend);
	CDX::ApplySS(_deviceContext, CDX::KG_DxState::g_pSampler, 0);
	CDX::ApplySS(_deviceContext, CDX::KG_DxState::g_pSSClampLinear, 1);

	/////////////////////Map////////////////////
	ID3D11ShaderResourceView* ppSRVNULL[1] = { NULL };
	ID3D11UnorderedAccessView* ppUAViewNULL[1] = { NULL };
	ID3D11Buffer* ppCBNULL[1] = { NULL };

	if (JH::I_MapMgr.GetCurrentMap()->m_pMap == nullptr)return true;

	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//_deviceContext->VSSetConstantBuffers(1, 1, CBY::CBY_GAMEMGR.GetLightBuffer());
	//_deviceContext->PSSetConstantBuffers(1, 1, CBY::CBY_GAMEMGR.GetLightBuffer());
	_deviceContext->PSSetShaderResources(11, 1, CBY::CBY_GAMEMGR.GetShadow()->m_Rt->m_pDsvSRV.GetAddressOf());
	JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->Render();

	//_deviceContext->VSSetConstantBuffers(2, 1, CBY::CBY_GAMEMGR.GetLightBuffer());
	//_deviceContext->PSSetConstantBuffers(2, 1, CBY::CBY_GAMEMGR.GetLightBuffer());
	_deviceContext->VSSetConstantBuffers(2, 1, JH::I_LIGHT_MGR.m_pLightConstantBuffer[0].GetAddressOf());
	_deviceContext->PSSetConstantBuffers(2, 1, JH::I_LIGHT_MGR.m_pLightConstantBuffer[0].GetAddressOf());
	if (g_bDebug)_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	for (auto Obj : JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->m_DrawObjectList)
	{
		_deviceContext->PSSetShaderResources(1, 1, CBY::CBY_GAMEMGR.GetShadow()->m_Rt->m_pDsvSRV.GetAddressOf());
		Obj->GetObj()->SetMatrix(&Obj->GetObj()->m_matWorld,
			&m_pMainCamera->m_View,
			&m_pMainCamera->m_Proj);

		Obj->GetObj()->Render();

	}

	CBY::CBY_GAMEMGR.Render();
	return true;
}

bool KYS::BossScene::Release()
{
	CBY::CBY_GAMEMGR.Release();
	JH::I_MapMgr.Release();
	UI_MGR->ClearUiList();
	return true;
}

void KYS::BossScene::Shadow()
{
	//CBY::CBY_GAMEMGR.LightFrame();
	CBY::CBY_GAMEMGR.ShadowBegin();
	CBY::CBY_GAMEMGR.ShadowRender();
	ShadowRender();
	CBY::CBY_GAMEMGR.ShadowEnd();
}

void KYS::BossScene::ShadowRender()
{
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, 0.15, 0.15, 0.15);

	D3DXMATRIX view, proj;
	view = CBY::CBY_GAMEMGR.GetShadow()->GetViewMatrix();
	proj = CBY::CBY_GAMEMGR.GetShadow()->GetProjMatrix();

	/*D3DXMatrixScaling(&scale, 0.05, 0.05, 0.05);
	for (int i = 0; i < m_EnemySpiderList.size(); i++)
	{
		m_EnemySpiderList[i]->SetMatrix(&scale, &view,
			&proj);
		m_EnemySpiderList[i]->Render();
	}*/
}


KYS::BossScene::BossScene()
{
	OutputDebugString(L"create BossScene");
	m_bClear = false;
	m_fEndTime = 0.0f;
}


KYS::BossScene::~BossScene()
{
}
