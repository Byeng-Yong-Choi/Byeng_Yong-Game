#include "C_ASEObj.h"

bool C_ASEObj::Load(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	if(!m_Loader.Load(pszLoad.c_str()))
	{
		return false;
	}
	m_obj.m_pd3dDevice = pd3dDevice;
	m_obj.m_pContext = Context;
	
	int iObjType = -1;
	bool bBreak = false;
	while (!bBreak && (iObjType=m_Loader.SearchTokenArray(aseSections)) >= 0)
	{
		switch (iObjType)
		{
		case SCENE:
			m_Loader.LoadScene();
			break;

		case MATERIAL:
			m_Loader.LoadMaterial();
			break;

		case GEOMOBJECT:
		case HELPEROBJECT:
			bBreak = true;
			break;
		}
	}
	m_Loader.LoadGeomobject();

	Convert(m_FaceVertexData);
	Create(pd3dDevice, Context, L"../../data/shader/ColorBlend.txt", L"../../data/obj/wik00.dds", "VSmat", "PSASEBland");
	return true;
}

void C_ASEObj::MeshmatWorld(D3DXMATRIX& meshworld, D3DXVECTOR3* pos)
{
	meshworld._11 = pos[0].x;
	meshworld._12 = pos[0].y;
	meshworld._13 = pos[0].z;
	meshworld._21 = pos[1].x;
	meshworld._22 = pos[1].y;
	meshworld._23 = pos[1].z;
	meshworld._31 = pos[2].x;
	meshworld._32 = pos[2].y;
	meshworld._33 = pos[2].z;
	meshworld._41 = pos[3].x;
	meshworld._42 = pos[3].y;
	meshworld._43 = pos[3].z;
}

void C_ASEObj::SetVertexData(CMesh& mesh, int obj, int face)
{
	D3DXMATRIX matInverse;
	D3DXMatrixInverse(&matInverse, NULL, &mesh.m_matWorld);
	PNCT_VERTEX v;
	
	int index;
	int iSubMtrl = m_Loader.m_ObjectList[obj].PosFaceList[face].subMtrl;

	for (int num = 0; num < (sizeof(m_Loader.m_ObjectList[obj].PosFaceList[face].i) / sizeof(UINT)); num++)
	{
		// p, n, c, t
		index = m_Loader.m_ObjectList[obj].PosFaceList[face].i[num];
		v.p = m_Loader.m_ObjectList[obj].PosVertexList[index];
		D3DXVec3TransformCoord(&v.p, &v.p, &matInverse);

		v.n = m_Loader.m_ObjectList[obj].NormalVertexList[face].p[num];
		v.c = D3DXVECTOR4(1, 1, 1, 1);
		if (m_Loader.m_ObjectList[obj].ColorVertexList.size() > 0)
		{
			index = m_Loader.m_ObjectList[obj].ColorFaceList[face].i[num];
			v.c.x = m_Loader.m_ObjectList[obj].ColorVertexList[index].x;
			v.c.y = m_Loader.m_ObjectList[obj].ColorVertexList[index].y;
			v.c.z = m_Loader.m_ObjectList[obj].ColorVertexList[index].z;
			v.c.w = 1.0f;
		}
		index = m_Loader.m_ObjectList[obj].UVFaceList[face].i[num];
		v.t = m_Loader.m_ObjectList[obj].UVVertexList[index];
		if (mesh.m_subMesh.size() > 0)
		{
			if (iSubMtrl > mesh.m_subMesh.size())
			{
				iSubMtrl = 0;
			}
			mesh.m_subMesh[iSubMtrl].vblist.push_back(v);
		}
		else
		{
			mesh.vblist.push_back(v);
		}
	}

}

void C_ASEObj::Convert(std::vector<PNCT_VERTEX>& list)
{
	int iBaseOffsetVertex = 0;
	int iBaseOffsetIndex = 0;

	m_Scene = m_Loader.m_Scene;
	m_Scene.iNumMesh = m_Loader.m_ObjectList.size();
	m_Scene.iNumMaterial = m_Loader.m_MtrlList.size();

	for (int iObj = 0; iObj < m_Loader.m_ObjectList.size(); iObj++)
	{
		CMesh mesh;
		mesh.m_szName = m_Loader.m_ObjectList[iObj].szName;
		mesh.m_szParentName = m_Loader.m_ObjectList[iObj].szParent;
		mesh.m_iParentIndex = m_Loader.GetParentIndex(mesh.m_szParentName);

		D3DXMatrixIdentity(&mesh.m_matWorld);
		MeshmatWorld(mesh.m_matWorld, m_Loader.m_ObjectList[iObj].v);
		

		D3DXMATRIX matInvParent, matAnim;
		D3DXMatrixIdentity(&matInvParent);
		if (mesh.m_iParentIndex >= 0)
		{
			D3DXMatrixInverse(&matInvParent, NULL, &m_ObjectList[mesh.m_iParentIndex].m_matWorld);
		}
		matAnim = mesh.m_matWorld * matInvParent;
		D3DXMatrixDecompose(&mesh.m_vAnimScale, &mesh.m_qAnimRotation, &mesh.m_vAnimPos, &matAnim);
		D3DXQuaternionIdentity(&mesh.m_qAnimScaleRotation);

		mesh.posTrack.assign(m_Loader.m_ObjectList[iObj].AniTrack_Pos.begin(), m_Loader.m_ObjectList[iObj].AniTrack_Pos.end());
		mesh.rotTrack.assign(m_Loader.m_ObjectList[iObj].AniTrack_Rotate.begin(), m_Loader.m_ObjectList[iObj].AniTrack_Rotate.end());
		mesh.sclTrack.assign(m_Loader.m_ObjectList[iObj].AniTrack_Scale.begin(), m_Loader.m_ObjectList[iObj].AniTrack_Scale.end());

		int iRef = m_Loader.m_ObjectList[iObj].m_iRef;
		if (iRef >= 0)
		{
			if (m_Loader.m_MtrlList[iRef].SubMaterial.size() > 0)
			{
				int iSubMtrl = m_Loader.m_MtrlList[iRef].SubMaterial.size();
				mesh.m_subMesh.resize(iSubMtrl);
				for (int iSub = 0; iSub < iSubMtrl; iSub++)
				{
					mesh.m_subMesh[iSub].m_iTexIndex =
						I_Texture.Add(m_obj.m_pd3dDevice, mtw(m_Loader.m_MtrlList[iRef].SubMaterial[iSub].szTextureName).c_str(), L"../../data/obj/");
					mesh.m_subMesh[iSub].m_pTexture = I_Texture.GetPtr(mesh.m_subMesh[iSub].m_iTexIndex);
				}
			}
			else
			{
				mesh.m_iTexIndex = I_Texture.Add(m_obj.m_pd3dDevice, mtw(m_Loader.m_MtrlList[iRef].szTextureName).c_str(), L"../../data/obj/");

				mesh.m_pTexture = I_Texture.GetPtr(mesh.m_iTexIndex);
			}
		}

		for (int iFace = 0; iFace < m_Loader.m_ObjectList[iObj].iNumFace; iFace++)
		{
			SetVertexData(mesh, iObj, iFace);
		}

		if (mesh.m_subMesh.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh < mesh.m_subMesh.size(); iSubMesh++)
			{
				CreateVIData(mesh.m_subMesh[iSubMesh]);
				mesh.m_subMesh[iSubMesh].m_iNumVertex = mesh.m_subMesh[iSubMesh].vblist.size();
				mesh.m_subMesh[iSubMesh].m_iNumIndex = mesh.m_subMesh[iSubMesh].iblist.size();
				mesh.m_subMesh[iSubMesh].m_iBaseVertex += iBaseOffsetVertex;
				mesh.m_subMesh[iSubMesh].m_iBaseIndex += iBaseOffsetIndex;
				iBaseOffsetVertex += mesh.m_subMesh[iSubMesh].vblist.size();
				iBaseOffsetIndex += mesh.m_subMesh[iSubMesh].iblist.size();
				std::copy(mesh.m_subMesh[iSubMesh].vblist.begin(), mesh.m_subMesh[iSubMesh].vblist.end(), back_inserter(list));
				std::copy(mesh.m_subMesh[iSubMesh].iblist.begin(), mesh.m_subMesh[iSubMesh].iblist.end(), back_inserter(m_FaceIndexData));
			}
		}
		else
		{
			CreateVIData(mesh);
			mesh.m_iNumVertex = mesh.vblist.size();
			mesh.m_iNumIndex = mesh.iblist.size();
			mesh.m_iBaseVertex += iBaseOffsetVertex;
			mesh.m_iBaseIndex += iBaseOffsetIndex;
			iBaseOffsetVertex += mesh.vblist.size();
			iBaseOffsetIndex += mesh.iblist.size();

			std::copy(mesh.vblist.begin(), mesh.vblist.end(), back_inserter(list));
			std::copy(mesh.iblist.begin(), mesh.iblist.end(), back_inserter(m_FaceIndexData));
		}

		m_ObjectList.push_back(mesh);
	}
	m_Loader.Release();
}

void C_ASEObj::CreateVIData(CMesh& mesh)
{
	if (mesh.m_vtlist.size() <= 0)
	{
		return;
	}
	mesh.vblist.reserve(mesh.m_vtlist.size());
	for (int iVertex = 0; iVertex < mesh.m_vtlist.size(); iVertex++)
	{
		int iPos = -1;
		for (int i = 0; i < mesh.vblist.size(); i++)
		{
			if (mesh.vblist[i] == mesh.m_vtlist[iVertex])
			{
				iPos = i;
				break;
			}
		}
		if (iPos < 0)
		{
			mesh.vblist.push_back(mesh.m_vtlist[iVertex]);
			iPos = mesh.vblist.size() - 1;
		}
		mesh.iblist.push_back(iPos);
	}
}

HRESULT C_ASEObj::CreateVertexData()
{

	HRESULT hr = S_OK;
	m_obj.m_VertexSize = sizeof(PNCT_VERTEX);
	m_VerTex.resize(m_FaceVertexData.size());
	for (int iVertex = 0; iVertex < m_FaceVertexData.size(); iVertex++)
	{
		m_VerTex[iVertex] = m_FaceVertexData[iVertex];
	}
	return hr;
}


HRESULT C_ASEObj::CreateIndexData()
{
	HRESULT hr = S_OK;
	m_IndexData.resize(m_FaceIndexData.size());
	for (int iIndex = 0; iIndex < m_IndexData.size(); iIndex++)
	{
		m_IndexData[iIndex] = m_FaceIndexData[iIndex];
	}
	
	return hr;
}

HRESULT C_ASEObj::CreateConstantBuffer()
{
	HRESULT hr = S_OK;

	ZeroMemory(&m_ConData, sizeof(CONDATA));		//상수버퍼에 들어갈 데이터 정보 초기화
	D3DXMatrixIdentity(&m_ConData.World);
	D3DXMatrixIdentity(&m_ConData.View);
	D3DXMatrixIdentity(&m_ConData.Proj);
	m_ConData.ddata = D3DXVECTOR4(1, 1, 1, 1);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(CONDATA);								//버퍼의 크기
	bd.Usage = D3D11_USAGE_DYNAMIC;								//버퍼의 읽기 쓰기 방법, Usage를 DYNAMIC으로 설정을 해야 cpu가 접근 가능하다
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;					//cpu가 접근하여 쓸수 있다.
	m_obj.iCBBufferType = CB_DEFAULT;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = &m_ConData;							//초기화 데이터에 대한 포인터

	m_obj.m_pConstantBuffer.Attach(CDXH::CreateConstantBuffer(m_obj.m_pd3dDevice,nullptr, 1,sizeof(CONDATA)));
	
	return hr;
}

void C_ASEObj::Draw(CMesh& mesh)
{
	if (mesh.m_pTexture != nullptr)
	{
		m_obj.m_pContext->PSSetShaderResources(0, 1, &mesh.m_pTexture->m_pTextureRV);
	}
	if (mesh.m_iNumIndex > 0)
	{
		m_obj.m_pContext->DrawIndexed(mesh.m_iNumIndex, mesh.m_iBaseIndex, mesh.m_iBaseVertex);
	}
	else
	{
		m_obj.m_pContext->Draw(mesh.m_iNumVertex, mesh.m_iBaseVertex);
	}
}

bool    C_ASEObj::Render()
{
	m_obj.PrePender();
	for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
	{
		m_ObjectList[iObj].m_cbData = m_ConData;
		m_ObjectList[iObj].m_matCalculation *= m_matWorld;
		D3DXMatrixTranspose(&m_ObjectList[iObj].m_cbData.World, &m_ObjectList[iObj].m_matCalculation);
		m_obj.m_pContext->UpdateSubresource(m_obj.m_pConstantBuffer.Get(), 0, NULL, &m_ObjectList[iObj].m_cbData, 0, 0);

		if (m_ObjectList[iObj].m_subMesh.size() > 0)
		{
			for (int iSub = 0; iSub < m_ObjectList[iObj].m_subMesh.size(); iSub++)
			{
				Draw(m_ObjectList[iObj].m_subMesh[iSub]);
			}
		}
		else
		{
			Draw(m_ObjectList[iObj]);
		}
	}
	return true;
}


bool C_ASEObj::AniTrackSet(CMatSetData& matdata, CAnimationTrack start, int iobj,std::vector<CAnimationTrack>& AniTrack, int tracktype)
{
	CAnimationTrack aniStart = start;
	CAnimationTrack aniEnd;
	for (int tick = 0; tick < AniTrack.size(); tick++)
	{
		if (AniTrack[tick].iTick <= m_fElapseTick)
		{
			aniStart = AniTrack[tick];
		}
		if (AniTrack[tick].iTick > m_fElapseTick)
		{
			aniEnd = AniTrack[tick];
			break;
		}
		if (tick == AniTrack.size() - 1)
		{
			aniEnd = start;
		}
	}
	float step = aniEnd.iTick - aniStart.iTick;
	float t = 0.0f;
	if (step > 0.0f)
	{
		t = (m_fElapseTick - aniStart.iTick) / step;
	}
	switch (tracktype)
	{
	case ANI_POS:
	{
		D3DXVec3Lerp(&matdata.vPos, &aniStart.p, &aniEnd.p, t);
	}
	break;

	case ANI_ROT:
	{
		D3DXQuaternionSlerp(&matdata.qR, &aniStart.q, &aniEnd.q, t);
		D3DXMatrixRotationQuaternion(&matdata.matRotation, &matdata.qR);
	}
	break;

	case ANI_SCL:
	{
		D3DXQuaternionSlerp(&matdata.qScale, &aniStart.q, &aniEnd.q, t);
		D3DXMatrixRotationQuaternion(&matdata.matScaleRot, &matdata.qScale);
		D3DXMatrixInverse(&matdata.matScaleInverRot, NULL, &matdata.matScaleRot);

		D3DXVec3Lerp(&matdata.vScale, &aniStart.p, &aniEnd.p, t);
		D3DXMatrixScaling(&matdata.matScale, matdata.vScale.x, matdata.vScale.y, matdata.vScale.z);
		matdata.matScale = matdata.matScaleInverRot * matdata.matScale * matdata.matScaleRot;
	}
		break;
	}
	return true;
}

bool C_ASEObj::Frame()
{
	float fTime = 0;
	fTime = 1.0f*g_SecondTime*m_Scene.iTickPerFrame*m_Scene.iFrameSpeed;

	if (fTime >= m_Scene.iTickPerFrame)
	{
		fTime = m_Scene.iTickPerFrame;
	}

	m_fElapseTick += fTime;

	if (m_fElapseTick >= m_Scene.iLastFrame * m_Scene.iTickPerFrame)
	{
		m_fElapseTick = 0.0f;
	}

	CMatSetData matdata;
	
	for (int i = 0; i < m_ObjectList.size(); i++)
	{
		CMatSetData matdata(m_ObjectList[i].m_vAnimPos, m_ObjectList[i].m_vAnimScale, m_ObjectList[i].m_qAnimRotation, m_ObjectList[i].m_qAnimScaleRotation);
		CAnimationTrack start;
		start.iTick = 0;
		start.p = matdata.vPos;
		start.q = matdata.qR;


		if (m_ObjectList[i].posTrack.size() > 0)
		{
			AniTrackSet(matdata, start, i, m_ObjectList[i].posTrack, ANI_POS);
		}

		if (m_ObjectList[i].rotTrack.size() > 0)
		{
			AniTrackSet(matdata, start, i, m_ObjectList[i].rotTrack, ANI_ROT);
		}

		if (m_ObjectList[i].sclTrack.size() > 0)
		{
			AniTrackSet(matdata, start, i, m_ObjectList[i].sclTrack, ANI_SCL);
		}
		m_ObjectList[i].m_matCalculation = matdata.matScale * matdata.matRotation;
		m_ObjectList[i].m_matCalculation._41 = matdata.vPos.x;
		m_ObjectList[i].m_matCalculation._42 = matdata.vPos.y;
		m_ObjectList[i].m_matCalculation._43 = matdata.vPos.z;

		if (m_ObjectList[i].m_iParentIndex >= 0)
		{
			D3DXMATRIX matParent = m_ObjectList[m_ObjectList[i].m_iParentIndex].m_matCalculation;
			m_ObjectList[i].m_matCalculation *= matParent;
		}
	}

	//for (int i = 0; i < m_ObjectList.size(); i++)
	//{
	//	//m_ObjectList[i].m_matCalculation = m_ObjectList[i].m_matWorld;
	//	D3DXMatrixIdentity(&m_ObjectList[i].m_matCalculation);
	//}

	return true;
}

bool C_ASEObj::Release()
{
	C_Model::Release();
	return true;
}


C_ASEObj::C_ASEObj()
{
	m_fElapseTick = 0;
}


C_ASEObj::~C_ASEObj()
{
}

