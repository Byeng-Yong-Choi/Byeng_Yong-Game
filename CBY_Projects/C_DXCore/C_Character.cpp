#include "C_Character.h"
#include "C_CharMgr.h"

bool C_Character::ObjLoad(T_STR pszSkinLoad)
{
	std::shared_ptr<CBY_Weapon> obj = std::make_shared<CBY_Weapon>();
	obj->Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/SkinShader.txt", nullptr, "VSSKIN", "PS");
	obj->SkinLoad(pszSkinLoad);
	//obj->BoneLoad(pszMtrLoad);
	m_ObjectList.push_back(obj);
	return true;
}

bool C_Character::Init()
{
	CBY_Object::Init();
	return true;
}

bool C_Character::Frame()
{
	m_fElapseTick = 0;
	if (m_dwAniType == CHAR_FRAMETYPE)
	{
		m_Bone->Update(m_StateList[m_dwState].m_iStartFrame, m_StateList[m_dwState].m_iEndFrame,
			m_fElapseTick, m_pMatrixList);
	}

	else if (m_dwAniType == CHAR_MTRTYPE)
	{
		m_Bone->MTRUpdate(0, 0, 0, m_pMatrixList);
	}

	for (int iBone = 0; iBone < m_ObjList[0]->m_matBoneBindPoss.size(); iBone++)
	{
		D3DXMATRIX mat = m_ObjList[0]->m_matBoneBindPoss[iBone] * m_pMatrixList[iBone];		//바인드 포즈는 공통이다
		D3DXMatrixTranspose(&m_cbBoneWorld.g_matConstBoneWorld[iBone], &mat);
	}

	D3D11_MAPPED_SUBRESOURCE mapResource;
	m_obj.m_pContext->Map(m_pCBConstBoneWorld.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	memcpy(mapResource.pData, &m_cbBoneWorld, sizeof(CBConstBoneWorld));
	m_obj.m_pContext->Unmap(m_pCBConstBoneWorld.Get(), 0);


	for (int i = 0; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i]->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
		int imat = m_ObjectList[i]->GetSocket();
		m_ObjectList[i]->Update(&m_pMatrixList[imat]);
	}

	return true;
}

void C_Character::SetObjectSocket(int iSocket, int iObjSocket, int iFireSocket,int iobj)
{
	if (m_ObjectList.size() < 0)
	{
		return;
	}
	m_ObjectList[iobj]->SetSocket(iSocket);
	m_ObjectList[iobj]->SetObjSocke(iObjSocket);
	m_ObjectList[iobj]->SetFireSocket(iFireSocket);
}

bool C_Character::Render()
{
	PreRender();
	CharPostRender();
	for (int i = 0; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i]->Render();
	}
	return true;
}

bool C_Character::Release()
{
	CBY_Object::Release();

	m_ObjList.clear();
	for (int i = 0; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i]->Release();
	}
	return true;
}

C_Character::C_Character()
{
}


C_Character::~C_Character()
{
}

