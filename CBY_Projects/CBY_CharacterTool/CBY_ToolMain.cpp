#include "stdafx.h"
#include "CBY_ToolMain.h"
#include "C_CharMgr.h"
#include "C_TextureMgr.h"


void CBY_ToolMain::Create(T_STR SkinLoad, T_STR MTRLoad)
{
	CDX::ApplyBS(m_pContext, CDX::CDxState::g_pAlpahBlend);
	CDX::ApplySS(m_pContext, CDX::CDxState::g_pSampler);
	CDX::ApplyRS(m_pContext, CDX::CDxState::g_pRSSold);

	if (m_Character)
	{
		m_Character->Release();
		m_Character = 0;
	}
	m_Character = std::make_shared<CBY_CharacterToolSet>();
	m_Character->Create(m_pd3dDevice, m_pContext, L"../../data/shader/SkinShader.txt", nullptr, "VSSKIN", "PS");
	m_Character->SkinLoad(SkinLoad);
	m_Character->BoneLoad(MTRLoad);
	MTRInfo info;
	info.MtrName = MTRLoad;
	info.State = -1;
	//m_Data.m_data.CharBox

	m_Data.m_data.MtrList.push_back(info);
	//m_Character->SetStateNum();

	m_Data.m_data.SknNameList.push_back(SkinLoad);
	m_Data.m_data.StateList.clear();
	//m_Data.m_data.StateList.resize(m_Character->m_StateList.size());

	m_Data.m_data.m_CharType = CHAR_FRAMETYPE;
}

void CBY_ToolMain::ObjCreate(T_STR SkinLoad, T_STR MTRLoad)
{
	m_Character->ObjLoad(SkinLoad);

	ObjInfo objinfo;
	objinfo.objdata.SknNameList.push_back(SkinLoad);

	m_Character->m_ObjectList[0]->BoneLoad(MTRLoad);
	MTRInfo info;
	info.MtrName = MTRLoad;
	info.State = -1;
	objinfo.objdata.MtrList.push_back(info);
	m_Data.m_data.ObjList.push_back(objinfo);
}

void CBY_ToolMain::ObjSocketSet(int socket, int objnum)
{
	m_Character->SetObjectSocket(socket, m_Character->m_ObjectList[objnum]->GetObjSocke(),m_Character->m_ObjectList[objnum]->GetFireSocket(), objnum);
	m_Data.m_data.ObjList[objnum].Socket = socket;
	m_Data.m_data.ObjList[objnum].ObjSocket = m_Character->m_ObjectList[objnum]->GetObjSocke();
	m_Data.m_data.ObjList[objnum].FireSocket = m_Character->m_ObjectList[objnum]->GetFireSocket();
}

void CBY_ToolMain::LoadCreate()
{
	CDX::ApplyBS(m_pContext, CDX::CDxState::g_pAlpahBlend);
	CDX::ApplySS(m_pContext, CDX::CDxState::g_pSampler);
	CDX::ApplyRS(m_pContext, CDX::CDxState::g_pRSSold);

	if (m_Character)
	{
		m_Character->Release();
		m_Character = 0;
	}
	m_Character = std::make_shared<CBY_CharacterToolSet>();
	m_Character->Create(m_pd3dDevice, m_pContext, L"../../data/shader/SkinShader.txt", nullptr, "VSSKIN", "PS");
	for (DWORD dw = 0; dw < m_Data.m_data.SknNameList.size(); dw++)
	{
		m_Character->SkinLoad(m_Data.m_data.SknNameList[dw]);
	}

	if (m_Data.m_data.m_CharType == CHAR_FRAMETYPE)
	{
		m_Character->SetStateNum(m_Data.m_data.dwStateNum);

		for (DWORD dwState = 0; dwState < m_Data.m_data.dwStateNum; dwState++)
		{
			SetCharAction(dwState, m_Data.m_data.StateList[dwState].m_iStartFrame,
				m_Data.m_data.StateList[dwState].m_iEndFrame);
		}

		for (DWORD dw = 0; dw < m_Data.m_data.MtrList.size(); dw++)
		{
			m_Character->BoneLoad(m_Data.m_data.MtrList[dw].MtrName);
		}
	}
	else
	{
		m_Character->SetStateNum(m_Data.m_data.dwStateNum);

		for (DWORD dw = 0; dw < m_Data.m_data.MtrList.size(); dw++)
		{
			if (m_Data.m_data.MtrList[dw].State == -1)
			{
				m_Character->BoneLoad(m_Data.m_data.MtrList[dw].MtrName);
			}
			else
			{
				m_Character->SetMTRApply(m_Data.m_data.MtrList[dw].MtrName, m_Data.m_data.MtrList[dw].State);
			}
		}
	}
	m_Character->SetColBoxList(m_Data.m_data.ColBoxList);
	m_Data.m_data.ColBoxList.clear();

	for (DWORD dw = 0; dw < m_Data.m_data.ObjList.size(); dw++)
	{
		for (DWORD ob = 0; ob < m_Data.m_data.SknNameList.size(); ob++)
		{
			m_Character->ObjLoad(m_Data.m_data.ObjList[dw].objdata.SknNameList[0]);
		}

		if (m_Data.m_data.ObjList[dw].objdata.m_CharType == CHAR_FRAMETYPE)
		{
			m_Character->m_ObjectList[dw]->SetStateNum(m_Data.m_data.ObjList[dw].objdata.dwStateNum);

			for (DWORD dwState = 0; dwState < m_Data.m_data.ObjList[dw].objdata.dwStateNum; dwState++)
			{
				SetCharAction(dwState, m_Data.m_data.ObjList[dw].objdata.StateList[dwState].m_iStartFrame,
					m_Data.m_data.ObjList[dw].objdata.StateList[dwState].m_iEndFrame);
			}

			for (DWORD ob = 0; ob < m_Data.m_data.MtrList.size(); ob++)
			{
				m_Character->m_ObjectList[dw]->BoneLoad(m_Data.m_data.ObjList[dw].objdata.MtrList[ob].MtrName);
			}
		}
		else
		{
			m_Character->m_ObjectList[dw]->SetStateNum(m_Data.m_data.ObjList[dw].objdata.dwStateNum);

			for (DWORD ob = 0; ob < m_Data.m_data.ObjList[dw].objdata.MtrList.size(); ob++)
			{
				if (m_Data.m_data.ObjList[dw].objdata.MtrList[ob].State == -1)
				{
					m_Character->m_ObjectList[dw]->BoneLoad(m_Data.m_data.ObjList[dw].objdata.MtrList[ob].MtrName);
				}
				else
				{
					m_Character->m_ObjectList[dw]->SetMTRApply(m_Data.m_data.ObjList[dw].objdata.MtrList[ob].MtrName,
						m_Data.m_data.ObjList[dw].objdata.MtrList[ob].State);
				}
			}
		}

		m_Character->SetObjectSocket(m_Data.m_data.ObjList[dw].Socket, m_Data.m_data.ObjList[dw].ObjSocket, m_Data.m_data.ObjList[dw].FireSocket,dw);
	}

	m_Character->SetState(0);
}

void CBY_ToolMain::SkinAdd(T_STR SkinLoad)
{
	m_Character->SkinLoad(SkinLoad);
	m_Data.m_data.SknNameList.push_back(SkinLoad);
}

void CBY_ToolMain::MTRAdd(T_STR MTRLoad)
{
	m_Character->BoneLoad(MTRLoad);
	MTRInfo info;
	info.MtrName = MTRLoad;
	info.State = -2;
	m_Data.m_data.MtrList.push_back(info);
}

void CBY_ToolMain::AniMTRAdd(T_STR MTRLoad,bool bani)
{
	m_Character->AniBoneLoad(MTRLoad, bani);
	m_Data.m_data.m_CharType = CHAR_MTRTYPE;
}

void CBY_ToolMain::ObjectAniMTRAdd(T_STR MTRLoad, bool bani)
{
	m_Character->m_ObjectList[0]->AniBoneLoad(MTRLoad, bani);
	m_Data.m_data.ObjList[0].objdata.m_CharType = CHAR_MTRTYPE;
}

void CBY_ToolMain::AniMTRApply(T_STR MTRLoad, int state)
{
	m_Character->SetMTRApply(MTRLoad, state);
	m_Data.m_data.m_CharType = CHAR_MTRTYPE;

	if (m_Data.m_data.MtrList.size() > 0)
	{
		if (m_Data.m_data.MtrList.size() > state)
		{
			for (int i = 0; i < m_Data.m_data.MtrList.size(); i++)			//기존에 있던 상태 값이면 정보를 이름만 바꾸고 추가하지않는다.
			{
				if (m_Data.m_data.MtrList[i].State == state)
				{
					m_Data.m_data.MtrList[i].MtrName = MTRLoad;
					return;
				}
			}

			MTRInfo info;
			info.MtrName = MTRLoad;
			info.State = state;
			m_Data.m_data.MtrList.push_back(info);
		}
		else
		{
			for (int i = m_Data.m_data.MtrList.size(); i <= state; i++)
			{
				MTRInfo info;
				info.MtrName = MTRLoad;
				info.State = state;
				m_Data.m_data.MtrList.push_back(info);
			}
		}
	}
	else
	{
		for (int i = m_Data.m_data.MtrList.size(); i <= state; i++)
		{
			MTRInfo info;
			info.MtrName = MTRLoad;
			info.State = state;
			m_Data.m_data.MtrList.push_back(info);
		}
	}
}

void CBY_ToolMain::ObjectAniMTRApply(T_STR MTRLoad, int state)
{
	m_Character->m_ObjectList[0]->SetMTRApply(MTRLoad, state);
	m_Data.m_data.m_CharType = CHAR_MTRTYPE;

	if (m_Data.m_data.ObjList[0].objdata.MtrList.size() > 0)
	{
		if (m_Data.m_data.ObjList[0].objdata.MtrList.size() > state)
		{
			for (int i = 0; i < m_Data.m_data.MtrList.size(); i++)			//기존에 있던 상태 값이면 정보를 이름만 바꾸고 추가하지않는다.
			{
				if (m_Data.m_data.ObjList[0].objdata.MtrList[i].State == state)
				{
					m_Data.m_data.ObjList[0].objdata.MtrList[i].MtrName = MTRLoad;
					return;
				}
			}

			MTRInfo info;
			info.MtrName = MTRLoad;
			info.State = state;
			m_Data.m_data.ObjList[0].objdata.MtrList.push_back(info);
		}
		else
		{
			for (int i = m_Data.m_data.ObjList[0].objdata.MtrList.size(); i <= state; i++)
			{
				MTRInfo info;
				info.MtrName = MTRLoad;
				info.State = state;
				m_Data.m_data.ObjList[0].objdata.MtrList.push_back(info);
			}
		}
	}
	else
	{
		for (int i = m_Data.m_data.ObjList[0].objdata.MtrList.size(); i <= state; i++)
		{
			MTRInfo info;
			info.MtrName = MTRLoad;
			info.State = state;
			m_Data.m_data.ObjList[0].objdata.MtrList.push_back(info);
		}
	}
}

bool CBY_ToolMain::Init()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	m_ModelCamera = std::make_shared<CModelViewCamera>();
	m_BackCamera = std::make_shared<CBY_BackViewCamera>();
	m_ModelCamera->SetViewProj(3000.0f);
	m_BackCamera->m_Pos.y = 300;
	m_BackCamera->m_Pos.z = 200;
	m_BackCamera->SetViewProj(3000.0f);
	m_pMainCamera = m_ModelCamera.get();

	m_CharCamera = std::make_shared<CBY_CharacterCamera>();
	m_CharCamera->m_At.z = 1;
	m_CharCamera->SetViewProj();
	m_CharCamera->SetClientCursor(m_Center);

	m_SkyBox = std::make_shared<CBY_SkyBox>();
	m_SkyBox->Create(m_pd3dDevice, m_pContext, L"../../data/shader/SkyBox.txt");

	m_Shadow = std::make_shared<CBY_Shadow>();
	m_Shadow->m_DSFormat = DXGI_FORMAT_R32_TYPELESS;
	m_Shadow->Create(m_pd3dDevice,2048,2048);

	m_Map = std::make_shared<C_Map_shape>();
	//std::wstring szHeightMap;
	//szHeightMap = L"../../data/obj/maptool/height.bmp";
	//m_Map->CBYHeightMapLoad(szHeightMap.c_str(), m_pd3dDevice, m_pContext);
	m_Map->m_iNumCols = 256;
	m_Map->m_iNumRows = 256;
	C_MapDesc mapdesc = { m_Map->m_iNumCols,m_Map->m_iNumRows, 1, 1.0f,L"../../data/shader/Textmap.txt", L"../../data/map/baseColor.jpg", "VSmat","PS" };	//깊이그림자
	if (FAILED(m_Map->Load(mapdesc, m_pd3dDevice, m_pContext)))
	{
		return false;
	}
	m_Map->m_ConData.ddata.x = m_Map->m_iNumRows;
	m_Map->m_ConData.ddata.z = m_Map->m_iNumCols;

	m_Light.g_cAmbientLightColor = D3DXVECTOR4(0.3, 0.3, 0.3, 1);
	m_Light.g_cDifuseLightColor = D3DXVECTOR4(1, 1, 1, 1);
	m_Light.g_cSpecularLightColor= D3DXVECTOR4(1, 1, 1, 1);

	m_Lightbuffer.Attach(CDXH::CreateConstantBuffer(m_pd3dDevice, &m_Light, 1, sizeof(CB_LIGHT)));

	//m_QuadTree = std::make_shared<CBY_QuadTree>();
	//m_QuadTree->Build(m_Map.get(), m_pMainCamera);
	//m_QuadTree->GetSelect(&m_Select);
	mapdesc.strShaderFile.clear();
	mapdesc.strTextureFile.clear();
	mapdesc.str_PSname.clear();
	mapdesc.str_VSname.clear();
	return true;
}
bool CBY_ToolMain::Frame()
{
	m_SkyBox->Frame();
	m_Light.g_vEyeDir.x = m_pMainCamera->m_Look.x;
	m_Light.g_vEyeDir.y = m_pMainCamera->m_Look.y;
	m_Light.g_vEyeDir.z = m_pMainCamera->m_Look.z;
	m_Light.g_vEyeDir.w = 1;

	t += D3DX_PI * g_SecondTime / 2;
	float fcos = cosf(t);
	float fsin = sinf(t);

	float a = I_Col.Lerp(-10, 10, fcos);
	float b = I_Col.Lerp(-10, 10, fsin);
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
	if (m_Character == nullptr)return true;
	if (m_bPlay)
	{
		//m_pMainCamera->m_Pos = m_Character->m_Bone->m_ObjectList[46]->m_Bone.m_vPos;
		
		bool bClick=false;
		if (I_Input.GetKeyCheck(VK_SHIFT))
		{
			m_Character->SetState(CHAR_RUN);
			bClick = true;
		}

		if (I_Input.GetKeyCheck('W'))
		{
			m_Character->SetState(CHAR_MOVE);
			fCharFront += 10 * g_SecondTime;
			bClick = true;
		}
		if (I_Input.GetKeyCheck('S'))
		{
			m_Character->SetState(CHAR_MOVE);
			fCharFront -= 10 * g_SecondTime;
			bClick = true;
		}
		if (I_Input.GetKeyCheck('A'))
		{
			m_Character->SetState(CHAR_MOVE);
			fCharSide -= 10 * g_SecondTime;
			bClick = true;
		}
		if (I_Input.GetKeyCheck('D'))
		{
			m_Character->SetState(CHAR_MOVE);
			fCharSide += 10 * g_SecondTime;
			//D3DXMatrixRotationY(&m_CharRot, 90);
			bClick = true;
		}
		
		if (I_Input.GetKeyCheck(VK_SPACE))
		{
			m_Character->SetState(CHAR_JUMP);
			bClick = true;
		}

		if (!bClick)
		{
			m_Character->SetState(CHAR_IDLE);
		}

		m_Character->Frame();
		m_SkyBox->Frame();
		//m_QuadTree->Frame();
		D3DXMATRIX zpos;
		D3DXMatrixIdentity(&zpos);
		zpos._42 = 20;
		zpos._43 = -30;
		D3DXMATRIX charmat = m_Character->m_pMatrixList[46] * zpos * m_Character->m_matWorld;
		D3DXVECTOR3 bonepos = D3DXVECTOR3(charmat._41, charmat._42, charmat._43);
		m_pMainCamera->m_Pos = bonepos;
		//m_pMainCamera->m_Pos.z -= 30;

		SetShadow();
	}
	else
	{
		m_Character->Frame();
	}
	return true;
}

bool CBY_ToolMain::Render()
{

	CDX::ApplyBS(m_pContext, CDX::CDxState::g_pAlpahBlend);
	CDX::ApplySS(m_pContext, CDX::CDxState::g_pSampler, 0);
	CDX::ApplySS(m_pContext, CDX::CDxState::g_pSSClampLinear, 1);
	//CDX::ApplyRS(m_pContext, CDX::CDxState::g_pRSSold);

	if (m_bPlay)
	{
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		m_SkyBox->SetMatrix(nullptr, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
		m_SkyBox->Render();

		m_Map->SetMatrix(nullptr, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
		m_pContext->VSSetConstantBuffers(1, 1, m_Lightbuffer.GetAddressOf());
		m_pContext->PSSetConstantBuffers(1, 1, m_Lightbuffer.GetAddressOf());
		m_pContext->PSSetShaderResources(1, 1, m_Shadow->m_Rt->m_pDsvSRV.GetAddressOf());
		m_Map->Render();
		//m_QuadTree->Render();
	}

	if (m_Character == nullptr)return true;

	D3DXMatrixIdentity(&m_matWorld);
	m_Character->SetMatrix(&(m_matWorld*m_pMainCamera->m_World), &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);

	m_pContext->VSSetConstantBuffers(2, 1, m_Lightbuffer.GetAddressOf());
	m_pContext->PSSetConstantBuffers(2, 1, m_Lightbuffer.GetAddressOf());
	m_pContext->PSSetShaderResources(1, 1, m_Shadow->m_Rt->m_pDsvSRV.GetAddressOf());
	m_Character->Render();
	
	return true;
}
bool CBY_ToolMain::Release()
{
	if (m_Character == nullptr)return true;
	m_Character->Release();
	m_SkyBox->Release();
	m_Lightbuffer->Release();
	//m_QuadTree->Release();
	m_Map->Release();
	m_ModelCamera->Release();
	m_BackCamera->Release();
	m_CharCamera->Release();
	I_CHARACTER.Release();
	I_Texture.Release();
	m_Shadow->Release();
	return true;
}

void CBY_ToolMain::SetCharAction(int state, float start, float end)
{

	if (state >= m_Data.m_data.StateList.size())
	{
		Char_State_Info info;
		info.m_iStartFrame = start;
		info.m_iEndFrame = end;
		m_Data.m_data.StateList.push_back(info);
	}
	else
	{
		m_Data.m_data.StateList[state].m_iStartFrame = start;
		m_Data.m_data.StateList[state].m_iEndFrame = end;
	}

	m_Character->SetFrameTime(state, start, end);
}

void CBY_ToolMain::GetCharAction(int state)
{
	if (state < m_Character->GetStateNum())
	{
		m_Character->SetState(state);
	}
}

void CBY_ToolMain::SetCharacterPlay()
{
	m_bPlay = true;
	if (m_Character == nullptr) return;
	GetCharAction(CHAR_IDLE);

	m_pMainCamera = m_BackCamera.get();
	//m_pMainCamera = m_CharCamera.get();
}

bool CBY_ToolMain::Save(const TCHAR* pszSave)
{
	m_Data.m_data.CharBox = m_Character->GetCharBox();

	for (int i = 0; i < m_Character->GetColBoxSize(); i++)
	{
		m_Data.m_data.ColBoxList.push_back(m_Character->SetColBoxList(i));
	}
	m_Data.Save(pszSave);
	return true;
}

bool CBY_ToolMain::Load(const TCHAR* pszName)
{
	m_Data.Load(pszName);
	LoadCreate();
	return true;
}



CBY_ToolMain::CBY_ToolMain()
{
	m_bPlay = false;
	fCharFront = 0;
	fCharSide = 0;
	D3DXMatrixIdentity(&m_CharRot);
	D3DXMatrixIdentity(&m_matWorld);
	t = 0;
	m_bObjRender = false;
	
}


CBY_ToolMain::~CBY_ToolMain()
{	
	//auto ptr = aNew();
	//ptr = bNew();
	//pDelete(ptr);
}
//캐릭터
void CBY_ToolMain::SetCharCameraSocket(DWORD dwSocket)
{
	if (m_Character)
	{
		m_Character->SetCameraSocket(dwSocket);
		m_Data.m_data.m_dwCameraSocket = dwSocket;
	}
}
void CBY_ToolMain::SetWeaponFireSocket(DWORD dwSocket)
{
	if (m_Character)
	{
		m_Character->SetFireSocket(dwSocket);
		m_Data.m_data.m_dwFireSocket = dwSocket;
		m_Data.m_data.m_dwWeapon++;
	}
}

//오브젝트
void CBY_ToolMain::SetObjectMove(float fx, float fy, float fz)
{
	if (m_Character)
	{
		D3DXVECTOR3 vMove = { fx, fy, fz };
		m_Character->m_ObjectList[0]->SetMovePos(vMove);
		m_Data.m_data.ObjList[0].vpos = vMove;
	}
}

void CBY_ToolMain::SetObjectRot(float Yaw, float Pitch, float Roll)
{
	if (m_Character)
	{
		D3DXVECTOR3 vRot = { Yaw, Pitch, Roll };
		m_Character->m_ObjectList[0]->SetRot(vRot);
		m_Data.m_data.ObjList[0].vRot = vRot;
	}
}

void CBY_ToolMain::SetObjectScale(float Scl)
{
	if (m_Character)
	{
		m_Character->m_ObjectList[0]->SetScale(Scl);
		m_Data.m_data.ObjList[0].fScale = Scl;
	}
}



void CBY_ToolMain::SetShadow()
{
	
	t += D3DX_PI * g_SecondTime / 2;
	float fcos = cosf(t);
	float fsin = sinf(t);
	float a = I_Col.Lerp(-10, 10, fcos);
	float b = I_Col.Lerp(-10, 10, fsin);
	D3DXVECTOR3 Pos;
	Pos.x = a;
	Pos.y = 20;
	Pos.z = b;

	D3DXMATRIX matworld, matview, matproj;

	D3DXMatrixIdentity(&matworld);
	D3DXVECTOR3 vEye = Pos;
	D3DXVECTOR3 vLookat = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	float MaxDistance = sqrt((m_Map->m_iNumRows*m_Map->fCellDistance*m_Map->m_iNumRows*m_Map->fCellDistance) +
		(m_Map->m_iNumCols*m_Map->fCellDistance*m_Map->m_iNumCols*m_Map->fCellDistance));
	//MaxDistance *= 2;
	m_Shadow->SetLTM(&matworld, vEye, vLookat, MaxDistance,1000);
	m_Shadow->GetLTM(matworld, matview, matproj, m_LTM);

	m_Light.g_LTM = m_LTM;

	m_Shadow->Begin(m_pContext);
	m_Map->SetMatrix(nullptr, &matview, &matproj);
	m_Map->Render();

	D3DXMatrixIdentity(&m_matWorld);
	//m_matWorld._41 = m_pMainCamera->m_At.x;
	//m_matWorld._43 = m_pMainCamera->m_At.z;
	m_Character->SetMatrix(&m_matWorld, &matview, &matproj);
	m_Character->Render();

	m_Shadow->End(m_pContext);

	m_pContext->UpdateSubresource(m_Lightbuffer.Get(), 0, NULL, &m_Light, 0, 0);
}

void CBY_ToolMain::SetClientCenter(int x, int y)
{
	m_Center.x = x / 2;
	m_Center.y = y / 2;

	if (m_CharCamera != nullptr)
	{
		m_CharCamera->SetClientCursor(m_Center);
	}
}