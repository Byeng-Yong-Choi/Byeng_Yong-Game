#include "CBY_Weapon.h"

bool CBY_Weapon::Update(D3DXMATRIX* parmat)
{
	D3DXMATRIX poaworld;
	D3DXVECTOR3 vScale, vPos;
	D3DXQUATERNION qRot;
	D3DXMatrixDecompose(&vScale, &qRot, &vPos, parmat);
	D3DXMatrixRotationQuaternion(&poaworld, &qRot);
	poaworld._41 = vPos.x;
	poaworld._42 = vPos.y;
	poaworld._43 = vPos.z;

	m_fElapseTick = 0;
	if (m_dwAniType == CHAR_FRAMETYPE)
	{
		m_Bone->Update(m_StateList[m_dwState].m_iStartFrame, m_StateList[m_dwState].m_iEndFrame,
			m_fElapseTick, m_pMatrixList);
	}

	else if (m_dwAniType == CHAR_MTRTYPE)
	{
		m_Bone->ObjUpdate(0, 0, 0, m_pMatrixList, &poaworld, m_iObjSocket);
	}

	for (int iBone = 0; iBone < m_ObjList[0]->m_matBoneBindPoss.size(); iBone++)
	{
		D3DXMATRIX mat;
		mat = m_ObjList[0]->m_matBoneBindPoss[iBone] * m_pMatrixList[iBone];		//바인드 포즈는 공통이다
		D3DXMatrixTranspose(&m_cbBoneWorld.g_matConstBoneWorld[iBone], &mat);
	}

	D3D11_MAPPED_SUBRESOURCE mapResource;
	m_obj.m_pContext->Map(m_pCBConstBoneWorld.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	memcpy(mapResource.pData, &m_cbBoneWorld, sizeof(CBConstBoneWorld));
	m_obj.m_pContext->Unmap(m_pCBConstBoneWorld.Get(), 0);
	return true;
}

void CBY_Weapon::SetSocket(int i)
{
	m_iCharSocket = i;
}

int CBY_Weapon::GetSocket()
{
	return m_iCharSocket;
}

void CBY_Weapon::SetObjSocke(int i)
{
	m_iObjSocket = i;
}

int CBY_Weapon::GetObjSocke()
{
	return m_iObjSocket;
}

void CBY_Weapon::SetFireSocket(int i)
{
	m_iFireSocket = i;
}

int CBY_Weapon::GetFireSocket()
{
	return m_iFireSocket;
}

CBY_Weapon::CBY_Weapon()
{
	m_iCharSocket = -1;
	m_iObjSocket = -1;
	m_iFireSocket = -1;
}


CBY_Weapon::~CBY_Weapon()
{
}
