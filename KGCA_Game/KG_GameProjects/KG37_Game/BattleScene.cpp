#include "CBY_GameMgr.h"
#include "BattleScene.h"
#include "LightMgr.h"
#include "KG_DxState.h"
#include "KG_Input.h"
#include "KG_ShapeMap.h"
#include "KG_Input.h"
#include "KG_Collision.h"
#include "MyEffectParser.h"
#include "VFX_ObjMgr.h"
#include "UI_objMgr.h"
#include <stdlib.h>       //srand
#include <time.h>     //time
#include "UI_minimap.h"
#include "UI_panel.h"
#include "SceneMgr.h"

//_device;
//_deviceContext;
//_swapChain;
//*m_pMainCamera;

#define CAMERA (*m_pMainCamera)
bool KYS::BattleScene::Init()
{
	CBY::CBY_GAMEMGR.Init();
	CBY::CBY_GAMEMGR.SetBossScene(false);

	srand((unsigned)time(NULL));

	CDX::ApplyBS(_deviceContext, CDX::KG_DxState::g_pAlpahBlend);
	CDX::ApplySS(_deviceContext, CDX::KG_DxState::g_pSampler);
	CDX::ApplyRS(_deviceContext, CDX::KG_DxState::g_pRSSold);

	///////////////Camera//////////////////////
	ShowCursor(false);
	
	KG_Camera* camera = CBY::CBY_GAMEMGR.GetCamera();
	*_camera = camera;
	m_pMainCamera = camera;
	SCENE_MGR.SetCameara(m_pMainCamera);
	
	//////////////////////////////////////////

	////////////////Char////////////////////	

	m_EnemyList.resize(14);
	m_EnemySpiderList.resize(13);
	for (int i = 0; i < m_EnemyList.size(); i++)
	{
		m_EnemyList[i] = std::make_shared<CBY::CBY_EnemyRobot>();
		m_EnemyList[i]->Create(_device, _deviceContext);
		m_EnemyList[i]->SetCamera(m_pMainCamera);
		//m_EnemyList[i]->SetInitPos(D3DXVECTOR3((10 + (i * 4)), 0, (20 + (i * 3))));
		m_EnemyList[i]->SetInitPos(SetMonsterPosinfo(i*2));
		m_EnemyList[i]->SetIndex(i);
		m_EnemyList[i]->SetHero(CBY::CBY_GAMEMGR.GetCharacter());
	}
	for (int i = 0; i < m_EnemySpiderList.size(); i++)	
	{
		m_EnemySpiderList[i] = std::make_shared<CBY::CBY_EnemySpider>();
		m_EnemySpiderList[i]->Create(_device, _deviceContext);
		m_EnemySpiderList[i]->SetCamera(m_pMainCamera);
		//m_EnemySpiderList[i]->SetInitPos(D3DXVECTOR3((-10 + (i * 4)), 0, (-20 + (i * 3))));
		m_EnemySpiderList[i]->SetInitPos(SetMonsterPosinfo(i*2+1));
		m_EnemySpiderList[i]->SetIndex(i);
		m_EnemySpiderList[i]->SetHero(CBY::CBY_GAMEMGR.GetCharacter());
	}

	//////////////////////////////////////////
	D3DXMATRIX parentWorld;
	D3DXMatrixIdentity(&parentWorld);

	parentWorld._41 = m_pMainCamera->m_orthoPos.x;
	parentWorld._42 = m_pMainCamera->m_orthoPos.y;
	parentWorld._43 = m_pMainCamera->m_orthoPos.z;

	std::shared_ptr<KYS::UI_panel> minimapFrame = nullptr;

	minimapFrame = std::make_shared < KYS::UI_panel >();
	minimapFrame->Create(_device, _deviceContext, L"../../data/shader/DefaultShader.hlsl",
		L"../../data/ui/apex_ui/minimap_frame2.png", "VS", "PS");
	minimapFrame->SetScale(D3DXVECTOR3(150.0f, 125.0f, 0.0f));
	minimapFrame->SetPos(D3DXVECTOR3(-Winrt.right / 2.0f + 60, Winrt.bottom / 2.0f - 60, 1003));
	minimapFrame->SetUiType(UI_TYPE::UI_TYPE_MINIMAP_FRAME);
	minimapFrame->SetParentMatWorld(parentWorld);
	minimapFrame->Init();

	std::shared_ptr<KYS::UI_minimap> minimap = nullptr;
	minimap = std::make_shared<KYS::UI_minimap>();
	minimap->Create(_device, _deviceContext, L"../../data/shader/minimapShader.hlsl",
		L"../../data/ui/apex_ui/weapon_portrait.png", "VS", "PS");
	minimap->SetScale(D3DXVECTOR3(150.0f, 125.0f, 0.0f));
	minimap->SetPos(D3DXVECTOR3(0, 0, 1005));
	minimap->SetUiType(UI_TYPE::UI_TYPE_MINIMAP);
	minimap->SetDeviceAndConntext(_device, _deviceContext);
	minimap->Init();

	minimapFrame->AddUI(minimap.get());



	UI_MGR->add(minimap);
	UI_MGR->add(minimapFrame);
	//minimap 

	UI_MGR->find(UI_TYPE::UI_TYPE_MINIMAP)->SetValue(UI_VALUE::UI_VALUE_SET_CAMERA, 1, m_pMainCamera);
	//UI_MGR->find(UI_TYPE::UI_TYPE_MINIMAP)->SetValue(UI_VALUE::UI_VALUE_SET_MINIMAP_DRAW_DEST_CHARACTER, 1, m_Character);
	UI_MGR->find(UI_TYPE::UI_TYPE_MINIMAP)->SetValue(UI_VALUE::UI_VALUE_SET_MINIMAP_DRAW_DEST_ENEMY, 2, m_EnemyList, m_EnemySpiderList);
	UI_MGR->find(UI_TYPE::UI_TYPE_MINIMAP)->SetValue(UI_VALUE::UI_VALUE_SET_RECOVER_VIEWPORT, 1, &m_ViewPort);
	UI_MGR->find(UI_TYPE::UI_TYPE_MINIMAP)->SetValue(UI_VALUE::UI_VALUE_SET_MINIMAP_ALPHA, 0, m_ViewPort);
	UI_MGR->find(UI_TYPE::UI_TYPE_MINIMAP)->SetValue(UI_VALUE::UI_VALUE_SET_MINIMAP_VIEW_OFFSET, 1, 300.0f);

	CBY::CBY_GAMEMGR.GetCharacter()->SetInitPos(JH::I_MapMgr.GetCurrentMap()->m_pMap->GetCharPos());
	CBY::CBY_GAMEMGR.LightInit();
	return true;
}

bool  KYS::BattleScene::Frame()
{


	//////////////////Map/////////////////
	if (JH::I_MapMgr.GetCurrentMap()->m_pMap == nullptr)return true;

	JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->Frame();
	JH::I_LIGHT_MGR.Frame();


	//////////////char//////////////////////
	if (CBY::CBY_GAMEMGR.GetCharacter() == nullptr)return true;
	D3DXVECTOR3 pos;
	for (int i = 0; i < m_EnemyList.size(); i++)
	{
		pos = m_EnemyList[i]->GetHeroPos();
		//pos.y = JH::I_MapMgr.GetCurrentMap()->).m_pMap->GetHeight(pos.x, pos.z);
		pos.y = 0;
		m_EnemyList[i]->SetHeroPos(pos);
		m_EnemyList[i]->Process();
		if (m_EnemyList[i]->GetDie())
		{
			std::vector<std::shared_ptr<CBY::CBY_EnemyRobot>>::iterator iter;
			iter = m_EnemyList.begin() + i;
			m_EnemyList.erase(iter);
			i--;
			UI_MGR->find(UI_TYPE::UI_TYPE_MINIMAP)->SetValue(UI_VALUE::UI_VALUE_SET_MINIMAP_DRAW_DEST_ENEMY, 2, m_EnemyList, m_EnemySpiderList);
		}
	}
	for (int i = 0; i < m_EnemySpiderList.size(); i++)
	{
		pos = m_EnemySpiderList[i]->GetHeroPos();
		//pos.y = JH::I_MapMgr.GetCurrentMap()->).m_pMap->GetHeight(pos.x, pos.z);
		pos.y = 0;
		m_EnemySpiderList[i]->SetHeroPos(pos);
		m_EnemySpiderList[i]->Process();
		if (m_EnemySpiderList[i]->GetDie())
		{
			std::vector<std::shared_ptr<CBY::CBY_EnemySpider>>::iterator iter;
			iter = m_EnemySpiderList.begin() + i;
			m_EnemySpiderList.erase(iter);
			i--;
			UI_MGR->find(UI_TYPE::UI_TYPE_MINIMAP)->SetValue(UI_VALUE::UI_VALUE_SET_MINIMAP_DRAW_DEST_ENEMY, 2, m_EnemyList, m_EnemySpiderList);
		}
	}
		
	CBY::CBY_GAMEMGR.Frame();
	/////////////////////////////////////////////

	
	SHORT key = I_Input.KeyCheck(VK_OEM_PLUS);
	if (key == KEY_PUSH)					//카메라의 이동
	{
		UI_MGR->find(UI_TYPE::UI_TYPE_MINIMAP)->SetValue(UI_VALUE::UI_VALUE_ADD_MINIMAP_VIEW_OFFSET, 1, 10.0f);
	}

	key = I_Input.KeyCheck(VK_OEM_MINUS);
	if (key == KEY_PUSH)					//카메라의 이동
	{
		UI_MGR->find(UI_TYPE::UI_TYPE_MINIMAP)->SetValue(UI_VALUE::UI_VALUE_ADD_MINIMAP_VIEW_OFFSET, 1, -10.0f);
	}

	key = I_Input.KeyCheck('7');
	if (key == KEY_PUSH)					//카메라의 이동
	{
		SCENE_MGR.SetNextSceneType(SCENE_TYPE::SCENE_BOSS);
	}

	pos = CBY::CBY_GAMEMGR.GetCharacter()->GetHeroPos();
	if(pos.x>=211 && pos.z <=55 && m_EnemySpiderList.size()==0 && m_EnemyList.size() == 0)
	{
		SCENE_MGR.SetNextSceneType(SCENE_TYPE::SCENE_BOSS);
	}
	return true;
}

bool  KYS::BattleScene::Render()
{
	//if(g_bDebug)
	//Shadow();

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
	//_deviceContext->PSSetShaderResources(11, 1, CBY::CBY_GAMEMGR.GetShadow()->m_Rt->m_pDsvSRV.GetAddressOf());
	JH::I_MapMgr.GetCurrentMap()->m_pQuadTree->Render();
	
	//_deviceContext->VSSetConstantBuffers(2, 1,  CBY::CBY_GAMEMGR.GetLightBuffer());
	//_deviceContext->PSSetConstantBuffers(2, 1,  CBY::CBY_GAMEMGR.GetLightBuffer());
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
	
	_deviceContext->PSSetShaderResources(2, 1, ppSRVNULL);
	_deviceContext->PSSetShaderResources(3, 1, ppSRVNULL);
	_deviceContext->PSSetShaderResources(4, 1, ppSRVNULL);
	_deviceContext->PSSetShaderResources(11, 1, ppSRVNULL);
	////////////////////////////////////////////

	///////////////////Char//////////////////////
	//_deviceContext->PSSetShaderResources(6, 1, CBY::CBY_GAMEMGR.GetShadow()->m_Rt->m_pDsvSRV.GetAddressOf());
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, 0.15, 0.15, 0.15);
	//_deviceContext->VSSetConstantBuffers(2, 1, CBY::CBY_GAMEMGR.GetLightBuffer());
	//_deviceContext->PSSetConstantBuffers(2, 1, CBY::CBY_GAMEMGR.GetLightBuffer());
	_deviceContext->VSSetConstantBuffers(2, 1, JH::I_LIGHT_MGR.m_pLightConstantBuffer[0].GetAddressOf());
	_deviceContext->PSSetConstantBuffers(2, 1, JH::I_LIGHT_MGR.m_pLightConstantBuffer[0].GetAddressOf());


	for (int i = 0; i < m_EnemyList.size(); i++)
	{
		m_EnemyList[i]->SetMatrix(&scale, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
		m_EnemyList[i]->Render();
	}

	D3DXMatrixScaling(&scale, 0.05, 0.05, 0.05);
	for (int i = 0; i < m_EnemySpiderList.size(); i++)
	{
		m_EnemySpiderList[i]->SetMatrix(&scale, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
		m_EnemySpiderList[i]->Render();
	}

	CBY::CBY_GAMEMGR.Render();

	return true;

}

bool KYS::BattleScene::Release()
{
	for (int i = 0; i < m_EnemyList.size(); i++)
	{
		m_EnemyList[i]->Release();
	}
	for (int i = 0; i < m_EnemySpiderList.size(); i++)
	{
		m_EnemySpiderList[i]->Release();
	}
	JH::I_MapMgr.Release();

	UI_MGR->find(UI_TYPE::UI_TYPE_MINIMAP)->SetValue(UI_VALUE::UI_VALUE_SET_MINIMAP_DRAW_DEST_ENEMY, 2, m_EnemyList, m_EnemySpiderList);
	return true;
}


KYS::BattleScene::BattleScene()
{
	OutputDebugString(L"create BattleScene");
}


KYS::BattleScene::~BattleScene()
{
}

void KYS::BattleScene::Shadow()
{
	CBY::CBY_GAMEMGR.LightFrame();
	//CBY::CBY_GAMEMGR.ShadowBegin();
	//CBY::CBY_GAMEMGR.ShadowRender();
	//ShadowRender();
	//CBY::CBY_GAMEMGR.ShadowEnd();
}

void KYS::BattleScene::ShadowRender()
{
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, 0.15, 0.15, 0.15);

	D3DXMATRIX view, proj;
	view = CBY::CBY_GAMEMGR.GetShadow()->GetViewMatrix();
	proj = CBY::CBY_GAMEMGR.GetShadow()->GetProjMatrix();

	for (int i = 0; i < m_EnemyList.size(); i++)
	{
		m_EnemyList[i]->SetMatrix(&scale, &view,
			&proj);
		m_EnemyList[i]->Render();
	}

	D3DXMatrixScaling(&scale, 0.05, 0.05, 0.05);
	for (int i = 0; i < m_EnemySpiderList.size(); i++)
	{
		m_EnemySpiderList[i]->SetMatrix(&scale, &view,
			&proj);
		m_EnemySpiderList[i]->Render();
	}
}


D3DXVECTOR3 KYS::BattleScene::SetMonsterPosinfo(int i)
{
	switch (i)
	{
	case 0:
	{
		return D3DXVECTOR3(-131, 0,-95);
	}
	break;

	case 1:
	{
		return D3DXVECTOR3(-181, 0, -41);
	}
	break;

	case 2:
	{
		return D3DXVECTOR3(-240, 0, 19);
	}
	break;

	case 3:
	{
		return D3DXVECTOR3(-240, 0, 101);
	}
	break;

	case 4:
	{
		return D3DXVECTOR3(-242, 0, 193);
	}
	break;

	case 5:
	{
		return D3DXVECTOR3(-173, 0, 219);
	}
	break;

	case 6:
	{
		return D3DXVECTOR3(-148, 0, 143);
	}
	break;

	case 7:
	{
		return D3DXVECTOR3(-194, 0, 142);
	}
	break;

	case 8:
	{
		return D3DXVECTOR3(-171, 0, -95);
	}
	break;

	case 9:
	{
		return D3DXVECTOR3(-105, 0, -85);
	}
	break;

	case 10:
	{
		return D3DXVECTOR3(-49, 0, 81);
	}
	break;

	case 11:
	{
		return D3DXVECTOR3(-81, 0, 26);
	}
	break;

	case 12:
	{
		return D3DXVECTOR3(-140, 0, 24);
	}
	break;

	case 13:
	{
		return D3DXVECTOR3(-202, 0, 24);
	}
	break;

	case 14:
	{
		return D3DXVECTOR3(-43, 0, -63);
	}
	break;
	case 15:
	{
		return D3DXVECTOR3(-74, 0, -41);
	}
	break;
	case 16:
	{
		return D3DXVECTOR3(-23, 0, -106);
	}
	break;
	case 17:
	{
		return D3DXVECTOR3(-111, 0, -176);
	}
	break;
	case 18:
	{
		return D3DXVECTOR3(-44, 0, -200);
	}
	break;
	case 19:
	{
		return D3DXVECTOR3(12, 0, -166);
	}
	break;
	case 20:
	{
		return D3DXVECTOR3(12, 0, -135);
	}
	break;
	case 21:
	{
		return D3DXVECTOR3(12, 0, -117);
	}
	break;
	case 22:
	{
		return D3DXVECTOR3(12, 0, -83);
	}
	break;
	case 23:
	{
		return D3DXVECTOR3(44, 0, -165);
	}
	break;
	case 24:
	{
		return D3DXVECTOR3(44, 0, -137);
	}
	break;
	case 25:
	{
		return D3DXVECTOR3(44, 0, -107);
	}
	break;
	case 26:
	{
		return D3DXVECTOR3(44, 0, -77);
	}
	break;
	case 27:
	{
		return D3DXVECTOR3(151, 0, 120);
	}
	break;
	}
}