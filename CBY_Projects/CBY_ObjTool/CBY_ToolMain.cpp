#include "stdafx.h"
#include "CBY_ToolMain.h"

void CBY_ToolMain::ObjCreate(T_STR SkinLoad, T_STR MTRLoad)
{
	CDX::ApplyBS(m_pContext, CDX::CDxState::g_pAlpahBlend);
	CDX::ApplySS(m_pContext, CDX::CDxState::g_pSampler);
	CDX::ApplyRS(m_pContext, CDX::CDxState::g_pRSSold);

	if (m_Object)
	{
		m_Object->Release();
		m_Object = 0;
	}
	m_Object = std::make_shared<CBY_Object>();
	m_Object->Create(m_pd3dDevice, m_pContext, L"../../data/shader/SkinShader.txt", nullptr, "VSSKIN", "PS");
	m_Object->SkinLoad(SkinLoad);
	m_Object->BoneLoad(MTRLoad);
}
void CBY_ToolMain::TestCreate(T_STR SkinLoad, T_STR MTRLoad)
{
	CDX::ApplyBS(m_pContext, CDX::CDxState::g_pAlpahBlend);
	CDX::ApplySS(m_pContext, CDX::CDxState::g_pSampler);
	CDX::ApplyRS(m_pContext, CDX::CDxState::g_pRSSold);

	if (m_TestObj)
	{
		m_TestObj->Release();
		m_TestObj = 0;
	}
	m_TestObj = std::make_shared<CBY_ObjectTest>();
	m_TestObj->Create(m_pd3dDevice, m_pContext, L"../../data/shader/SkinShader.txt", nullptr, "VSSKIN", "PS");
	m_TestObj->SkinLoad(SkinLoad);
	m_TestObj->BoneLoad(MTRLoad);
}



bool CBY_ToolMain::Init()
{
	m_ModelCamera = std::make_shared<CModelViewCamera>();
	m_BackCamera = std::make_shared<CBY_BackViewCamera>();
	m_ModelCamera->SetViewProj();
	m_BackCamera->m_Pos.y = 300;
	m_BackCamera->m_Pos.z = 200;
	m_BackCamera->SetViewProj();
	m_pMainCamera = m_ModelCamera.get();

	//m_Object = std::make_shared<CBY_Object>();


	m_Light.g_cAmbientLightColor = D3DXVECTOR4(0.3, 0.3, 0.3, 1);
	m_Light.g_cDifuseLightColor = D3DXVECTOR4(1, 1, 1, 1);
	m_Light.g_cSpecularLightColor = D3DXVECTOR4(1, 1, 1, 1);

	m_Lightbuffer.Attach(CDXH::CreateConstantBuffer(m_pd3dDevice, &m_Light, 1, sizeof(CB_LIGHT)));

	return true;
}

bool CBY_ToolMain::Frame()
{
	m_Light.g_vEyeDir.x = m_pMainCamera->m_Look.x;
	m_Light.g_vEyeDir.y = m_pMainCamera->m_Look.y;
	m_Light.g_vEyeDir.z = m_pMainCamera->m_Look.z;
	m_Light.g_vEyeDir.w = 1;

	m_Light.g_vEyePos.x = m_pMainCamera->m_Pos.x;	//a
	m_Light.g_vEyePos.y = m_pMainCamera->m_Pos.y;	//20
	m_Light.g_vEyePos.z = m_pMainCamera->m_Pos.z;	//b
	m_Light.g_vEyePos.w = 1;

	m_Light.g_vLightDir.x = 0;
	m_Light.g_vLightDir.y = -1;
	m_Light.g_vLightDir.z = 0;
	m_Light.g_vLightDir.w = 1;

	m_Light.g_vLightPos.x = m_pMainCamera->m_Pos.x;
	m_Light.g_vLightPos.y = m_pMainCamera->m_Pos.y;
	m_Light.g_vLightPos.z = m_pMainCamera->m_Pos.z;
	m_Light.g_vLightPos.w = 1;

	m_pContext->UpdateSubresource(m_Lightbuffer.Get(), 0, NULL, &m_Light, 0, 0);
	if (m_Object != nullptr)
	m_Object->Frame();

	if (m_TestObj != nullptr)
	m_TestObj->Frame();

	return true;
}

bool CBY_ToolMain::Render()
{
	CDX::ApplyBS(m_pContext, CDX::CDxState::g_pAlpahBlend);
	CDX::ApplySS(m_pContext, CDX::CDxState::g_pSampler, 0);
	CDX::ApplySS(m_pContext, CDX::CDxState::g_pSSClampLinear, 1);

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	if (m_Object != nullptr)
	{
		m_pContext->VSSetConstantBuffers(2, 1, m_Lightbuffer.GetAddressOf());
		m_pContext->PSSetConstantBuffers(2, 1, m_Lightbuffer.GetAddressOf());
		m_Object->SetMatrix(&(matWorld*m_pMainCamera->m_World), &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
		m_Object->Render();
	}
	
	if (m_TestObj != nullptr)
	{
		m_pContext->VSSetConstantBuffers(2, 1, m_Lightbuffer.GetAddressOf());
		m_pContext->PSSetConstantBuffers(2, 1, m_Lightbuffer.GetAddressOf());
		m_TestObj->SetMatrix(&(matWorld*m_pMainCamera->m_World), &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
		m_TestObj->Render();
	}
	

	
	//m_pContext->PSSetShaderResources(1, 1, m_Shadow->m_Rt->m_pDsvSRV.GetAddressOf());
	
	return true;
}

bool CBY_ToolMain::Release()
{
	m_Lightbuffer->Release();
	m_ModelCamera->Release();
	m_BackCamera->Release();
	if (m_Object != nullptr)
	m_Object->Release();
	m_Parser.Release();
	if (m_TestObj != nullptr)
	m_TestObj->Release();
	return true;
}

void CBY_ToolMain::SkinParser(T_STR Load)
{
	m_Parser.Clear();
	m_Parser.SkinLoad(Load.c_str());
	m_Parser.SkinTextToBinary();
}

void CBY_ToolMain::SkinBLoad(T_STR Load)
{
	m_Parser.Clear();
	m_Parser.SkinBinaryLoad(Load.c_str());
}

void CBY_ToolMain::MtrParser(T_STR Load)
{
	m_Parser.Clear();
	m_Parser.MtrLoad(Load.c_str());
	m_Parser.MtrTextToBinary();
}
void CBY_ToolMain::MtrBLoad(T_STR Load)
{
	m_Parser.Clear();
	m_Parser.MtrBinaryLoad(Load.c_str());
}

CBY_ToolMain::CBY_ToolMain()
{
}


CBY_ToolMain::~CBY_ToolMain()
{
}
