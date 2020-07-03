#include "C_ASEParser.h"

template <typename OutputIterator>
void C_ASEParser::Tokenize(const C_STR& text, const C_STR& token, OutputIterator ListData)			//토큰, 즉 token로 잘라내서 파일을 읽기위해
{
	size_t start, stop, TotalBytes = text.length();

	for (start = text.find_first_not_of(token); 0 <= start && start < TotalBytes;
		start = text.find_first_not_of(token, stop + 1))
	{
		stop = text.find_first_of(token, start);
		if (stop < 0 || stop > TotalBytes)
		{
			stop = TotalBytes;
		}
		*ListData++ = text.substr(start, stop - start);
	}
}

bool C_ASEParser::BeginToken()		//1차원 데이터 리스토어
{
	C_STR BufferData = m_pmbMeshData;		//버퍼안에 든 데이터
	Tokenize(BufferData, "*", back_inserter(m_ListTokens));
	return true;
}

bool C_ASEParser::Load(const TCHAR* pszfilename)
{
	LARGE_INTEGER FileSize;
	DWORD dwByteRead;

	HANDLE m_hHandle = CreateFile(pszfilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_hHandle != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(m_hHandle, &FileSize);			//모든 파일 사이즈 받아서 파일 사이즈 확인
		m_pmbMeshData = new CHAR[FileSize.QuadPart];	//파일 사이즈만큼 버퍼를 만든다
		if (!ReadFile(m_hHandle, m_pmbMeshData, FileSize.QuadPart, &dwByteRead, NULL))
		{
			delete[] m_pmbMeshData;			//읽지 못할시 전부 해제하는 작업
		}
		BeginToken();
		CloseHandle(m_hHandle);
		delete[] m_pmbMeshData;
	}
	return true;
}

bool C_ASEParser::GetNextLine()		//다음 줄의 데이터를 받아오기
{
	if (m_iTokenindex >= m_ListTokens.size())				//가지고있는 버퍼보다 크면 나간다
	{
		return false;
	}

	m_smbTokenData = m_ListTokens[m_iTokenindex++];
	return true;
}

int C_ASEParser::SearchTokenArray(std::vector<std::string>& asesections)
{
	int iRet = -1;
	while (iRet < 0)
	{
		if (!GetNextLine())
		{
			break;
		}
		//m_smbTokenData = m_ListTokens[m_iTokenindex++];
		for (int iType = 0; iType < asesections.size(); iType++)
		{
			if (strstr(m_smbTokenData.c_str(),asesections[iType].c_str())!=nullptr)		//str안에서 str찾기
			{
				iRet = iType;
				break;
			}
		}
	}
	
	return iRet;
}

const CHAR* C_ASEParser::GetSearchString(const CHAR* pString, BOOL bLoop)
{
	GetNextLine();
	const CHAR* pSerchString = nullptr;
	if (bLoop == FALSE)
	{
		pSerchString = strstr(m_smbTokenData.c_str(), pString);
		if (pSerchString == nullptr)
		{
			--m_iTokenindex;
		}
	}
	else
	{
		pSerchString = strstr(m_smbTokenData.c_str(), pString);
		while (pSerchString == nullptr)
		{
			GetNextLine();
			pSerchString = strstr(m_smbTokenData.c_str(), pString);
			if (pSerchString != nullptr)
			{
				break;
			}
		}
	}

	return pSerchString;
}

bool C_ASEParser::GetValue(VOID* pData, INT dataType)
{
	assert(pData);
	switch (dataType)
	{
		case INT_DATA: {
			sscanf_s(m_smbTokenData.c_str(), "%s%d", m_TempString, _countof(m_TempString), pData);
			return true;
		}
		case STRING_DATA: {
			size_t start, end;
			start = m_smbTokenData.find_first_of("\"") + 1;
			end = m_smbTokenData.find_first_of("\"", start);
			strcpy((CHAR*)pData, m_smbTokenData.substr(start, end - start).c_str());
			return true;
		}
		case FLOAT_DATA:
		{
			sscanf_s(m_smbTokenData.c_str(), "%s%f", m_TempString, _countof(m_TempString),
				pData);
			return true;
		}break;
		case VECTOR_DATA:
		{
			sscanf_s(m_smbTokenData.c_str(), "%s%f%f%f", m_TempString, _countof(m_TempString),
				&((D3DXVECTOR3*)pData)->x,
				&((D3DXVECTOR3*)pData)->z,
				&((D3DXVECTOR3*)pData)->y);
			return true;
		}break;
		case VERTEX_DATA:
		{
			int iIndex;
			sscanf_s(m_smbTokenData.c_str(), "%s%d%f%f%f", m_TempString, _countof(m_TempString),
				&iIndex,
				&((D3DXVECTOR3*)pData)->x,
				&((D3DXVECTOR3*)pData)->z,
				&((D3DXVECTOR3*)pData)->y);
			return true;
		}break;
		case INDEX_DATA:
		{
			int iIndex;
			sscanf_s(m_smbTokenData.c_str(), "%s%d%d%d%d", m_TempString, _countof(m_TempString),
				&iIndex,
				&((CIndex*)pData)->i[0],
				&((CIndex*)pData)->i[2],
				&((CIndex*)pData)->i[1]);
		}break;
	}
	return true;
}

bool C_ASEParser::GetData(const CHAR* pString, VOID* pData, INT dataType, BOOL bLoop)
{
	UINT iStartIndex = m_iTokenindex;
	if (GetSearchString(pString, bLoop) != nullptr)
	{
		if (dataType == NULL_DATA)
		{
			return true;
		}
		return GetValue(pData, dataType);
	}

	m_iTokenindex = iStartIndex;

	return false;
}


void C_ASEParser::LoadScene()
{
	GetData("SCENE_FIRSTFRAME", &m_Scene.iFirstFrame, INT_DATA);
	GetData("SCENE_LASTFRAME", &m_Scene.iLastFrame, INT_DATA);
	GetData("SCENE_FRAMESPEED", &m_Scene.iFrameSpeed, INT_DATA);
	GetData("SCENE_TICKSPERFRAME", &m_Scene.iTickPerFrame, INT_DATA);
}

void C_ASEParser::LoadMaterial()
{
	GetNextLine();
	GetValue(&m_Scene.iNumMaterial, INT_DATA);
	m_MtrlList.resize(m_Scene.iNumMaterial);
	for (int mtr = 0; mtr < m_MtrlList.size(); mtr++)
	{
		GetNextLine();
		LoadMaterialData(mtr,-1);
	}
}

void C_ASEParser::LoadMaterialData(int i,int iSub)
{
	int iDataType = -1;
	while ((iDataType = SearchTokenArray(MaterialSections)) >= 0)
	{
		switch (iDataType)
		{
		case NEWMATERIAL:
		case NEWSUBMATERIAL:
		{
			return;
		}break;

		case GEOM_OBJECT:
		case HELPER_OBJECT:
		{
			m_smbTokenData = m_ListTokens[--m_iTokenindex];
			return;
		}break;

		case SUBNUMCOUNT:
		{
			int iSubMaterial = 0;
			GetValue(&iSubMaterial, INT_DATA);
			m_MtrlList[i].SubMaterial.resize(iSubMaterial);
			for (int isub = 0; isub < iSubMaterial; isub++)
			{
				GetNextLine();
				LoadMaterialData(i, isub);
			}
			return;
		}break;

		case SUBNO:
		{
			if (iSub < 0)
			{
				GetValue(&m_MtrlList[i].iSubNo, INT_DATA);
			}
			else
			{
				GetValue(&m_MtrlList[i].SubMaterial[iSub].iSubNo, INT_DATA);
			}
		}
			break;
			
		case TEXTURE:
		{
			CHAR szTexturePath[256] = { 0, };
			if (iSub < 0)
			{
				GetValue(m_MtrlList[i].szTextureName, STRING_DATA);
			}
			else
			{
				GetValue(m_MtrlList[i].SubMaterial[iSub].szTextureName, STRING_DATA);
			}
		}
		break;
		}
	}
}

void C_ASEParser::LoadGeomobject()
{
	int iType = -1;
	int iObject = 0;
	while ((iType = SearchTokenArray(GeomObjectSections)) >= 0)
	{
		switch (iType)
		{
		case NODENAME:
		{
			CGeomObject obj;
			m_ObjectList.push_back(obj);
			iObject = m_ObjectList.size() - 1;
			GetValue(&m_ObjectList[iObject].szName, STRING_DATA);
		}break;
		case PARENTNAME:
		{
			GetValue(&m_ObjectList[iObject].szParent, STRING_DATA);
		}break;
		case TM:
		{
			GetData("TM_ROW0", &m_ObjectList[iObject].v[0], VECTOR_DATA);
			GetData("TM_ROW1", &m_ObjectList[iObject].v[2], VECTOR_DATA);
			GetData("TM_ROW2", &m_ObjectList[iObject].v[1], VECTOR_DATA);
			GetData("TM_ROW3", &m_ObjectList[iObject].v[3], VECTOR_DATA);
		}break;
		case MESH:
		{
			GetNextLine();
			// pos
			GetData("MESH_NUMVERTEX", &m_ObjectList[iObject].iNumPosVertex, INT_DATA);

			
			GetData("MESH_NUMFACES", &m_ObjectList[iObject].iNumFace, INT_DATA);
			m_ObjectList[iObject].PosVertexList.resize(m_ObjectList[iObject].iNumPosVertex);

			//*MESH_VERTEX_LIST{
			if (GetData("MESH_VERTEX_LIST", nullptr, NULL_DATA))
			{
				for (int iVertex = 0; iVertex < m_ObjectList[iObject].iNumPosVertex; iVertex++)
				{
					GetNextLine();
					GetValue(&m_ObjectList[iObject].PosVertexList[iVertex], VERTEX_DATA);
				}
			}

			//*MESH_FACE_LIST 
			if (GetData("MESH_FACE_LIST", nullptr, NULL_DATA))
			{
				m_ObjectList[iObject].PosFaceList.resize(m_ObjectList[iObject].iNumFace);

				for (int iFace = 0; iFace < m_ObjectList[iObject].iNumFace; iFace++)
				{
					GetNextLine();
					//*MESH_FACE    0:    A:    0 B : 2 C : 3 AB : 1 BC : 1 CA : 0 * MESH_SMOOTHING 2 * MESH_MTLID 1
					int iIndex;
					sscanf_s(m_smbTokenData.c_str(), "%s%s%s %d %s%d %s%d",
						m_TempString, _countof(m_TempString),
						m_TempString, _countof(m_TempString),
						m_TempString, _countof(m_TempString),
						&m_ObjectList[iObject].PosFaceList[iFace].i[0],
						m_TempString, _countof(m_TempString),
						&m_ObjectList[iObject].PosFaceList[iFace].i[2],
						m_TempString, _countof(m_TempString),
						&m_ObjectList[iObject].PosFaceList[iFace].i[1]);

					GetNextLine();
					GetNextLine();
					GetValue(&m_ObjectList[iObject].PosFaceList[iFace].subMtrl, INT_DATA);
				}
			}
			//*MESH_NUMTVERTEX 12
			GetData("MESH_NUMTVERTEX", &m_ObjectList[iObject].iNumUVVertex, INT_DATA);
			//*MESH_TVERTLIST {
			if (m_ObjectList[iObject].iNumUVVertex > 0)
			{
				m_ObjectList[iObject].UVVertexList.resize(m_ObjectList[iObject].iNumUVVertex);
				GetNextLine();
				int index;
				for (int iVertex = 0; iVertex < m_ObjectList[iObject].iNumUVVertex; iVertex++)
				{
					GetNextLine();
					sscanf_s(m_smbTokenData.c_str(), "%s %d %f%f", m_TempString, _countof(m_TempString),
						&index,&m_ObjectList[iObject].UVVertexList[iVertex].x,
						&m_ObjectList[iObject].UVVertexList[iVertex].y);
					m_ObjectList[iObject].UVVertexList[iVertex].y = 1.0f - m_ObjectList[iObject].UVVertexList[iVertex].y;
				}
			}
			//MESH_NUMTVFACES 12
			GetData("MESH_NUMTVFACES", &m_ObjectList[iObject].iNumUVFace, INT_DATA);
			if (m_ObjectList[iObject].iNumUVFace > 0)
			{
				m_ObjectList[iObject].UVFaceList.resize(m_ObjectList[iObject].iNumUVFace);
				GetNextLine();

				for (int iVertex = 0; iVertex < m_ObjectList[iObject].iNumUVFace; iVertex++)
				{
					GetNextLine();
					GetValue(&m_ObjectList[iObject].UVFaceList[iVertex],
						INDEX_DATA);
				}
			}
			//*MESH_NUMCVERTEX 0
			GetData("MESH_NUMCVERTEX", &m_ObjectList[iObject].iNumColorVertex, INT_DATA);
			if (m_ObjectList[iObject].iNumColorVertex > 0)
			{
				m_ObjectList[iObject].ColorVertexList.resize(
					m_ObjectList[iObject].iNumColorVertex);
				GetNextLine();
				int index;
				for (int iVertex = 0; iVertex < m_ObjectList[iObject].iNumColorVertex; iVertex++)
				{
					GetNextLine();
					sscanf_s(m_smbTokenData.c_str(), "%s %d %f%f%f", m_TempString, _countof(m_TempString),&index,&m_ObjectList[iObject].ColorVertexList[iVertex].x,
						&m_ObjectList[iObject].ColorVertexList[iVertex].y,&m_ObjectList[iObject].ColorVertexList[iVertex].z);
				}
				//MESH_NUMCVFACES 
				GetData("MESH_NUMCVFACES", &m_ObjectList[iObject].iNumColorFace, INT_DATA);
				if (m_ObjectList[iObject].iNumColorFace > 0)
				{
					m_ObjectList[iObject].ColorFaceList.resize(m_ObjectList[iObject].iNumColorFace);
					GetNextLine();

					for (int iVertex = 0; iVertex < m_ObjectList[iObject].iNumColorFace; iVertex++)
					{
						GetNextLine();
						GetValue(&m_ObjectList[iObject].ColorFaceList[iVertex], INDEX_DATA);
					}
				}
			}

			//*MESH_NORMALS {*/
			if (GetData("MESH_NORMALS", nullptr, NULL_DATA))
			{
				m_ObjectList[iObject].NormalVertexList.resize(
					m_ObjectList[iObject].iNumFace);
				GetNextLine();
				for (int iFace = 0; iFace < m_ObjectList[iObject].iNumFace; iFace++)
				{
					GetNextLine();
					GetValue(&m_ObjectList[iObject].NormalVertexList[iFace].p[0], VERTEX_DATA);
					
					GetNextLine();
					GetValue(&m_ObjectList[iObject].NormalVertexList[iFace].p[2], VERTEX_DATA);
					
					GetNextLine();
					GetValue(&m_ObjectList[iObject].NormalVertexList[iFace].p[1], VERTEX_DATA);
					GetNextLine();
				}
			}
		}break;
		case REF:
		{
			GetValue(&m_ObjectList[iObject].m_iRef, INT_DATA);
			break;
		}

		case ANIMATION:
		{
			GetNextLine();
			if (GetData("CONTROL_POS_TRACK", NULL, NULL_DATA, FALSE))
			{
				CAnimationTrack track;
				ZeroMemory(&track, sizeof(CAnimationTrack));
				//GetNextLine();
				while (1)
				{
					GetNextLine();

					sscanf_s(m_smbTokenData.c_str(), "%s%d%f%f%f",	m_TempString, _countof(m_TempString), &track.iTick,
						&track.p.x, &track.p.z, &track.p.y);
					m_ObjectList[iObject].AniTrack_Pos.push_back(track);
					if (strstr(m_smbTokenData.c_str(), "}"))
					{
						break;
					}
				}
			}

			if(GetData("CONTROL_ROT_TRACK", NULL, NULL_DATA, FALSE))
			{
				CAnimationTrack track;
				ZeroMemory(&track, sizeof(CAnimationTrack));
				D3DXQUATERNION qBefore;
				D3DXQuaternionIdentity(&qBefore);
				//GetNextLine();
				while (1)
				{
					GetNextLine();

					sscanf_s(m_smbTokenData.c_str(), "%s%d%f%f%f%f", m_TempString, _countof(m_TempString), &track.iTick,
						&track.p.x, &track.p.z, &track.p.y,&track.q.w);
					D3DXQuaternionRotationAxis(&track.q, &track.p, track.q.w);		//임의의 축에대한 벡터 좌표를 쿼터니언으로 넘겨준다.
					track.q = track.q * qBefore;
					m_ObjectList[iObject].AniTrack_Rotate.push_back(track);
					if (strstr(m_smbTokenData.c_str(), "}"))
					{
						break;
					}
					qBefore = track.q;						//축에대한 정보는 이전에서 얼마나 축으로 돌았는지가 되어있으므로 그 값을 더해 주기위한 식
				}
			}


			if (GetData("CONTROL_SCALE_TRACK", NULL, NULL_DATA, FALSE))
			{
				CAnimationTrack track;
				ZeroMemory(&track, sizeof(CAnimationTrack));
				D3DXVECTOR3 vsc;
				//GetNextLine();
				while (1)
				{
					GetNextLine();

					sscanf_s(m_smbTokenData.c_str(), "%s%d%f%f%f%f%f%f%f", m_TempString, _countof(m_TempString), &track.iTick,
						&track.p.x, &track.p.z, &track.p.y, &vsc.x, &vsc.z, &vsc.y,&track.q.w);
					D3DXQuaternionRotationAxis(&track.q, &vsc, track.q.w);		//스케일도 임의의 축으로 스케일을 가진다.
					m_ObjectList[iObject].AniTrack_Scale.push_back(track);
					if (strstr(m_smbTokenData.c_str(), "}"))
					{
						break;
					}
				}
			}
		}
		break;
		}
	}
}

bool C_ASEParser::Release()
{
	m_MtrlList.clear();
	m_ObjectList.clear();
	m_ListTokens.clear();

	return true;
}

int  C_ASEParser::GetParentIndex(C_STR szParentName)
{
	for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
	{
		C_STR szName = m_ObjectList[iObj].szName;
		if (szName == szParentName)
		{
			return iObj;
		}
	}
	return -1;
}

C_ASEParser::C_ASEParser()
{
	m_pwcMeshData = nullptr;
	m_pmbMeshData = nullptr;
	m_iTokenindex = 0;
}


C_ASEParser::~C_ASEParser()
{
}
