#include "C_BoneObj.h"

bool C_BoneObj::BoneCreate()
{
	if (FAILED(CreateVertexData()))
	{
		return false;
	}
	if (FAILED(CreateIndexData()))
	{
		return false;
	}
	if (FAILED(CreateVertexBuffer()))
	{
		return false;
	}
	if (FAILED(CreateIndexBuffer()))
	{
		return false;
	}
	return true;
}

bool	C_BoneObj::Load(T_STR loadfile, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
	m_obj.m_pd3dDevice = pd3dDevice;
	m_obj.m_pContext = pContext;
	if (!m_Loader.Load(loadfile.c_str()))
	{
		return false;
	}

	Convert(m_VertexDataSkin);

	//BoneCreate();

	return true;
}

void C_BoneObj::Update(int iStart, int iEnd, float fTime, D3DXMATRIX* pMatrixList)
{
	CMatSetData matdata;

	float afTime = 0;
	afTime = 1.0f*g_SecondTime * m_Loader.m_Scene.iTickPerFrame * m_Loader.m_Scene.iFrameSpeed;
	if (afTime >= m_Loader.m_Scene.iTickPerFrame)
	{
		afTime = m_Loader.m_Scene.iTickPerFrame;
	}

	m_fElapseTick += afTime;
	if (m_fElapseTick >= m_Loader.m_Scene.iLastFrame * m_Loader.m_Scene.iTickPerFrame)
	{
		m_fElapseTick = 0.0f;
	}

	for (int iObj = 0; iObj < m_Loader.m_ObjList.size(); iObj++)
	{
		CMatSetData matdata(m_Loader.m_ObjList[iObj].m_vAnimPos, m_Loader.m_ObjList[iObj].m_vAnimScale,
			m_Loader.m_ObjList[iObj].m_qAnimRotation, m_Loader.m_ObjList[iObj].m_qAnimScaleRotation);
		CAnimationTrack start;
		start.iTick = iStart;
		start.p = matdata.vPos;
		start.q = matdata.qR;

		if (m_Loader.m_ObjList[iObj].posTrack.size() > 0)
		{
			AniTrackSet(matdata, start, iObj, m_Loader.m_ObjList[iObj].posTrack, ANI_POS, m_fElapseTick);
		}

		if (m_Loader.m_ObjList[iObj].rotTrack.size() > 0)
		{
			AniTrackSet(matdata, start, iObj, m_Loader.m_ObjList[iObj].rotTrack, ANI_ROT, m_fElapseTick);
		}

		if (m_Loader.m_ObjList[iObj].sclTrack.size() > 0)
		{
			AniTrackSet(matdata, start, iObj, m_Loader.m_ObjList[iObj].sclTrack, ANI_SCL, m_fElapseTick);
		}
		m_Loader.m_ObjList[iObj].m_matCalculation = matdata.matScale * matdata.matRotation;
		m_Loader.m_ObjList[iObj].m_matCalculation._41 = matdata.vPos.x;
		m_Loader.m_ObjList[iObj].m_matCalculation._42 = matdata.vPos.y;
		m_Loader.m_ObjList[iObj].m_matCalculation._43 = matdata.vPos.z;


		if (m_Loader.m_ObjList[iObj].m_iParentIndex >= 0)
		{
			D3DXMATRIX matParent = m_Loader.m_ObjList[m_Loader.m_ObjList[iObj].m_iParentIndex].m_matCalculation;
			m_Loader.m_ObjList[iObj].m_matCalculation *= matParent;
		}
		

		pMatrixList[iObj] = m_Loader.m_ObjList[iObj].m_matCalculation;

	}
}


bool C_BoneObj::AniTrackSet(CMatSetData& matdata, CAnimationTrack start, int iobj, std::vector<CAnimationTrack>& AniTrack, int tracktype, float fETick)
{
	CAnimationTrack aniStart = start;
	CAnimationTrack aniEnd;
	for (int tick = 0; tick < AniTrack.size(); tick++)
	{
		if (AniTrack[tick].iTick <= fETick)
		{
			aniStart = AniTrack[tick];
		}
		if (AniTrack[tick].iTick > fETick)
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
		t = (fETick - aniStart.iTick) / step;
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

C_BoneObj::C_BoneObj()
{
	m_fElapseTick = 0.0f;
}


C_BoneObj::~C_BoneObj()
{
}
