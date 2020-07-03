#include "C_CBYObj.h"

bool C_CBYObj::Load(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	if (!m_Loader.Load(pszLoad.c_str()))
	{
		return false;
	}
	m_obj.m_pd3dDevice = pd3dDevice;
	m_obj.m_pContext = Context;

	int iObjType = -1;
	bool bBreak = false;
	while (!bBreak && (iObjType = m_Loader.SearchTokenArray(CBYaseSections)) >= 0)
	{
		switch (iObjType)
		{
		case CBYSCENE:
			m_Loader.LoadScene();
			break;

		case CBYMATERIAL:
			m_Loader.LoadMaterial();
			break;

		case CBYGEOMOBJECT:
			bBreak = true;
			break;
		}
	}
	m_Loader.LoadGeomobject();

	Convert(m_VerTex);
	Create(pd3dDevice, Context, L"../../data/shader/ColorBlend.txt", L"../../data/obj/wik00.dds", "VSmat", "PSASEBland");
	return true;
}

void C_CBYObj::Convert(std::vector<PNCT_VERTEX>& list)
{
	int iBaseOffsetVertex = 0;
	int iBaseOffsetIndex = 0;

	m_Scene = m_Loader.m_Scene;

	m_ObjectList.resize(m_Loader.m_Mesh.size());

	for (int i = 0; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i] = m_Loader.m_Mesh[i];
		m_ObjectList[i].m_iParentIndex = m_Loader.GetParentIndex(m_ObjectList[i].m_szParentName);

		D3DXMATRIX matInvParent, matAnim, mataa;
		D3DXMatrixIdentity(&matInvParent);
		D3DXMatrixIdentity(&mataa);
		if (m_ObjectList[i].m_iParentIndex >= 0)
		{
			mataa = m_ObjectList[m_ObjectList[i].m_iParentIndex].m_matWorld;
			D3DXMatrixInverse(&matInvParent, NULL, &m_ObjectList[m_ObjectList[i].m_iParentIndex].m_matWorld);
		}
		matAnim = m_ObjectList[i].m_matWorld * matInvParent;
		D3DXMatrixDecompose(&m_ObjectList[i].m_vAnimScale, &m_ObjectList[i].m_qAnimRotation, &m_ObjectList[i].m_vAnimPos, &matAnim);
		D3DXQuaternionIdentity(&m_ObjectList[i].m_qAnimScaleRotation);

		int iRef = m_ObjectList[i].m_iTexIndex;
		if (iRef >= 0)
		{
			if (m_Loader.m_MtrlList[iRef].SubMaterial.size() > 0)
			{
				int iSubMtrl = m_Loader.m_MtrlList[iRef].SubMaterial.size();
				for (int iSub = 0; iSub < iSubMtrl; iSub++)
				{
					m_ObjectList[i].m_subMesh[iSub].m_iTexIndex =
						I_Texture.Add(m_obj.m_pd3dDevice, mtw(m_Loader.m_MtrlList[iRef].SubMaterial[iSub].szTextureName).c_str(), L"../../data/obj/");
					m_ObjectList[i].m_subMesh[iSub].m_pTexture = I_Texture.GetPtr(m_ObjectList[i].m_subMesh[iSub].m_iTexIndex);
				}
			}
			else
			{
				m_ObjectList[i].m_iTexIndex = I_Texture.Add(m_obj.m_pd3dDevice, mtw(m_Loader.m_MtrlList[iRef].szTextureName).c_str(), L"../../data/obj/");

				m_ObjectList[i].m_pTexture = I_Texture.GetPtr(m_ObjectList[i].m_iTexIndex);
			}
		}

		if (m_ObjectList[i].m_subMesh.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh < m_ObjectList[i].m_subMesh.size(); iSubMesh++)
			{
				CreateVIData(m_ObjectList[i].m_subMesh[iSubMesh]);
				m_ObjectList[i].m_subMesh[iSubMesh].m_iNumVertex = m_ObjectList[i].m_subMesh[iSubMesh].vblist.size();
				m_ObjectList[i].m_subMesh[iSubMesh].m_iNumIndex = m_ObjectList[i].m_subMesh[iSubMesh].iblist.size();
				m_ObjectList[i].m_subMesh[iSubMesh].m_iBaseVertex += iBaseOffsetVertex;
				m_ObjectList[i].m_subMesh[iSubMesh].m_iBaseIndex += iBaseOffsetIndex;
				iBaseOffsetVertex += m_ObjectList[i].m_subMesh[iSubMesh].vblist.size();
				iBaseOffsetIndex += m_ObjectList[i].m_subMesh[iSubMesh].iblist.size();
				std::copy(m_ObjectList[i].m_subMesh[iSubMesh].vblist.begin(), m_ObjectList[i].m_subMesh[iSubMesh].vblist.end(), back_inserter(list));
				std::copy(m_ObjectList[i].m_subMesh[iSubMesh].iblist.begin(), m_ObjectList[i].m_subMesh[iSubMesh].iblist.end(), back_inserter(m_IndexData));
			}
		}
		else
		{
			CreateVIData(m_ObjectList[i]);
			m_ObjectList[i].m_iNumVertex = m_ObjectList[i].vblist.size();
			m_ObjectList[i].m_iNumIndex = m_ObjectList[i].iblist.size();
			m_ObjectList[i].m_iBaseVertex += iBaseOffsetVertex;
			m_ObjectList[i].m_iBaseIndex += iBaseOffsetIndex;
			iBaseOffsetVertex += m_ObjectList[i].vblist.size();
			iBaseOffsetIndex += m_ObjectList[i].iblist.size();

			std::copy(m_ObjectList[i].vblist.begin(), m_ObjectList[i].vblist.end(), back_inserter(list));
			std::copy(m_ObjectList[i].iblist.begin(), m_ObjectList[i].iblist.end(), back_inserter(m_IndexData));
		}
	}

	m_Loader.Release();
}

void C_CBYObj::CreateVIData(CMesh& mesh)
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

HRESULT C_CBYObj::CreateVertexData()
{

	HRESULT hr = S_OK;
	m_obj.m_VertexSize = sizeof(PNCT_VERTEX);
	return hr;
}


HRESULT C_CBYObj::CreateIndexData()
{
	HRESULT hr = S_OK;
	return hr;
}

HRESULT C_CBYObj::CreateConstantBuffer()
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

	m_obj.m_pConstantBuffer.Attach(CDXH::CreateConstantBuffer(m_obj.m_pd3dDevice, nullptr, 1, sizeof(CONDATA)));

	return hr;
}

void C_CBYObj::Draw(CMesh& mesh)
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

bool    C_CBYObj::Render()
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


bool C_CBYObj::AniTrackSet(CMatSetData& matdata, CAnimationTrack start, int iobj, std::vector<CAnimationTrack>& AniTrack, int tracktype)
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

bool C_CBYObj::Frame()
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

	return true;
}

bool C_CBYObj::Release()
{
	C_Model::Release();
	return true;
}


C_CBYObj::C_CBYObj()
{
	m_fElapseTick = 0;
}


C_CBYObj::~C_CBYObj()
{
}

