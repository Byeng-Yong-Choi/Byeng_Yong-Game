#include "CBY_CharBox.h"

HRESULT CBY_CharBox::UpdateBoxVB()
{
	HRESULT hr = S_OK;
	m_VertexList.clear();
	m_VertexList.resize(24);
	// front
	m_VertexList[0] = PNCT_VERTEX(D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMin.z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMin.z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMin.z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMin.z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
	// Right
	m_VertexList[4] = PNCT_VERTEX(D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMin.z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[5] = PNCT_VERTEX(D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMax.z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[6] = PNCT_VERTEX(D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMin.z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 1.0f));
	m_VertexList[7] = PNCT_VERTEX(D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMax.z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
	// back
	m_VertexList[8] = PNCT_VERTEX(D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMax.z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[9] = PNCT_VERTEX(D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMax.z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[10] = PNCT_VERTEX(D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMax.z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 1.0f));
	m_VertexList[11] = PNCT_VERTEX(D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMax.z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
	// left
	m_VertexList[12] = PNCT_VERTEX(D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMax.z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[13] = PNCT_VERTEX(D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMin.z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[14] = PNCT_VERTEX(D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMax.z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 1.0f));
	m_VertexList[15] = PNCT_VERTEX(D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMin.z), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
	// up
	m_VertexList[16] = PNCT_VERTEX(D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMax.z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[17] = PNCT_VERTEX(D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMax.z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[18] = PNCT_VERTEX(D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMin.z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 1.0f));
	m_VertexList[19] = PNCT_VERTEX(D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMin.z), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
	// down
	m_VertexList[20] = PNCT_VERTEX(D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMax.z), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[21] = PNCT_VERTEX(D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMax.z), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[22] = PNCT_VERTEX(D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMin.z), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 1.0f));
	m_VertexList[23] = PNCT_VERTEX(D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMin.z), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));

	UpdateVertexData(m_VertexList);

	return hr;
}

HRESULT CBY_CharBox::CreateIndexData()
{
	HRESULT hr = S_OK;
	m_IndexData.resize(36);
	int iIndex = 0;
	m_IndexData[iIndex++] = 0; m_IndexData[iIndex++] = 1; m_IndexData[iIndex++] = 2;
	m_IndexData[iIndex++] = 2; m_IndexData[iIndex++] = 1; m_IndexData[iIndex++] = 3;
	m_IndexData[iIndex++] = 4; m_IndexData[iIndex++] = 5; m_IndexData[iIndex++] = 6;
	m_IndexData[iIndex++] = 6; m_IndexData[iIndex++] = 5; m_IndexData[iIndex++] = 7;

	m_IndexData[iIndex++] = 8; m_IndexData[iIndex++] = 9; m_IndexData[iIndex++] = 10;
	m_IndexData[iIndex++] = 10; m_IndexData[iIndex++] = 9; m_IndexData[iIndex++] = 11;
	m_IndexData[iIndex++] = 12; m_IndexData[iIndex++] = 13; m_IndexData[iIndex++] = 14;
	m_IndexData[iIndex++] = 14; m_IndexData[iIndex++] = 13; m_IndexData[iIndex++] = 15;

	m_IndexData[iIndex++] = 16; m_IndexData[iIndex++] = 17; m_IndexData[iIndex++] = 18;
	m_IndexData[iIndex++] = 18; m_IndexData[iIndex++] = 17; m_IndexData[iIndex++] = 19;
	m_IndexData[iIndex++] = 20; m_IndexData[iIndex++] = 21; m_IndexData[iIndex++] = 22;
	m_IndexData[iIndex++] = 22; m_IndexData[iIndex++] = 21; m_IndexData[iIndex++] = 23;

	return hr;
}


void CBY_CharBox::CreateBox(int index, D3DXVECTOR3 vPos, float fXsize, float fYsize, float fZsize, D3DXMATRIX mRot)
{
	m_iBoneIndex = index;

	m_vPos = vPos;
	m_Box.vCenter = vPos;

	m_vBoxSize.x = fXsize;
	m_vBoxSize.y = fYsize;
	m_vBoxSize.z = fZsize;

	//D3DXVECTOR3 center = D3DXVECTOR3(0, 0, 0);
	m_vMax = m_Box.vCenter + m_vBoxSize;
	m_vMin = m_Box.vCenter - m_vBoxSize;

	m_Box.vMax = m_Box.vCenter + m_vBoxSize;
	m_Box.vMin = m_Box.vCenter - m_vBoxSize;

	m_Box.fExtent[0] = m_vBoxSize.x;
	m_Box.fExtent[1] = m_vBoxSize.y;
	m_Box.fExtent[2] = m_vBoxSize.z;

	m_Box.vAxis[0] = D3DXVECTOR3(1, 0, 0);
	m_Box.vAxis[1] = D3DXVECTOR3(0, 1, 0);
	m_Box.vAxis[2] = D3DXVECTOR3(0, 0, 1);

	UpdateBoxVB();
}

void CBY_CharBox::UpdateBoxAxis(D3DXMATRIX mat)
{
	D3DXVECTOR3 vX = D3DXVECTOR3(1, 0, 0);
	D3DXVECTOR3 vY = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 vZ = D3DXVECTOR3(0, 0, 1);

	D3DXVec3TransformNormal(&vX, &vX, &mat);
	D3DXVec3TransformNormal(&vY, &vY, &mat);
	D3DXVec3TransformNormal(&vZ, &vZ, &mat);

	m_Box.vAxis[0] = vX;
	m_Box.vAxis[1] = vY;
	m_Box.vAxis[2] = vZ;
}


void CBY_CharBox::SetBox(float fXsize, float fYsize, float fZsize, int id)
{

	switch (id)
	{
	case 0:		//X Right
	{
		m_vMax.x += fXsize;
	}
	break;

	case 1:		//X Left
	{
		m_vMin.x -= fXsize;
	}
	break;

	case 2:		//Y Up
	{
		m_vMax.y += fYsize;
	}
	break;

	case 3:		//Y Down
	{
		m_vMin.y -= fYsize;
	}
	break;

	case 4:		//Z Front
	{
		m_vMin.z -= fZsize;
	}
	break;

	case 5:		//Z Back
	{
		m_vMax.z += fZsize;
	}
	break;

	}

	m_vPos = (m_vMax + m_vMin) / 2;
	m_Box.vCenter = m_vPos;

	m_Box.fExtent[0] = m_vMax.x - m_vPos.x;
	m_Box.fExtent[1] = m_vMax.y - m_vPos.y;
	m_Box.fExtent[2] = m_vMax.z - m_vPos.z;

	m_vBoxSize = m_Box.fExtent;
	m_vInitBoxSize = m_vBoxSize;

	D3DXVECTOR3 center = D3DXVECTOR3(0, 0, 0);
	m_vMax = center + m_vBoxSize;
	m_vMin = center - m_vBoxSize;

	UpdateBoxVB();
}


void CBY_CharBox::SetParentMatrix(D3DXMATRIX* mat)
{
	m_pParmatList.push_back(mat);
}

void CBY_CharBox::SetBindMatrix(D3DXMATRIX* mat)
{
	m_pBindmat = mat;
}

D3DXMATRIX CBY_CharBox::GetParentMatrix(int index)
{
	return *m_pParmatList[index];
}

D3DXMATRIX CBY_CharBox::GetBindMatrix()
{
	return *m_pBindmat;
}

D3DXMATRIX CBY_CharBox::GetWorldMatrix(int index)
{
	return (*m_pBindmat) * (*m_pParmatList[index]);
}

int CBY_CharBox::GetBoneIndex()
{
	return m_iBoneIndex;
}

D3DXVECTOR3 CBY_CharBox::GetSize()
{
	return m_vBoxSize;
}
D3DXVECTOR3 CBY_CharBox::GetPos()
{
	return m_vPos;
}


CBY_CharBox::CBY_CharBox()
{
	//D3DXMatrixIdentity(&m_Parmat);
	m_vBoxSize = D3DXVECTOR3(0, 0, 0);
	m_iBoneIndex = -1;
}

CBY_CharBox::~CBY_CharBox()
{
	m_VertexList.clear();
}

