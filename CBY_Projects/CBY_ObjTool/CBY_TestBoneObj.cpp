#include "stdafx.h"
#include "CBY_TestBoneObj.h"

bool	CBY_TestBoneObj::Load(T_STR loadfile, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
	m_obj.m_pd3dDevice = pd3dDevice;
	m_obj.m_pContext = pContext;
	if (!m_BLoader.MtrBinaryLoad(loadfile.c_str()))
	{
		return false;
	}

	Convert(m_VertexDataSkin);

	//BoneCreate();

	return true;
}

void    CBY_TestBoneObj::Convert(std::vector<PNCT2IW_VERTEX>& list)
{
	int iBaseVertex = 0;
	int iBaseIndex = 0;
	int iBaseOffset = 0;

	for (int iObj = 0; iObj < m_BLoader.m_ObjList.size(); iObj++)
	{
		CBY_MeshSkin* mesh = m_BLoader.m_ObjList[iObj];

		mesh->m_iParentIndex = m_BLoader.GetParentIndex(mesh->m_szParentName);


		D3DXMATRIX matInvParent, matAnim;
		D3DXMatrixIdentity(&matInvParent);
		if (mesh->m_iParentIndex >= 0)
		{
			mesh->m_Parent = m_BLoader.m_ObjList[mesh->m_iParentIndex];
			m_BLoader.m_ObjList[mesh->m_iParentIndex]->m_Child.push_back(mesh);
			D3DXMatrixInverse(&matInvParent, NULL, &m_BLoader.m_ObjList[mesh->m_iParentIndex]->m_matWorld);
		}
		matAnim = mesh->m_matWorld * matInvParent;
		D3DXMatrixDecompose(&mesh->m_vAnimScale, &mesh->m_qAnimRotation, &mesh->m_vAnimPos, &matAnim);
		D3DXQuaternionIdentity(&mesh->m_qAnimScaleRotation);


		int iRef = mesh->m_iTexIndex;
		if (iRef >= 0)
		{
			mesh->subMeshSkin.resize(
				m_BLoader.m_MtlList[iRef].SubMaterial.size());
		}


		if (mesh->subMeshSkin.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh < mesh->subMeshSkin.size(); iSubMesh++)
			{
				if (iRef >= 0)
				{
					if (m_BLoader.m_MtlList[iRef].SubMaterial[iSubMesh].texList.size() > 0)
					{
						mesh->subMeshSkin[iSubMesh].m_iTexIndex =
							I_Texture.Add(m_obj.m_pd3dDevice,
								m_BLoader.m_MtlList[iRef].SubMaterial[iSubMesh].texList[0].szTextureName,
								L"../../data/obj/");
						mesh->subMeshSkin[iSubMesh].m_pTexture =
							I_Texture.GetPtr(mesh->subMeshSkin[iSubMesh].m_iTexIndex);
					}
				}

				mesh->subMeshSkin[iSubMesh].m_iNumVertex =
					mesh->subMeshSkin[iSubMesh].listSkin.size();

				CreateVIData(&mesh->subMeshSkin[iSubMesh]);

				mesh->subMeshSkin[iSubMesh].m_iBaseIndex = iBaseIndex;
				mesh->subMeshSkin[iSubMesh].m_iBaseVertex = iBaseVertex;

				mesh->subMeshSkin[iSubMesh].m_iNumVertex = mesh->subMeshSkin[iSubMesh].vblistSkin.size();
				mesh->subMeshSkin[iSubMesh].m_iNumIndex = mesh->subMeshSkin[iSubMesh].iblistSkin.size();

				iBaseVertex += mesh->subMeshSkin[iSubMesh].m_iNumVertex;
				iBaseIndex += mesh->subMeshSkin[iSubMesh].m_iNumIndex;


				std::copy(mesh->subMeshSkin[iSubMesh].vblistSkin.begin(),
					mesh->subMeshSkin[iSubMesh].vblistSkin.end(),
					back_inserter(list));

				std::copy(mesh->subMeshSkin[iSubMesh].iblistSkin.begin(),
					mesh->subMeshSkin[iSubMesh].iblistSkin.end(),
					back_inserter(m_IndexData));
			}
		}
		else
		{
			if (iRef >= 0)
			{
				mesh->m_iTexIndex =
					I_Texture.Add(m_obj.m_pd3dDevice,
						m_BLoader.m_MtlList[iRef].texList[0].szTextureName,
						L"../../data/obj/");
				mesh->m_pTexture = I_Texture.GetPtr(mesh->m_iTexIndex);
			}

			CreateVIData(mesh);


			mesh->m_iBaseVertex = iBaseVertex;
			mesh->m_iBaseIndex = iBaseIndex;

			mesh->m_iNumVertex = mesh->vblistSkin.size();
			mesh->m_iNumIndex = mesh->iblistSkin.size();

			iBaseVertex += mesh->m_iNumVertex;
			iBaseIndex += mesh->m_iNumIndex;


			std::copy(mesh->vblistSkin.begin(), mesh->vblistSkin.end(), back_inserter(list));
			std::copy(mesh->iblistSkin.begin(), mesh->iblistSkin.end(), back_inserter(m_IndexData));
		}

		if (mesh->m_bRoot)
		{
			m_iRootList.push_back(iObj);
		}

		if (mesh->m_bBone)
		{
			mesh->m_BoneBox.SetBox(mesh->m_Bone.m_Box.vMin, mesh->m_Bone.m_Box.vMax);
			mesh->m_BoneBox.Create(m_obj.m_pd3dDevice, m_obj.m_pContext);
		}
	}
	m_ObjectList.swap(m_BLoader.m_ObjList);
	m_Scene = m_BLoader.m_Scene;
	m_matBoneBindPoss.swap(m_BLoader.m_matBoneBindPoss);
	m_BLoader.Release();
}

CBY_TestBoneObj::CBY_TestBoneObj()
{
}


CBY_TestBoneObj::~CBY_TestBoneObj()
{
}
