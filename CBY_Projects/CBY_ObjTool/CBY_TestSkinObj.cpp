#include "stdafx.h"
#include "CBY_TestSkinObj.h"

bool	CBY_TestSkinObj::Load(T_STR loadfile, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
	m_obj.m_pd3dDevice = pd3dDevice;
	m_obj.m_pContext = pContext;


	if (!m_BLoader.SkinBinaryLoad(loadfile.c_str()))
	{
		return false;
	}

	ConvertSkin(m_VertexDataSkin);
	SkinCreate();
	return true;
}

void    CBY_TestSkinObj::ConvertSkin(std::vector<PNCT2IW_VERTEX>& list)
{
	int iBaseVertex = 0;
	int iBaseIndex = 0;
	int iBaseOffset = 0;

	for (int iObj = 0; iObj < m_BLoader.m_ObjList.size(); iObj++)
	{
		CBY_MeshSkin* mesh = m_BLoader.m_ObjList[iObj];
		mesh->m_iParentIndex = m_BLoader.GetParentIndex(mesh->m_szParentName);

		D3DXMatrixInverse(&mesh->m_matInverse, NULL, &mesh->m_matWorld);

		D3DXMATRIX matInvParent, matAnim;
		D3DXMatrixIdentity(&matInvParent);
		if (mesh->m_iParentIndex >= 0)
		{
			mesh->m_Parent = m_BLoader.m_ObjList[mesh->m_iParentIndex];
			m_BLoader.m_ObjList[mesh->m_iParentIndex]->m_Child.push_back(mesh);
			D3DXMatrixInverse(&matInvParent, NULL,
				&m_BLoader.m_ObjList[mesh->m_iParentIndex]->m_matWorld);
		}
		matAnim = mesh->m_matWorld * matInvParent;
		D3DXVECTOR3 vPos, vScale;
		D3DXQUATERNION qR;
		D3DXMatrixDecompose(&vScale, &qR, &vPos, &matAnim);
		mesh->m_qAnimRotation = qR;
		mesh->m_vAnimPos = vPos;
		mesh->m_vAnimScale = vScale;
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

					mesh->subMeshSkin[iSubMesh].m_pTexture = nullptr;
					mesh->subMeshSkin[iSubMesh].m_pNormalTexture = nullptr;
					mesh->subMeshSkin[iSubMesh].m_pSpecTexture = nullptr;
					mesh->subMeshSkin[iSubMesh].m_pEmsTexture = nullptr;

					for (int i = 0; i < m_BLoader.m_MtlList[iRef].SubMaterial[iSubMesh].texList.size(); i++)
					{
						mesh->subMeshSkin[iSubMesh].m_iTexIndex =
							I_Texture.Add(m_obj.m_pd3dDevice,
								m_BLoader.m_MtlList[iRef].SubMaterial[iSubMesh].texList[i].szTextureName,
								L"../../data/obj/");

						switch (mesh->subMeshSkin[iSubMesh].m_iTexsize)
						{
						case 0:
						{
							mesh->subMeshSkin[iSubMesh].m_pTexture =
								I_Texture.GetPtr(mesh->subMeshSkin[iSubMesh].m_iTexIndex);
						}
						break;

						case 1:
						{
							mesh->subMeshSkin[iSubMesh].m_pNormalTexture =
								I_Texture.GetPtr(mesh->subMeshSkin[iSubMesh].m_iTexIndex);
						}
						break;

						case 2:
						{
							mesh->subMeshSkin[iSubMesh].m_pSpecTexture =
								I_Texture.GetPtr(mesh->subMeshSkin[iSubMesh].m_iTexIndex);
						}
						break;

						case 3:
						{
							mesh->subMeshSkin[iSubMesh].m_pEmsTexture =
								I_Texture.GetPtr(mesh->subMeshSkin[iSubMesh].m_iTexIndex);
						}
						break;

						case 4:
						{
							mesh->subMeshSkin[iSubMesh].m_pDetailTexture =
								I_Texture.GetPtr(mesh->subMeshSkin[iSubMesh].m_iTexIndex);
						}
						break;
						}
						mesh->subMeshSkin[iSubMesh].m_iTexsize++;


					}
				}

				//mesh->subMeshSkin[iSubMesh].m_iNumVertex =mesh->subMeshSkin[iSubMesh].listSkin.size();

				//CreateVIData(&mesh->subMeshSkin[iSubMesh]);
				//VertexDataTan(&mesh->subMeshSkin[iSubMesh]);

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
				//mesh->subMeshSkin[iSubMesh].m_iTexsize = 0;
			}
		}
		else
		{
			if (iRef >= 0)
			{
				mesh->m_pTexture = nullptr;
				mesh->m_pNormalTexture = nullptr;
				mesh->m_pSpecTexture = nullptr;
				mesh->m_pEmsTexture = nullptr;

				for (int i = 0; i < m_BLoader.m_MtlList[iRef].texList.size(); i++)
				{
					mesh->m_iTexIndex =
						I_Texture.Add(m_obj.m_pd3dDevice,
							m_BLoader.m_MtlList[iRef].texList[i].szTextureName,
							L"../../data/obj/");

					switch (mesh->m_iTexsize)
					{
					case 0:
					{
						mesh->m_pTexture = I_Texture.GetPtr(mesh->m_iTexIndex);
					}
					break;

					case 1:
					{
						mesh->m_pNormalTexture = I_Texture.GetPtr(mesh->m_iTexIndex);
					}
					break;

					case 2:
					{
						mesh->m_pSpecTexture = I_Texture.GetPtr(mesh->m_iTexIndex);
					}
					break;

					case 3:
					{
						mesh->m_pEmsTexture = I_Texture.GetPtr(mesh->m_iTexIndex);
					}
					break;

					case 4:
					{
						mesh->m_pDetailTexture = I_Texture.GetPtr(mesh->m_iTexIndex);
					}
					break;
					}

					mesh->m_iTexsize++;
				}
			}

			//CreateVIData(mesh);
			//VertexDataTan(mesh);

			mesh->m_iBaseVertex = iBaseVertex;
			mesh->m_iBaseIndex = iBaseIndex;

			mesh->m_iNumVertex = mesh->vblistSkin.size();
			mesh->m_iNumIndex = mesh->iblistSkin.size();

			iBaseVertex += mesh->m_iNumVertex;
			iBaseIndex += mesh->m_iNumIndex;


			std::copy(mesh->vblistSkin.begin(), mesh->vblistSkin.end(), back_inserter(list));
			std::copy(mesh->iblistSkin.begin(), mesh->iblistSkin.end(), back_inserter(m_IndexData));
		}
	}
	m_ObjList.swap(m_BLoader.m_ObjList);
	m_matBoneBindPoss.swap(m_BLoader.m_matBoneBindPoss);
	m_BLoader.Release();
}

CBY_TestSkinObj::CBY_TestSkinObj()
{
}


CBY_TestSkinObj::~CBY_TestSkinObj()
{
}
