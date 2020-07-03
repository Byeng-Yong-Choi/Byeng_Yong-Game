#include "C_CBYObj_1.h"

HRESULT C_CBYObj_1::CreateInputLayout()
{
	HRESULT hr = S_OK;
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION_OBJ",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 64,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iElementCount = sizeof(layout) /
		sizeof(layout[0]);
	m_obj.m_pd3dDevice->CreateInputLayout(
		layout,
		iElementCount,
		m_obj.m_pVertexCode->GetBufferPointer(),
		m_obj.m_pVertexCode->GetBufferSize(),
		&m_obj.m_pVertexLayout);
	return hr;
}

HRESULT C_CBYObj_1::CreateVertexBuffer()
{
	HRESULT hr = S_OK;
	if (m_VertexDataSkin.size() <= 0) return E_FAIL;
	m_obj.m_Vertexnum = m_VertexDataSkin.size();
	D3D11_BUFFER_DESC pDesc;
	ZeroMemory(&pDesc, sizeof(D3D11_BUFFER_DESC));
	pDesc.Usage = D3D11_USAGE_DEFAULT;
	pDesc.ByteWidth = m_obj.m_VertexSize * m_VertexDataSkin.size();
	pDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA pInitialData;
	ZeroMemory(&pInitialData,
		sizeof(D3D11_SUBRESOURCE_DATA));
	pInitialData.pSysMem = &m_VertexDataSkin.at(0);

	hr = m_obj.m_pd3dDevice->CreateBuffer(&pDesc, &pInitialData, &m_obj.m_pVertexBuffer);
	return hr;
}

HRESULT C_CBYObj_1::CreateConstantBuffer()
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
void    C_CBYObj_1::Convert(std::vector<PNCTIW_VERTEX>& list)
{
	int iBaseVertex = 0;
	int iBaseIndex = 0;
	int iBaseOffset = 0;

	for (int iObj = 0; iObj < m_Loader.m_ObjList.size(); iObj++)
	{
		CMeshSkin& mesh = m_Loader.m_ObjList[iObj];
		mesh.m_iParentIndex =
			m_Loader.GetParentIndex(mesh.m_szParentName);

		D3DXMATRIX matInvParent, matAnim;
		D3DXMatrixIdentity(&matInvParent);
		if (mesh.m_iParentIndex >= 0)
		{
			D3DXMatrixInverse(&matInvParent, NULL, &m_Loader.m_ObjList[mesh.m_iParentIndex].m_matWorld);
		}
		matAnim = mesh.m_matWorld * matInvParent;
		D3DXMatrixDecompose(&mesh.m_vAnimScale, &mesh.m_qAnimRotation, &mesh.m_vAnimPos, &matAnim);
		D3DXQuaternionIdentity(&mesh.m_qAnimScaleRotation);


		int iRef = mesh.m_iTexIndex;
		if (iRef >= 0)
		{
			mesh.subMeshSkin.resize(
				m_Loader.m_MtlList[iRef].SubMaterial.size());
		}


		if (mesh.subMeshSkin.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh < mesh.subMeshSkin.size(); iSubMesh++)
			{
				if (iRef >= 0)
				{
					if (m_Loader.m_MtlList[iRef].SubMaterial[iSubMesh].texList.size() > 0)
					{
						mesh.subMeshSkin[iSubMesh].m_iTexIndex =
							I_Texture.Add(m_obj.m_pd3dDevice,
								m_Loader.m_MtlList[iRef].SubMaterial[iSubMesh].texList[0].szTextureName,
								L"../../data/obj/");
						mesh.subMeshSkin[iSubMesh].m_pTexture =
							I_Texture.GetPtr(mesh.subMeshSkin[iSubMesh].m_iTexIndex);
					}
				}

				mesh.subMeshSkin[iSubMesh].m_iNumVertex =
					mesh.subMeshSkin[iSubMesh].listSkin.size();

				CreateVIData(mesh.subMeshSkin[iSubMesh]);

				mesh.subMeshSkin[iSubMesh].m_iBaseIndex = iBaseIndex;
				mesh.subMeshSkin[iSubMesh].m_iBaseVertex = iBaseVertex;

				mesh.subMeshSkin[iSubMesh].m_iNumVertex = mesh.subMeshSkin[iSubMesh].vblistSkin.size();
				mesh.subMeshSkin[iSubMesh].m_iNumIndex = mesh.subMeshSkin[iSubMesh].iblistSkin.size();

				iBaseVertex += mesh.subMeshSkin[iSubMesh].m_iNumVertex;
				iBaseIndex += mesh.subMeshSkin[iSubMesh].m_iNumIndex;


				std::copy(mesh.subMeshSkin[iSubMesh].vblistSkin.begin(),
					mesh.subMeshSkin[iSubMesh].vblistSkin.end(),
					back_inserter(list));

				std::copy(mesh.subMeshSkin[iSubMesh].iblistSkin.begin(),
					mesh.subMeshSkin[iSubMesh].iblistSkin.end(),
					back_inserter(m_IndexData));
			}
		}
		else
		{
			if (iRef >= 0)
			{
				mesh.m_iTexIndex =
					I_Texture.Add(m_obj.m_pd3dDevice,
						m_Loader.m_MtlList[iRef].texList[0].szTextureName,
						L"../../data/obj/");
				mesh.m_pTexture = I_Texture.GetPtr(mesh.m_iTexIndex);
			}

			CreateVIData(mesh);


			mesh.m_iBaseVertex = iBaseVertex;
			mesh.m_iBaseIndex = iBaseIndex;

			mesh.m_iNumVertex = mesh.vblistSkin.size();
			mesh.m_iNumIndex = mesh.iblistSkin.size();

			iBaseVertex += mesh.m_iNumVertex;
			iBaseIndex += mesh.m_iNumIndex;


			std::copy(mesh.vblistSkin.begin(), mesh.vblistSkin.end(), back_inserter(list));
			std::copy(mesh.iblistSkin.begin(), mesh.iblistSkin.end(), back_inserter(m_IndexData));
		}
	}
	m_Scene = m_Loader.m_Scene;
}

HRESULT C_CBYObj_1::CreateVertexData()
{
	HRESULT hr = S_OK;
	m_obj.m_VertexSize = sizeof(PNCTIW_VERTEX);

	return hr;
}
HRESULT C_CBYObj_1::CreateIndexData()
{
	HRESULT hr = S_OK;

	return hr;
}
bool	C_CBYObj_1::Load(
	T_STR loadfile,
	ID3D11Device* pd3dDevice,
	ID3D11DeviceContext* pContext)
{
	m_obj.m_pd3dDevice = pd3dDevice;
	m_obj.m_pContext = pContext;
	if (!m_Loader.Load(loadfile.c_str()))
	{
		return false;
	}

	Convert(m_VertexDataSkin);

	Create(pd3dDevice, pContext, L"../../data/shader/SkinShader.txt", nullptr, "VSOBJECT", "PSBone");

	return true;
}

bool    C_CBYObj_1::Frame()
{
	float fTime = 0;
	fTime = 1.0f * g_SecondTime * m_Loader.m_Scene.iTickPerFrame * m_Loader.m_Scene.iFrameSpeed;

	if (fTime >= m_Loader.m_Scene.iTickPerFrame)
	{
		fTime = m_Loader.m_Scene.iTickPerFrame;
	}

	m_fElapseTick += fTime;

	if (m_fElapseTick >= m_Loader.m_Scene.iLastFrame * m_Loader.m_Scene.iTickPerFrame)
	{
		m_fElapseTick = 0.0f;
	}

	CMatSetData matdata;

	for (int iObj = 0; iObj < m_Loader.m_ObjList.size(); iObj++)
	{
		CMatSetData matdata(m_Loader.m_ObjList[iObj].m_vAnimPos, m_Loader.m_ObjList[iObj].m_vAnimScale, 
			m_Loader.m_ObjList[iObj].m_qAnimRotation, m_Loader.m_ObjList[iObj].m_qAnimScaleRotation);
		CAnimationTrack start;
		start.iTick = 0;
		start.p = matdata.vPos;
		start.q = matdata.qR;

		if (m_Loader.m_ObjList[iObj].posTrack.size() > 0)
		{
			AniTrackSet(matdata, start, iObj, m_Loader.m_ObjList[iObj].posTrack, ANI_POS);
		}

		if (m_Loader.m_ObjList[iObj].rotTrack.size() > 0)
		{
			AniTrackSet(matdata, start, iObj, m_Loader.m_ObjList[iObj].rotTrack, ANI_ROT);
		}

		if (m_Loader.m_ObjList[iObj].sclTrack.size() > 0)
		{
			AniTrackSet(matdata, start, iObj, m_Loader.m_ObjList[iObj].sclTrack, ANI_SCL);
		}
		m_Loader.m_ObjList[iObj].m_matCalculation = matdata.matScale * matdata.matRotation;
		m_Loader.m_ObjList[iObj].m_matCalculation._41 = matdata.vPos.x;
		m_Loader.m_ObjList[iObj].m_matCalculation._42 = matdata.vPos.y;
		m_Loader.m_ObjList[iObj].m_matCalculation._43 = matdata.vPos.z;

		if (m_Loader.m_ObjList[iObj].m_iParentIndex >= 0)
		{
			D3DXMATRIX matParent = m_Loader.m_ObjList[m_Loader.m_ObjList[iObj].m_iParentIndex].m_matCalculationBegin;
			m_Loader.m_ObjList[iObj].m_matCalculation *= matParent;
		}

		m_Loader.m_ObjList[iObj].m_matCalculation = m_Loader.m_matBoneBindPoss[iObj] * m_Loader.m_ObjList[iObj].m_matCalculation;
	}

	return true;
}

bool C_CBYObj_1::AniTrackSet(CMatSetData& matdata, CAnimationTrack start, int iobj, std::vector<CAnimationTrack>& AniTrack, int tracktype)
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
void C_CBYObj_1::Draw(CMeshSkin& mesh)
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

bool    C_CBYObj_1::Render()
{
	m_obj.PrePender();
	for (int iObj = 0; iObj < m_Loader.m_ObjList.size(); iObj++)
	{
		m_Loader.m_ObjList[iObj].m_cbData = m_ConData;
		m_Loader.m_ObjList[iObj].m_matCalculation *= m_matWorld;
		D3DXMatrixTranspose(&m_Loader.m_ObjList[iObj].m_cbData.World, &m_Loader.m_ObjList[iObj].m_matCalculation);
		m_obj.m_pContext->UpdateSubresource(m_obj.m_pConstantBuffer.Get(), 0, NULL, &m_Loader.m_ObjList[iObj].m_cbData, 0, 0);

		if (m_Loader.m_ObjList[iObj].subMeshSkin.size() > 0)
		{
			for (int iSub = 0; iSub < m_Loader.m_ObjList[iObj].subMeshSkin.size(); iSub++)
			{
				Draw(m_Loader.m_ObjList[iObj].subMeshSkin[iSub]);
			}
		}
		else
		{
			Draw(m_Loader.m_ObjList[iObj]);
		}
	}
	return true;
}

bool    C_CBYObj_1::Release()
{
	C_Model::Release();
	m_Loader.Release();
	return true;
}
void	C_CBYObj_1::CreateVIData(CMeshSkin& mesh)
{
	HRESULT hr = S_OK;
	mesh.iblistSkin.clear();
	if (mesh.listSkin.size() <= 0)
	{
		return;
	}
	mesh.vblistSkin.reserve(mesh.listSkin.size());
	for (int iVertex = 0; iVertex < mesh.listSkin.size(); iVertex++)
	{
		int iPos = -1;
		for (int i = 0; i < mesh.vblistSkin.size(); i++)
		{
			if (mesh.vblistSkin[i] == mesh.listSkin[iVertex])
			{
				iPos = i;
				break;
			}
		}
		if (iPos < 0)
		{
			mesh.vblistSkin.push_back(mesh.listSkin[iVertex]);
			iPos = mesh.vblistSkin.size() - 1;
		}
		mesh.iblistSkin.push_back(iPos);
	}
}

C_CBYObj_1::C_CBYObj_1()
{
	m_fElapseTick = 0.0f;
}


C_CBYObj_1::~C_CBYObj_1()
{
}
