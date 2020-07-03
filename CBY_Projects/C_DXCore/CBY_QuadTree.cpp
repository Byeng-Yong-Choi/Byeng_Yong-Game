#include "CBY_QuadTree.h"
#include "C_Intersection.h"

D3DXVECTOR3 g_fPickpos;

void CBY_QuadTree::CreateBB(CNode* pNode)
{
	pNode->m_Box.vMin.x = m_pMap->m_VerTex[pNode->m_dwCorner[2]].p.x;
	pNode->m_Box.vMin.y = 99999.0f;
	pNode->m_Box.vMin.z = m_pMap->m_VerTex[pNode->m_dwCorner[2]].p.z;

	pNode->m_Box.vMax.x = m_pMap->m_VerTex[pNode->m_dwCorner[1]].p.x;
	pNode->m_Box.vMax.y = -99999.0f;
	pNode->m_Box.vMax.z = m_pMap->m_VerTex[pNode->m_dwCorner[1]].p.z;

	DWORD dwColStart = pNode->m_dwCorner[0] / m_pMap->m_iNumRows;
	DWORD dwColEnd = pNode->m_dwCorner[2] / m_pMap->m_iNumRows;
	DWORD dwRowStart = pNode->m_dwCorner[0] % m_pMap->m_iNumRows;
	DWORD dwRowEnd = pNode->m_dwCorner[1] % m_pMap->m_iNumRows;
	
	for (DWORD dwCol = dwColStart; dwCol < dwColEnd; dwCol++)
	{
		for (DWORD dwRow = dwRowStart; dwRow < dwRowEnd; dwRow++)
		{
			DWORD dwver = dwCol * m_pMap->m_iNumRows + dwRow;
			if (pNode->m_Box.vMin.y > m_pMap->m_VerTex[dwver].p.y)
			{				
				pNode->m_Box.vMin.y = m_pMap->m_VerTex[dwver].p.y;
			}				
							
			if (pNode->m_Box.vMax.y < m_pMap->m_VerTex[dwver].p.y)
			{				
				pNode->m_Box.vMax.y = m_pMap->m_VerTex[dwver].p.y;
			}
		}
	}

	pNode->m_Box.vCenter = (pNode->m_Box.vMax + pNode->m_Box.vMin) / 2;
	pNode->m_Box.vAxis[0] = D3DXVECTOR3(1, 0, 0);
	pNode->m_Box.vAxis[1] = D3DXVECTOR3(0, 1, 0);
	pNode->m_Box.vAxis[2] = D3DXVECTOR3(0, 0, 1);

	pNode->m_Box.fExtent[0] = pNode->m_Box.vMax.x - pNode->m_Box.vCenter.x;
	pNode->m_Box.fExtent[1] = pNode->m_Box.vMax.y - pNode->m_Box.vCenter.y;
	pNode->m_Box.fExtent[2] = pNode->m_Box.vMax.z - pNode->m_Box.vCenter.z;

	pNode->m_Box.fExtentXZ = sqrt((pNode->m_Box.fExtent[0] * pNode->m_Box.fExtent[0]) + (pNode->m_Box.fExtent[2] * pNode->m_Box.fExtent[2]));

	//pNode->m_BoxIndexList.resize(36);
	//pNode->m_BoxVertexList.resize(8);
	//pNode->m_BoxVertexList[0] = D3DXVECTOR3(pNode->m_Box.vMin.x, pNode->m_Box.vMax.y, pNode->m_Box.vMin.z);
	//pNode->m_BoxVertexList[1] = D3DXVECTOR3(pNode->m_Box.vMax.x, pNode->m_Box.vMax.y, pNode->m_Box.vMin.z);
	//pNode->m_BoxVertexList[2] = D3DXVECTOR3(pNode->m_Box.vMin.x, pNode->m_Box.vMin.y, pNode->m_Box.vMin.z);
	//pNode->m_BoxVertexList[3] = D3DXVECTOR3(pNode->m_Box.vMax.x, pNode->m_Box.vMin.y, pNode->m_Box.vMin.z);

	//pNode->m_BoxVertexList[4] = D3DXVECTOR3(pNode->m_Box.vMin.x, pNode->m_Box.vMax.y, pNode->m_Box.vMax.z);
	//pNode->m_BoxVertexList[5] = D3DXVECTOR3(pNode->m_Box.vMax.x, pNode->m_Box.vMax.y, pNode->m_Box.vMax.z);
	//pNode->m_BoxVertexList[6] = D3DXVECTOR3(pNode->m_Box.vMin.x, pNode->m_Box.vMin.y, pNode->m_Box.vMax.z);
	//pNode->m_BoxVertexList[7] = D3DXVECTOR3(pNode->m_Box.vMax.x, pNode->m_Box.vMin.y, pNode->m_Box.vMax.z);

	//DWORD index = 0;

	//pNode->m_BoxIndexList[index++] = 0; pNode->m_BoxIndexList[index++] = 1; pNode->m_BoxIndexList[index++] = 2;
	//pNode->m_BoxIndexList[index++] = 2; pNode->m_BoxIndexList[index++] = 1; pNode->m_BoxIndexList[index++] = 3;
	////앞
	//pNode->m_BoxIndexList[index++] = 1; pNode->m_BoxIndexList[index++] = 5; pNode->m_BoxIndexList[index++] = 3;
	//pNode->m_BoxIndexList[index++] = 3; pNode->m_BoxIndexList[index++] = 5; pNode->m_BoxIndexList[index++] = 7;
	////오
	//pNode->m_BoxIndexList[index++] = 5; pNode->m_BoxIndexList[index++] = 4; pNode->m_BoxIndexList[index++] = 7;
	//pNode->m_BoxIndexList[index++] = 7; pNode->m_BoxIndexList[index++] = 4; pNode->m_BoxIndexList[index++] = 6;
	////뒤
	//pNode->m_BoxIndexList[index++] = 4; pNode->m_BoxIndexList[index++] = 0; pNode->m_BoxIndexList[index++] = 6;
	//pNode->m_BoxIndexList[index++] = 6; pNode->m_BoxIndexList[index++] = 0; pNode->m_BoxIndexList[index++] = 2;
	////왼
	//pNode->m_BoxIndexList[index++] = 4; pNode->m_BoxIndexList[index++] = 5; pNode->m_BoxIndexList[index++] = 0;
	//pNode->m_BoxIndexList[index++] = 0; pNode->m_BoxIndexList[index++] = 5; pNode->m_BoxIndexList[index++] = 1;
	////상
	//pNode->m_BoxIndexList[index++] = 6; pNode->m_BoxIndexList[index++] = 7; pNode->m_BoxIndexList[index++] = 2;
	//pNode->m_BoxIndexList[index++] = 2; pNode->m_BoxIndexList[index++] = 7; pNode->m_BoxIndexList[index++] = 3;
	////하
}

void CBY_QuadTree::UpdateBBY(CNode* pNode)
{
	pNode->m_Box.vMin.y = m_pMap->m_VerTex[pNode->m_dwCorner[2]].p.y;
	pNode->m_Box.vMax.y = m_pMap->m_VerTex[pNode->m_dwCorner[1]].p.y;

	DWORD dwColStart = pNode->m_dwCorner[0] / m_pMap->m_iNumRows;
	DWORD dwColEnd = pNode->m_dwCorner[2] / m_pMap->m_iNumRows;
	DWORD dwRowStart = pNode->m_dwCorner[0] % m_pMap->m_iNumRows;
	DWORD dwRowEnd = pNode->m_dwCorner[1] % m_pMap->m_iNumRows;

	for (DWORD dwCol = dwColStart; dwCol < dwColEnd; dwCol++)
	{
		for (DWORD dwRow = dwRowStart; dwRow < dwRowEnd; dwRow++)
		{
			DWORD dwver = dwCol * m_pMap->m_iNumRows + dwRow;
			if (pNode->m_Box.vMin.y > m_pMap->m_VerTex[dwver].p.y)
			{
				pNode->m_Box.vMin.y = m_pMap->m_VerTex[dwver].p.y;
			}

			if (pNode->m_Box.vMax.y < m_pMap->m_VerTex[dwver].p.y)
			{
				pNode->m_Box.vMax.y = m_pMap->m_VerTex[dwver].p.y;
			}
		}
	}

	pNode->m_Box.vCenter = (pNode->m_Box.vMax + pNode->m_Box.vMin) / 2;
	pNode->m_Box.fExtent[1] = pNode->m_Box.vMax.y - pNode->m_Box.vCenter.y;
}

CNode*	CBY_QuadTree::CreateNode(CNode* pParent,DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR)
{
	CNode* pNode = 0;
	SAFE_NEW(pNode, CNode);
	pNode->m_dwCorner[0] = dwTL;
	pNode->m_dwCorner[1] = dwTR;
	pNode->m_dwCorner[2] = dwBL;
	pNode->m_dwCorner[3] = dwBR;

	CreateBB(pNode);

	if (pParent != nullptr)
		pNode->m_dwDepth = pParent->m_dwDepth + 1;
	else
		pNode->m_dwDepth = 0;
	return pNode;
}
bool CBY_QuadTree::Build(C_Map_shape* pMap, CCamera* pCamera)
{
	m_pMap = pMap;
	m_pCamera = pCamera;
	DWORD dwTL = 0;
	DWORD dwTR = m_pMap->m_iNumRows - 1;
	DWORD dwBL = m_pMap->m_iNumCols * m_pMap->m_iNumRows - m_pMap->m_iNumRows;
	DWORD dwBR = m_pMap->m_iNumCols * m_pMap->m_iNumRows - 1;
	m_pRootNode = CreateNode(nullptr, dwTL, dwTR, dwBL, dwBR);
	BuildTree(m_pRootNode);
	m_AxisLine.Create(pMap->m_obj.m_pd3dDevice, pMap->m_obj.m_pContext, L"../../data/shader/LineShader.txt", nullptr, "VS", "PS");
	return true;
}

bool CBY_QuadTree::SubDivide(CNode* pNode)
{
	bool bSub=false;
	//if ((1 < (pNode->m_dwCorner[1] - pNode->m_dwCorner[0])) || (1 > (pNode->m_dwCorner[2] - pNode->m_dwCorner[0])))
	DWORD dwTL = pNode->m_dwCorner[0];
	DWORD dwTR = pNode->m_dwCorner[1];
	DWORD dwBL = pNode->m_dwCorner[2];
	DWORD dwBR = pNode->m_dwCorner[3];

	DWORD c0 = (dwTR + dwTL) / 2;
	DWORD c2 = (dwBR + dwTL) / 2;
	DWORD c1 = (dwBL + dwTL) / 2;
	DWORD c3 = (dwBR + dwTR) / 2;
	DWORD c4 = (dwBR + dwBL) / 2;

	if(pNode->m_dwDepth<3 && (dwTR - dwTL) > m_pMap->m_MapDesc.fCellDistance)
	{
		bSub = true;
	}

	if (bSub)
	{
		pNode->m_pChild[0] = CreateNode(pNode, dwTL, c0, c1, c2);
		pNode->m_pChild[1] = CreateNode(pNode, c0, dwTR, c2, c3);
		pNode->m_pChild[2] = CreateNode(pNode, c1, c2, dwBL, c4);
		pNode->m_pChild[3] = CreateNode(pNode, c2, c3, c4, dwBR);
		return true;
	}
	pNode->m_isLeaf = TRUE;
	CreateIndexData(pNode);
	return false;
}
bool CBY_QuadTree::BuildTree(CNode* pNode)
{
	if (SubDivide(pNode))
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			BuildTree(pNode->m_pChild[iNode]);
		}
	}
	return true;
}

bool CBY_QuadTree::ReleaseTree(CNode* pNode)
{
	if (pNode == nullptr)return true;
	if (pNode->m_isLeaf==TRUE)
	{
		delete pNode;
	}
	else
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			ReleaseTree(pNode->m_pChild[iNode]);
		}
		delete pNode;
	}

	return true;
}

void CBY_QuadTree::CreateIndexData(CNode* pNode)
{
	DWORD dwColStart = pNode->m_dwCorner[0] / m_pMap->m_iNumRows;
	DWORD dwColEnd = pNode->m_dwCorner[2] / m_pMap->m_iNumRows;
	DWORD dwRowStart = pNode->m_dwCorner[0] % m_pMap->m_iNumRows;
	DWORD dwRowEnd = pNode->m_dwCorner[1] % m_pMap->m_iNumRows;

	DWORD IndexSize = (pNode->m_dwCorner[1] - pNode->m_dwCorner[0]) * ((pNode->m_dwCorner[2] - pNode->m_dwCorner[0]) / m_pMap->m_iNumRows) * 2 * 3;
	pNode->m_IndexList.resize(IndexSize);

	DWORD indexnum = 0;
	for (DWORD dwCol = dwColStart; dwCol < dwColEnd; dwCol++)
	{
		for (DWORD dwRow = dwRowStart; dwRow < dwRowEnd; dwRow++)
		{
			DWORD dwRowNext= dwRow+1;
			DWORD dwColNext= dwCol+1;
			
			pNode->m_IndexList[indexnum++] = dwCol * m_pMap->m_iNumRows + dwRow;
			pNode->m_IndexList[indexnum++] = dwCol * m_pMap->m_iNumRows + dwRowNext;
			pNode->m_IndexList[indexnum++] = dwColNext * m_pMap->m_iNumRows + dwRow;

			pNode->m_IndexList[indexnum++] = dwColNext * m_pMap->m_iNumRows + dwRow;
			pNode->m_IndexList[indexnum++] = dwCol * m_pMap->m_iNumRows + dwRowNext;
			pNode->m_IndexList[indexnum++] = dwColNext * m_pMap->m_iNumRows + dwRowNext;
		}
	}

	pNode->m_pIndexBuffer.Attach(CDXH::CreateIndexBuffer(m_pMap->m_obj.m_pd3dDevice, &pNode->m_IndexList.at(0), pNode->m_IndexList.size(), sizeof(DWORD)));
}

void CBY_QuadTree::Draw(CNode* pNode, ID3D11DeviceContext* pContext)
{
	if (pNode == nullptr) return;
	if (pNode->m_isLeaf==TRUE)
	{
		
		pContext->IASetIndexBuffer(pNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		pContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);
	}
	else
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			Draw(pNode->m_pChild[iNode], m_pMap->m_obj.m_pContext);
		}
	}
}

void CBY_QuadTree::DrawFindNode(CNode* pNode)
{
	if (pNode == nullptr) return;
	int pos = m_pCamera->CheckOBBInPlane(pNode->m_Box);

	if (pNode->m_isLeaf == TRUE && pos != P_BACK)
	{
		m_DrawNodeList.push_back(pNode);
		return;
	}
	if (pos == P_FRONT)
	{
		VisibleNode(pNode);
		return;
	}
	for (int iNode = 0; iNode < 4; iNode++)
	{
		DrawFindNode(pNode->m_pChild[iNode]);
	}
}

void CBY_QuadTree::VisibleNode(CNode* pNode)
{
	if (pNode == nullptr) return;
	if (pNode->m_isLeaf == TRUE)
	{
		m_DrawNodeList.push_back(pNode);
		return;
	}
	for (int iNode = 0; iNode < 4; iNode++)
	{
		VisibleNode(pNode->m_pChild[iNode]);
	}
}

bool CBY_QuadTree::Frame()
{
	m_DrawNodeList.clear();
	DrawFindNode(m_pRootNode);
	return true;
}

bool CBY_QuadTree::Render()
{
	m_pMap->PreRender();
	for (int iNode = 0; iNode < m_DrawNodeList.size(); iNode++)
	{
		CNode* pNode = m_DrawNodeList[iNode];

		m_pMap->m_vBuf0.i = pNode->m_dwTilenum;
		
		m_pMap->m_obj.m_pContext->IASetIndexBuffer(pNode->m_pIndexBuffer.Get(),
			DXGI_FORMAT_R32_UINT, 0);
		m_pMap->m_obj.m_pContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);
	}
	//Draw(m_pRootNode, m_pMap->m_obj.m_pContext);
	return true;
}

void CBY_QuadTree::NonPreRender()
{
	for (int iNode = 0; iNode < m_DrawNodeList.size(); iNode++)
	{
		CNode* pNode = m_DrawNodeList[iNode];

		m_pMap->m_vBuf0.i = pNode->m_dwTilenum;

		m_pMap->m_obj.m_pContext->IASetIndexBuffer(pNode->m_pIndexBuffer.Get(),
			DXGI_FORMAT_R32_UINT, 0);
		m_pMap->m_obj.m_pContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);
	}
}

bool CBY_QuadTree::Release()
{
	m_PickNodeList.clear();
	ReleaseTree(m_pRootNode);
	return true;
}


void CBY_QuadTree::DrawLine(CNode* pNode, bool bpick)
{
	m_AxisLine.SetMatrix(nullptr,
		&m_pCamera->m_View,
		&m_pCamera->m_Proj);

	D3DXVECTOR4 color;
	if (bpick)
	{
		color = { 0, 1, 0, 1 };
	}
	else
	{
		color = { 1, 0, 0, 1 };
	}

	D3DXVECTOR3 vS;
	vS.x = pNode->m_Box.vMin.x;
	vS.y = pNode->m_Box.vMax.y;
	vS.z = pNode->m_Box.vMax.z;
	D3DXVECTOR3 vE;
	vE.x = pNode->m_Box.vMax.x;
	vE.y = pNode->m_Box.vMax.y;
	vE.z = pNode->m_Box.vMax.z;
	m_AxisLine.Draw(vE, vS, color);
	vS.x = pNode->m_Box.vMax.x;
	vS.y = pNode->m_Box.vMax.y;
	vS.z = pNode->m_Box.vMin.z;
	m_AxisLine.Draw(vE, vS, color);
	vE.x = pNode->m_Box.vMin.x;
	vE.y = pNode->m_Box.vMax.y;
	vE.z = pNode->m_Box.vMin.z;
	m_AxisLine.Draw(vS, vE, color);
	vS.x = pNode->m_Box.vMin.x;
	vS.y = pNode->m_Box.vMax.y;
	vS.z = pNode->m_Box.vMax.z;
	m_AxisLine.Draw(vS, vE, color);


	vS.x = pNode->m_Box.vMin.x;
	vS.y = pNode->m_Box.vMin.y;
	vS.z = pNode->m_Box.vMax.z;
	vE.x = pNode->m_Box.vMax.x;
	vE.y = pNode->m_Box.vMin.y;
	vE.z = pNode->m_Box.vMax.z;
	m_AxisLine.Draw(vE, vS, color);
	vS.x = pNode->m_Box.vMax.x;
	vS.y = pNode->m_Box.vMin.y;
	vS.z = pNode->m_Box.vMin.z;
	m_AxisLine.Draw(vE, vS, color);
	vE.x = pNode->m_Box.vMin.x;
	vE.y = pNode->m_Box.vMin.y;
	vE.z = pNode->m_Box.vMin.z;
	m_AxisLine.Draw(vS, vE, color);
	vS.x = pNode->m_Box.vMin.x;
	vS.y = pNode->m_Box.vMin.y;
	vS.z = pNode->m_Box.vMax.z;
	m_AxisLine.Draw(vS, vE, color);


	vS.x = pNode->m_Box.vMin.x;
	vS.y = pNode->m_Box.vMax.y;
	vS.z = pNode->m_Box.vMax.z;
	vE.x = pNode->m_Box.vMin.x;
	vE.y = pNode->m_Box.vMin.y;
	vE.z = pNode->m_Box.vMax.z;
	m_AxisLine.Draw(vE, vS, color);
	vS.x = pNode->m_Box.vMax.x;
	vS.y = pNode->m_Box.vMax.y;
	vS.z = pNode->m_Box.vMax.z;
	vE.x = pNode->m_Box.vMax.x;
	vE.y = pNode->m_Box.vMin.y;
	vE.z = pNode->m_Box.vMax.z;
	m_AxisLine.Draw(vE, vS, color);
	vS.x = pNode->m_Box.vMax.x;
	vS.y = pNode->m_Box.vMax.y;
	vS.z = pNode->m_Box.vMin.z;
	vE.x = pNode->m_Box.vMax.x;
	vE.y = pNode->m_Box.vMin.y;
	vE.z = pNode->m_Box.vMin.z;
	m_AxisLine.Draw(vS, vE, color);
	vS.x = pNode->m_Box.vMin.x;
	vS.y = pNode->m_Box.vMax.y;
	vS.z = pNode->m_Box.vMin.z;
	vE.x = pNode->m_Box.vMin.x;
	vE.y = pNode->m_Box.vMin.y;
	vE.z = pNode->m_Box.vMin.z;
	m_AxisLine.Draw(vS, vE, color);
}

void	CBY_QuadTree::DrawNodeLine()
{
	m_pMap->PreRender();
	bool bpick, bloof;
	bloof=true;
	for (int iNode = 0; iNode < m_DrawNodeList.size(); iNode++)
	{
		bpick = false;
		CNode* pNode = m_DrawNodeList[iNode];
		if (iNode == iPicknum)
		{
			bpick = true;
			//iPicknum = -1;
			m_bPick = false;
		}
		DrawLine(pNode, bpick);
	}
}

void CBY_QuadTree::PickingSelect()
{
	m_vIntersection = D3DXVECTOR3(-1, -1, -1);
	//MousePicking();
	//if (m_bPick)
	if(MousePicking())
	{
		FacePick();
	}
	g_fPickpos = m_vIntersection;
}

bool CBY_QuadTree::MousePicking()
{
	m_PickNodeList.clear();
	bool bpick = false;
	float t = 999999.0f;
	for (int iNode = 0; iNode < m_DrawNodeList.size(); iNode++)
	{
		CNode* pNode = m_DrawNodeList[iNode];
		if (GetPickingData(pNode))
		{
			bpick = true;
			m_PickNodeList.push_back(pNode);
			/*if (pNode->fPick_t < t)
			{
				t = pNode->fPick_t;
				iPicknum = iNode;
			}*/
		}
	}

	return bpick;
}

void CBY_QuadTree::SetHeightMap(int mode)
{
	if (!MousePicking())
	{
		return;
	}
	//if (iPicknum == -1)return;

	FacePick();

	float HeightRadian = m_Radian * 2;		//지형올리고 내릴 반지름
	float fDistance;						

	for (int iNode = 0; iNode < m_DrawNodeList.size(); iNode++)
	{
		CNode* pNode = m_DrawNodeList[iNode];
		fDistance= I_Col.GetDistance(pNode->m_Box.vCenter.x, pNode->m_Box.vCenter.z,
									m_vIntersection.x, m_vIntersection.z);
		if (fDistance <= (pNode->m_Box.fExtentXZ+ HeightRadian))
		{
			SetHeight(pNode, mode);
			UpdateBBY(pNode);
			UpdateNomal(pNode);
		}
	}
	m_pMap->m_obj.m_pContext->UpdateSubresource(m_pMap->m_obj.m_pVertexBuffer.Get(), 0, nullptr, &m_pMap->m_VerTex.at(0), 0, 0);
	iPicknum = -1;
}

void CBY_QuadTree::UpdateNomal(CNode* pNode)
{
	PNCT_VERTEX* ver[3];
	DWORD dwnum = 0;
	float fDistance;
	float HeightRadian = m_Radian * 2;
	for (DWORD index = 0; index < pNode->m_IndexList.size(); index++)
	{
		if (dwnum == 2)
		{
			DWORD dwindex = pNode->m_IndexList[index-2];
			ver[0] = &m_pMap->m_VerTex[dwindex];
			dwindex = pNode->m_IndexList[index-1];
			ver[1] = &m_pMap->m_VerTex[dwindex];
			dwindex = pNode->m_IndexList[index];
			ver[2] = &m_pMap->m_VerTex[dwindex];

			D3DXVECTOR3 E0, E1, facenomal;
			E0 = ver[1]->p - ver[0]->p;
			E1 = ver[2]->p - ver[0]->p;
			D3DXVec3Cross(&facenomal, &E0, &E1);
			D3DXVec3Normalize(&facenomal, &facenomal);
			ver[0]->n += facenomal;
			ver[1]->n += facenomal;
			ver[2]->n += facenomal;
		
			dwnum = 0;
		}
		else
		{
			dwnum++;
		}
	}
	for (DWORD index = 0; index < pNode->m_IndexList.size(); index++)
	{
		DWORD dwIndex = pNode->m_IndexList[index];
		D3DXVec3Normalize(&m_pMap->m_VerTex[dwIndex].n, &m_pMap->m_VerTex[dwIndex].n);
	}
}

void CBY_QuadTree::SetHeight(CNode* pNode,int mode)
{
	D3DXVECTOR3 v0;
	DWORD dwnum = 0;
	float fDistance;
	float HeightRadian = m_Radian * 2;
	for (DWORD index = 0; index < pNode->m_IndexList.size(); index++)
	{
		if (dwnum == 2)
		{
			for (int i = 0; i < 3; i++)
			{
				DWORD dwindex = pNode->m_IndexList[index - i];
				v0 = m_pMap->m_VerTex[dwindex].p;
				fDistance= I_Col.GetDistance(v0.x, v0.z, m_vIntersection.x, m_vIntersection.z);
				//fDistance = D3DXVec3Length(&(v0 - m_vIntersection));
				
				if (fDistance <= HeightRadian)
				{
					float value = (1 - I_Col.Smoothstep(0, HeightRadian, fDistance)) * 90;
					float fHeight = I_Col.Lerp(0, m_Radian * 4, sin(value*(3.141592 / 180)));
					if (mode == 0)
					{
						m_pMap->m_VerTex[dwindex].p.y = 0;
					}
					else
					{
						m_pMap->m_VerTex[dwindex].p.y += mode*g_SecondTime*fHeight;
					}
				}
			}
			dwnum = 0;
		}
		else
		{
			dwnum++;
		}
	}
}

void CBY_QuadTree::TileSet(DWORD num)
{
	/*MousePicking();
	if (iPicknum == -1)return;*/
	if (!MousePicking())
	{
		return;
	}

	m_DrawNodeList[iPicknum]->m_dwTilenum = num;
	iPicknum = -1;
}

void CBY_QuadTree::SetRadian(float rad)
{
	m_Radian = rad;
}

void CBY_QuadTree::SplattingBufSet(BufSplatting* buf, int i)
{
	if (!MousePicking())
	{
		return;
	}

	FacePick();
	buf->vIntersection = m_vIntersection;
	buf->index = i;
	buf->fRadan = m_Radian;
	buf->cellsize = m_pMap->m_MapDesc.fCellDistance;
	buf->iCol = m_pMap->m_MapDesc.iNumCols;
	buf->iRow = m_pMap->m_MapDesc.iNumRows;
}

void CBY_QuadTree::Splatting(ID3D11Texture2D* text,int i)
{
	/*MousePicking();
	if (iPicknum == -1)return;*/
	if (!MousePicking())
	{
		return;
	}

	FacePick();
	
	D3DXVECTOR3 vDistance;
	float fDistance;//=D3DXVec3Length()

	D3D11_TEXTURE2D_DESC desc;
	text->GetDesc(&desc);

	float fHalfCols = (desc.Height - 1) / 2.0f;
	float fHalfRows = (desc.Width - 1) / 2.0f;
	
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(m_pMap->m_obj.m_pContext->Map((ID3D11Resource*)text, 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;

		for (UINT y = 0; y < desc.Height; y++)
		{
			for (UINT x = 0; x < desc.Width; x++)
			{	
				int pixel = y * MappedFaceDest.RowPitch + (x * 4);
				BYTE* R = &pDestBytes[pixel];
				BYTE* G = &pDestBytes[pixel] + 1;
				BYTE* B = &pDestBytes[pixel] + 2;
				BYTE* A = &pDestBytes[pixel] + 3;

				D3DXVECTOR3 vpix;
				vpix.x = (x - fHalfRows) * m_pMap->m_MapDesc.fCellDistance;
				//vpix.y = 0.0f;
				vpix.z= -((y - fHalfCols) * m_pMap->m_MapDesc.fCellDistance); 

				fDistance= I_Col.GetDistance(vpix.x, vpix.z, m_vIntersection.x, m_vIntersection.z);
				//vDistance = m_vIntersection - vpix;
				//fDistance = D3DXVec3Length(&vDistance);

				float a = 1 - I_Col.Smoothstep(m_Radian, m_Radian + 5, fDistance);
				a *= 255;
				if (a == 0)
				{
					continue;
				}

				switch (i)
				{
				case 0:
				{
					if (*R < a)
					{
						*R = a;
					}
				}
				break;

				case 1:
				{
					if (*G < a)
					{
						*G = a;
					}
				}
				break;

				case 2:
				{
					if (*B < a)
					{
						*B = a;
					}
				}
				break;

				case 3:
				{
					if (*A < a)
					{
						*A = a;
					}
				}
				break;
				}
			}
		}
		m_pMap->m_obj.m_pContext->Unmap(text, 0);
	}
	//iPicknum = -1;
}

bool CBY_QuadTree::GetPickingData(CNode* pNode)
{
	//pNode->fPick_t = 999;
	DWORD dwnum = 0;
	D3DXVECTOR3 v0, v1, v2;
	bool bpick = false;
	bool check;

	bpick = m_pSelect->ChkOBBToRay(&pNode->m_Box);
	m_bPick = bpick;
	//for (DWORD index = 0; index < pNode->m_BoxIndexList.size(); index++)
	//{
	//	if (dwnum == 2)
	//	{
	//		DWORD dwindex = pNode->m_BoxIndexList[index - 2];
	//		v0 = pNode->m_BoxVertexList[dwindex];
	//		dwindex = pNode->m_BoxIndexList[index - 1];
	//		v1 = pNode->m_BoxVertexList[dwindex];
	//		dwindex = pNode->m_BoxIndexList[index];
	//		v2 = pNode->m_BoxVertexList[dwindex];

	//		
	//		check = m_pSelect->PickCheck(&pNode->fPick_t, v0, v1, v2);

	//		if (check)
	//		{
	//			m_bPick = true;	//
	//			bpick = true;
	//		}
	//		dwnum = 0;
	//	}
	//	else
	//	{
	//		dwnum++;
	//	}
	//}

	return bpick;
}

void CBY_QuadTree::FacePick()
{
	DWORD dwnum = 0;
	D3DXVECTOR3 v0, v1, v2;
	bool bpick = false;
	bool check;
	float f=9999;

	for (DWORD iNode = 0; iNode < m_PickNodeList.size(); iNode++)
	{
		CNode* pNode = m_PickNodeList[iNode];

		for (DWORD index = 0; index < pNode->m_IndexList.size(); index++)
		{
			if (dwnum == 2)
			{
				DWORD dwindex = pNode->m_IndexList[index - 2];
				v0 = m_pMap->m_VerTex[dwindex].p;
				dwindex = pNode->m_IndexList[index - 1];
				v1 = m_pMap->m_VerTex[dwindex].p;
				dwindex = pNode->m_IndexList[index];
				v2 = m_pMap->m_VerTex[dwindex].p;

				check = m_pSelect->PickCheck(&f, v0, v1, v2);

				if (check)
				{
					m_vIntersection = m_pSelect->GetIntersection(f);
					g_fPickpos = m_vIntersection;
					return;
				}
				dwnum = 0;
			}
			else
			{
				dwnum++;
			}
		}
	}
}

void CBY_QuadTree::GetSelect(CBY_Select* select)
{
	m_pSelect = select;
}

void CBY_QuadTree::SaveheightMap(ID3D11Texture2D* text)
{
	D3D11_TEXTURE2D_DESC desc;
	text->GetDesc(&desc);

	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;

	if (SUCCEEDED(m_pMap->m_obj.m_pContext->Map((ID3D11Resource*)text, 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		DWORD dwIndex=0;
		for (UINT y = 0; y < desc.Height; y++)
		{
			for (UINT x = 0; x < desc.Width; x++)
			{
				int pixel = y * MappedFaceDest.RowPitch + (x * 4);
				BYTE* R = &pDestBytes[pixel];
				BYTE* G = &pDestBytes[pixel+1];

				if (m_pMap->m_VerTex[dwIndex].p.y >= 0)
				{
					*R = m_pMap->m_VerTex[dwIndex].p.y;
				}
				else
				{
					*G = -(m_pMap->m_VerTex[dwIndex].p.y);
				}
				dwIndex++;
			}
		}
		m_pMap->m_obj.m_pContext->Unmap(text, 0);
	}
}

CBY_QuadTree::CBY_QuadTree()
{
	iPicknum = -1;
	m_Radian = 1;
}


CBY_QuadTree::~CBY_QuadTree()
{
}

