#include "stdafx.h"
#include "CBY_ObjectToolSet.h"
#include "C_CharMgr.h"

void CBY_ObjectToolSet::AniBoneLoad(T_STR pszLoad, bool bani)
{
	m_dwAniType = CHAR_MTRTYPE;
	m_MtrViewIndex = I_CHARACTER.BoneLoad(pszLoad, m_obj.m_pd3dDevice, m_obj.m_pContext);

	for (int i = 0; i < m_BoxList.size(); i++)
	{
		m_BoxList[i].SetParentMatrix(&I_CHARACTER.m_BoneLoadList[m_MtrViewIndex]->
			m_ObjectList[m_BoxList[i].GetBoneIndex()]->m_matCalculation);
	}


	m_bViewCheck = true;
}

bool CBY_ObjectToolSet::Frame()
{
	m_fElapseTick = 0;
	if (m_dwAniType == CHAR_FRAMETYPE)
	{
		if (m_bViewCheck)
		{
			m_BindBone->Update(m_iStartFrame, m_iEndFrame, m_fElapseTick, m_pMatrixList);
		}
		else
		{
			m_BindBone->Update(m_StateList[m_dwState].m_iStartFrame, m_StateList[m_dwState].m_iEndFrame,
				m_fElapseTick, m_pMatrixList);
		}
	}
	else if (m_dwAniType == CHAR_MTRTYPE)
	{
		if (m_bViewCheck)
		{
			I_CHARACTER.m_BoneLoadList[m_MtrViewIndex]->MTRUpdate(0, 0, 0, m_pMatrixList);
		}
		else
		{
			m_StateList[m_dwState].m_Bone.MTRUpdate(0, 0, 0, m_pMatrixList);
		}
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

	return true;
}

bool CBY_ObjectToolSet::Update(D3DXMATRIX* parmat)
{
	D3DXMATRIX poaworld,matrot,matscale;
	D3DXVECTOR3 vScale, vPos;
	D3DXQUATERNION qRot;
	D3DXMatrixDecompose(&vScale, &qRot, &vPos, parmat);
	D3DXMatrixRotationQuaternion(&poaworld, &qRot);
	D3DXMatrixRotationYawPitchRoll(&matrot, m_vRot.x, m_vRot.y, m_vRot.z);
	D3DXMatrixScaling(&matscale, m_fScale, m_fScale, m_fScale);
	matscale *= poaworld * matrot;
	matscale._41 = vPos.x + m_vObjectMove.x;
	matscale._42 = vPos.y + m_vObjectMove.y;
	matscale._43 = vPos.z + m_vObjectMove.z;

	m_fElapseTick = 0;
	if (m_dwAniType == CHAR_FRAMETYPE)
	{
		if (m_bViewCheck)
		{
			m_BindBone->Update(m_iStartFrame, m_iEndFrame, m_fElapseTick, m_pMatrixList);
		}
		else
		{
			m_BindBone->Update(m_StateList[m_dwState].m_iStartFrame, m_StateList[m_dwState].m_iEndFrame,
				m_fElapseTick, m_pMatrixList);
		}
	}
	else if (m_dwAniType == CHAR_MTRTYPE)
	{
		if (m_bViewCheck)
		{
			I_CHARACTER.m_BoneLoadList[m_MtrViewIndex]->ObjUpdate(0, 0, 0, m_pMatrixList, &matscale, m_iObjSocket);
		}
		else
		{
			m_StateList[m_dwState].m_Bone.ObjUpdate(0, 0, 0, m_pMatrixList, &matscale, m_iObjSocket);
		}
	}

	for (int iBone = 0; iBone < m_ObjList[0]->m_matBoneBindPoss.size(); iBone++)
	{
		D3DXMATRIX mat;
		/*if (iBone == m_iObjSocket)
		{*/
			//mat = m_ObjList[0]->m_matBoneBindPoss[iBone] * (*parmat) *m_pMatrixList[iBone];
		//}
		//else
		//{
			mat = m_ObjList[0]->m_matBoneBindPoss[iBone] * m_pMatrixList[iBone];		//바인드 포즈는 공통이다
		//}
		D3DXMatrixTranspose(&m_cbBoneWorld.g_matConstBoneWorld[iBone], &mat);
	}

	D3D11_MAPPED_SUBRESOURCE mapResource;
	m_obj.m_pContext->Map(m_pCBConstBoneWorld.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	memcpy(mapResource.pData, &m_cbBoneWorld, sizeof(CBConstBoneWorld));
	m_obj.m_pContext->Unmap(m_pCBConstBoneWorld.Get(), 0);
	return true;
}

void CBY_ObjectToolSet::SetNoStateFrameTime(float start, float end)
{
	m_iStartFrame = start;
	m_iEndFrame = end;
	m_bViewCheck = true;
}



void CBY_ObjectToolSet::SetState(int dw)
{
	if (dw == m_dwState)
	{
		m_bViewCheck = false;
		return;
	}
	if (m_dwAniType == CHAR_MTRTYPE)
	{
		m_StateList[m_dwState].m_Bone.m_fElapseTick = 0;
	}
	else
	{
		m_BindBone->m_fElapseTick = m_StateList[m_dwState].m_iStartFrame;
	}
	m_bViewCheck = false;
	m_dwState = dw;
}

bool CBY_ObjectToolSet::Render()
{
	CBY_Object::Render();

	if (m_bAllBoxRender)
	{
		//m_CharBox.SetMatrix(&m_matWorld, &m_matView, &m_matProj);
		//m_CharBox.Render();
		for (int ibox = 0; ibox < m_BoxList.size(); ibox++)
		{
			D3DXMATRIX mat = m_ObjList[0]->m_matBoneBindPoss[m_BoxList[ibox].GetBoneIndex()] * m_pMatrixList[m_BoxList[ibox].GetBoneIndex()];

			m_BoxList[ibox].SetMatrix(&(mat*m_matWorld), &m_matView, &m_matProj);
			m_BoxList[ibox].Render();
		}
	}
	else if (m_iBoneSelect > 0)
	{
		BoneBoxRender(m_BindBone->m_ObjectList[m_iBoneSelect], m_iBoneSelect);
		if (bColBoxRender)
		{
			ColBoxRender();
		}
	}
	return true;
}

void CBY_ObjectToolSet::ColBoxRender()
{
	for (int ibox = 0; ibox < m_BoxList.size(); ibox++)
	{
		if (m_BoxList[ibox].GetBoneIndex() == m_iBoneSelect)
		{
			D3DXMATRIX mat = m_ObjList[0]->m_matBoneBindPoss[m_iBoneSelect] * m_pMatrixList[m_iBoneSelect];

			m_BoxList[ibox].SetMatrix(&(mat*m_matWorld), &m_matView, &m_matProj);
			m_BoxList[ibox].Render();
			return;
		}
	}
}


void CBY_ObjectToolSet::CreateCharBox(float fX, float fY, float fZ)
{
	for (int ibox = 0; ibox < m_BoxList.size(); ibox++)
	{
		if (m_BoxList[ibox].GetBoneIndex() == m_iBoneSelect)
		{
			return;
		}
	}

	CBY_CharBox box;
	box.Create(m_obj.m_pd3dDevice, m_obj.m_pContext);
	box.CreateBox(m_iBoneSelect, m_BindBone->m_ObjectList[m_iBoneSelect]->m_Bone.m_vPos, fX, fY, fZ);
	box.SetParentMatrix(&m_BindBone->m_ObjectList[m_iBoneSelect]->m_matCalculation);
	box.SetBindMatrix(&m_ObjList[0]->m_matBoneBindPoss[m_iBoneSelect]);
	m_BoxList.push_back(box);
}

void CBY_ObjectToolSet::SetCharBox(int id, float fX, float fY, float fZ)
{
	for (int ibox = 0; ibox < m_BoxList.size(); ibox++)
	{
		if (m_BoxList[ibox].GetBoneIndex() == m_iBoneSelect)
		{
			m_BoxList[ibox].SetBox(fX, fY, fZ, id);
			return;
		}
	}
}


CBY_ObjectToolSet::CBY_ObjectToolSet()
{
	m_iBoneSelect = -1;
	m_bViewCheck = true;
	bColBoxRender = false;
	m_bAllBoxRender = false;
	m_MtrViewIndex = 0;
	m_vRot = D3DXVECTOR3(0,0,0);
	m_vObjectMove = D3DXVECTOR3(0, 0, 0);
	m_fScale = 1.0f;
}


CBY_ObjectToolSet::~CBY_ObjectToolSet()
{
}

void CBY_ObjectToolSet::SetMTRApply(T_STR pszLoad, int state)
{
	m_dwAniType = CHAR_MTRTYPE;
	m_bViewCheck = false;

	if (m_StateList.size() > 0)
	{
		DWORD index = I_CHARACTER.BoneLoad(pszLoad, m_obj.m_pd3dDevice, m_obj.m_pContext);

		if (m_StateList.size() > state)
		{
			m_StateList[state].m_Bone = *I_CHARACTER.m_BoneLoadList[index];
		}
		else
		{
			for (int i = m_StateList.size(); i <= state; i++)
			{
				Char_State_Info info;
				info.m_Bone = *I_CHARACTER.m_BoneLoadList[index];
				m_StateList.push_back(info);
			}
		}
	}
	else
	{
		DWORD index = I_CHARACTER.BoneLoad(pszLoad, m_obj.m_pd3dDevice, m_obj.m_pContext);
		for (int i = m_StateList.size(); i <= state; i++)
		{
			Char_State_Info info;
			info.m_Bone = *I_CHARACTER.m_BoneLoadList[index];
			m_StateList.push_back(info);
		}
	}
}

void CBY_ObjectToolSet::SetFrameTime(int m_dwState, float start, float end)
{
	if (m_dwState >= m_StateList.size())
	{
		Char_State_Info info;
		info.m_iStartFrame = start;
		info.m_iEndFrame = end;
		m_StateList.push_back(info);
	}
	else
	{
		m_StateList[m_dwState].m_iStartFrame = start;
		m_StateList[m_dwState].m_iEndFrame = end;
	}
	m_bViewCheck = false;
}

void CBY_ObjectToolSet::CreateColBox()
{
	for (int iBox = 0; iBox < m_BoxList.size(); iBox++)
	{
		D3DXVECTOR3 size = m_BoxList[iBox].GetSize();
		m_BoxList[iBox].Create(m_obj.m_pd3dDevice, m_obj.m_pContext);
		m_BoxList[iBox].CreateBox(m_BoxList[iBox].GetBoneIndex(),
			m_BoxList[iBox].GetPos(), size.x, size.y, size.z);
	}
}

void CBY_ObjectToolSet::SetCharBox()
{
	m_ObjList[0]->m_ObjList[0]->m_CharBox.vCenter.y = (m_ObjList[0]->m_ObjList[0]->m_CharBox.vMax.y + m_ObjList[0]->m_ObjList[0]->m_CharBox.vMin.y) / 2;
	D3DXVECTOR3 size = m_ObjList[0]->m_ObjList[0]->m_CharBox.vMax - m_ObjList[0]->m_ObjList[0]->m_CharBox.vCenter;
	m_CharBox.Create(m_obj.m_pd3dDevice, m_obj.m_pContext);
	m_CharBox.CreateBox(0, m_ObjList[0]->m_ObjList[0]->m_CharBox.vCenter, size.x, size.y, size.z);
}

