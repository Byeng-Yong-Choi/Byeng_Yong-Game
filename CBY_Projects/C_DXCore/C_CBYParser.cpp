#include "C_CBYParser.h"

template <typename OutputIterator>
void C_CBYParser::Tokenize(const C_STR& text, const C_STR& token, OutputIterator ListData)			//토큰, 즉 token로 잘라내서 파일을 읽기위해
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

bool C_CBYParser::BeginToken()		//1차원 데이터 리스토어
{
	C_STR BufferData = m_pmbMeshData;		//버퍼안에 든 데이터
	Tokenize(BufferData, "*", back_inserter(m_ListTokens));
	return true;
}

bool C_CBYParser::Load(const TCHAR* pszfilename)
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

bool C_CBYParser::GetNextLine()		//다음 줄의 데이터를 받아오기
{
	if (m_iTokenindex >= m_ListTokens.size())				//가지고있는 버퍼보다 크면 나간다
	{
		return false;
	}

	m_smbTokenData = m_ListTokens[m_iTokenindex++];
	return true;
}

int C_CBYParser::SearchTokenArray(std::vector<std::string>& asesections)
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
			if (strstr(m_smbTokenData.c_str(), asesections[iType].c_str()) != nullptr)		//str안에서 str찾기
			{
				iRet = iType;
				break;
			}
		}
	}

	return iRet;
}

const CHAR* C_CBYParser::GetSearchString(const CHAR* pString, BOOL bLoop)
{
	//GetNextLine();
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

bool C_CBYParser::GetValue(VOID* pData, INT dataType)
{
	//assert(pData);
	switch (dataType)
	{
	case SCENE_DATA:
	{
		sscanf_s(m_smbTokenData.c_str(), "%s%d%d%d%d%d%d", m_TempString,
			_countof(m_TempString), &m_Scene.iFirstFrame,
			&m_Scene.iLastFrame, &m_Scene.iFrameSpeed, &m_Scene.iTickPerFrame, &m_Scene.iNumMaterial, &m_Scene.iNumMesh);
		return true;
	}break;

	}
	return true;
}

bool C_CBYParser::GetData(const CHAR* pString, VOID* pData, INT dataType, BOOL bLoop)
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


void C_CBYParser::LoadScene()
{
	GetData("SCENE", nullptr, SCENE_DATA);
}

void C_CBYParser::LoadMaterial()
{
	m_MtrlList.resize(m_Scene.iNumMaterial);
	for (int mtr = 0; mtr < m_MtrlList.size(); mtr++)
	{
		LoadMaterialData(mtr, -1);
	}
}

void C_CBYParser::LoadMaterialData(int i, int iSub)
{
	int iDataType = -1;
	int subno = -1;
	int front = 0;
	while ((iDataType = SearchTokenArray(CBYMaterialSections)) >= 0)
	{
		if (m_MtrlList[i].SubMaterial.size() > 0 && iDataType == 0)
		{
			iDataType += 1;
		}

		switch (iDataType)
		{
		case NEWMAT:
		{
			front = 1;
			int a = 0;
			sscanf_s(m_smbTokenData.c_str(), "%s%d%d", m_TempString,
				_countof(m_TempString), &a, &m_MtrlList[i].iSubNo);

			if (m_MtrlList[i].iSubNo > 0)
			{
				m_MtrlList[i].SubMaterial.resize(m_MtrlList[i].iSubNo);
				break;
			}
			else
			{
				break;
			}
		}
		case NEWSUBMAT:
		{
			subno++;
			if (subno >= m_MtrlList[i].SubMaterial.size())
			{
				m_iTokenindex--;
				return;
			}

			if (subno >= m_MtrlList[i].SubMaterial.size() - 1)
			{
				GetNextLine();
			}
			else
			{
				break;
			}

		}

		case TEXT:
		{
			if (front == 0)
			{
				break;
			}

			int a = 0;
			int b = 0;
			if (m_MtrlList[i].iSubNo > 0)
			{
				sscanf_s(m_smbTokenData.c_str(), "%s%d%d%s", m_TempString,
					_countof(m_TempString), &a, &b, m_MtrlList[i].SubMaterial[subno].szTextureName, _countof(m_MtrlList[i].SubMaterial[subno].szTextureName));
				if (subno >= m_MtrlList[i].SubMaterial.size() - 1)
				{
					m_iTokenindex--;
					return;
				}
			}
			else
			{
				sscanf_s(m_smbTokenData.c_str(), "%s%d%d%s", m_TempString,
					_countof(m_TempString), &a, &b, &m_MtrlList[i].szTextureName, _countof(m_MtrlList[i].szTextureName));

				return;
			}


		}
		break;
		}
	}
}

void C_CBYParser::LoadGeomobject()
{
	int iType = -1;
	int iObject = 0;
	int imesh = 0;
	int isub = -1;
	
	m_Mesh.clear();
	CBY_Mesh mesh;
	m_Mesh.push_back(mesh);

	int a = 0;
	int rot, pos, scl, list, sub;
	rot = pos = scl = list = sub = 0;

	sscanf_s(m_smbTokenData.c_str(), "%s%d%d%d%d%d%d%d",
		m_TempString,
		_countof(m_TempString),
		&a,
		&list,
		&m_Mesh[imesh].m_iTexIndex,
		&pos,
		&rot,
		&scl,
		&sub);

	m_Mesh[imesh].vblist.resize(list);
	m_Mesh[imesh].posTrack.resize(pos);
	m_Mesh[imesh].rotTrack.resize(rot);
	m_Mesh[imesh].sclTrack.resize(scl);
	m_Mesh[imesh].m_subMesh.resize(sub);

	while ((iType = SearchTokenArray(CBYGeomObjectSections)) >= 0)
	{
		switch (iType)
		{
		case OBJECT:
		{
			isub = -1;
			CBY_Mesh mesh;
			m_Mesh.push_back(mesh);
			imesh++;
			sscanf_s(m_smbTokenData.c_str(), "%s%d%d%d%d%d%d%d",
				m_TempString,
				_countof(m_TempString),
				&a,
				&list,
				&m_Mesh[imesh].m_iTexIndex,
				&pos,
				&rot,
				&scl,
				&sub);

			m_Mesh[imesh].vblist.resize(list);
			m_Mesh[imesh].posTrack.resize(pos);
			m_Mesh[imesh].rotTrack.resize(rot);
			m_Mesh[imesh].sclTrack.resize(scl);
			m_Mesh[imesh].m_subMesh.resize(sub);
		}
		break;

		case NAME:
		{
			CHAR Name[255];
			CHAR Parent[255];
			ZeroMemory(Name, sizeof(Name));
			ZeroMemory(Parent, sizeof(Parent));


			/*sscanf_s(m_smbTokenData.c_str(), "%s%s%s", m_TempString,
				_countof(m_TempString), m_Mesh[imesh].m_szName, sizeof(m_Mesh[imesh].m_szName), m_Mesh[imesh].m_szParentName, sizeof(m_Mesh[imesh].m_szParentName));*/

			sscanf_s(m_smbTokenData.c_str(), "%s%s%s", m_TempString,
				_countof(m_TempString), Name, _countof(Name), Parent, _countof(Parent));

			m_Mesh[imesh].m_szName = Name;
			m_Mesh[imesh].m_szParentName = Parent;
		}
		break;

		case WORLD:
		{
			sscanf_s(m_smbTokenData.c_str(), "%s%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f",
				m_TempString,
				_countof(m_TempString),
				&m_Mesh[imesh].m_matWorld._11,
				&m_Mesh[imesh].m_matWorld._12,
				&m_Mesh[imesh].m_matWorld._13,
				&m_Mesh[imesh].m_matWorld._14,
				&m_Mesh[imesh].m_matWorld._21,
				&m_Mesh[imesh].m_matWorld._22,
				&m_Mesh[imesh].m_matWorld._23,
				&m_Mesh[imesh].m_matWorld._24,
				&m_Mesh[imesh].m_matWorld._31,
				&m_Mesh[imesh].m_matWorld._32,
				&m_Mesh[imesh].m_matWorld._33,
				&m_Mesh[imesh].m_matWorld._34,
				&m_Mesh[imesh].m_matWorld._41,
				&m_Mesh[imesh].m_matWorld._42,
				&m_Mesh[imesh].m_matWorld._43,
				&m_Mesh[imesh].m_matWorld._44);

			//GetData("TM_ROW0", &m_ObjectList[iObject].v[0], VECTOR_DATA);			
		}
		break;

		case SUBMESH:
		{
			isub++;
			sscanf_s(m_smbTokenData.c_str(), "%s%d%d%d%d%d%d",
				m_TempString,
				_countof(m_TempString),
				&a,
				&list,
				&m_Mesh[imesh].m_subMesh[isub].m_iTexIndex,
				&pos,
				&rot,
				&scl);

			m_Mesh[imesh].m_subMesh[isub].vblist.resize(list);
			m_Mesh[imesh].m_subMesh[isub].posTrack.resize(pos);
			m_Mesh[imesh].m_subMesh[isub].rotTrack.resize(rot);
			m_Mesh[imesh].m_subMesh[isub].sclTrack.resize(scl);
		}
		break;

		case GE_POSITION:
		{
			if (m_Mesh[imesh].m_subMesh.size() > 0)
			{
				for (int i = 0; i < m_Mesh[imesh].m_subMesh[isub].vblist.size(); i++)
				{
					GetNextLine();
					sscanf_s(m_smbTokenData.c_str(), "%s%f%f%f",
						m_TempString,
						_countof(m_TempString),
						&m_Mesh[imesh].m_subMesh[isub].vblist[i].p.x,
						&m_Mesh[imesh].m_subMesh[isub].vblist[i].p.y,
						&m_Mesh[imesh].m_subMesh[isub].vblist[i].p.z);

					/*D3DXMATRIX mat;
					D3DXMatrixInverse(&mat, NULL, &m_Mesh[imesh].m_matWorld);
					D3DXVec3TransformCoord(&m_Mesh[imesh].m_subMesh[isub].vblist[i].p,&m_Mesh[imesh].m_subMesh[isub].vblist[i].p,&mat);*/
				}
			}
			else
			{
				for (int i = 0; i < m_Mesh[imesh].vblist.size(); i++)
				{
					GetNextLine();
					sscanf_s(m_smbTokenData.c_str(), "%s%f%f%f",
						m_TempString,
						_countof(m_TempString),
						&m_Mesh[imesh].vblist[i].p.x,
						&m_Mesh[imesh].vblist[i].p.y,
						&m_Mesh[imesh].vblist[i].p.z);

					/*D3DXMATRIX mat;
					D3DXMatrixInverse(&mat, NULL, &m_Mesh[imesh].m_matWorld);
					D3DXVec3TransformCoord(&m_Mesh[imesh].vblist[i].p, &m_Mesh[imesh].vblist[i].p, &mat);*/
				}
			}
		}break;
		case NOMAL:
		{
			if (m_Mesh[imesh].m_subMesh.size() > 0)
			{
				for (int i = 0; i < m_Mesh[imesh].m_subMesh[isub].vblist.size(); i++)
				{
					GetNextLine();
					sscanf_s(m_smbTokenData.c_str(), "%s%f%f%f",
						m_TempString,
						_countof(m_TempString),
						&m_Mesh[imesh].m_subMesh[isub].vblist[i].n.x,
						&m_Mesh[imesh].m_subMesh[isub].vblist[i].n.y,
						&m_Mesh[imesh].m_subMesh[isub].vblist[i].n.z);
				}
			}
			else
			{
				for (int i = 0; i < m_Mesh[imesh].vblist.size(); i++)
				{
					GetNextLine();
					sscanf_s(m_smbTokenData.c_str(), "%s%f%f%f",
						m_TempString,
						_countof(m_TempString),
						&m_Mesh[imesh].vblist[i].n.x,
						&m_Mesh[imesh].vblist[i].n.y,
						&m_Mesh[imesh].vblist[i].n.z);
				}
			}
		}break;
		case COLOR:
		{
			if (m_Mesh[imesh].m_subMesh.size() > 0)
			{
				for (int i = 0; i < m_Mesh[imesh].m_subMesh[isub].vblist.size(); i++)
				{
					GetNextLine();
					sscanf_s(m_smbTokenData.c_str(), "%s%f%f%f%f",
						m_TempString,
						_countof(m_TempString),
						&m_Mesh[imesh].m_subMesh[isub].vblist[i].c.x,
						&m_Mesh[imesh].m_subMesh[isub].vblist[i].c.y,
						&m_Mesh[imesh].m_subMesh[isub].vblist[i].c.z,
						&m_Mesh[imesh].m_subMesh[isub].vblist[i].c.w);
				}
			}
			else
			{
				for (int i = 0; i < m_Mesh[imesh].vblist.size(); i++)
				{
					GetNextLine();
					sscanf_s(m_smbTokenData.c_str(), "%s%f%f%f%f",
						m_TempString,
						_countof(m_TempString),
						&m_Mesh[imesh].vblist[i].c.x,
						&m_Mesh[imesh].vblist[i].c.y,
						&m_Mesh[imesh].vblist[i].c.z,
						&m_Mesh[imesh].vblist[i].c.w);
				}
			}
		}break;


		case UV:
		{
			if (m_Mesh[imesh].m_subMesh.size() > 0)
			{
				for (int i = 0; i < m_Mesh[imesh].m_subMesh[isub].vblist.size(); i++)
				{
					GetNextLine();
					sscanf_s(m_smbTokenData.c_str(), "%s%f%f",
						m_TempString,
						_countof(m_TempString),
						&m_Mesh[imesh].m_subMesh[isub].vblist[i].t.x,
						&m_Mesh[imesh].m_subMesh[isub].vblist[i].t.y);
				}
			}
			else
			{
				for (int i = 0; i < m_Mesh[imesh].vblist.size(); i++)
				{
					GetNextLine();
					sscanf_s(m_smbTokenData.c_str(), "%s%f%f",
						m_TempString,
						_countof(m_TempString),
						&m_Mesh[imesh].vblist[i].t.x,
						&m_Mesh[imesh].vblist[i].t.y);
				}
			}
		}break;

		case ANI:
		{
			if (m_Mesh[imesh].m_subMesh.size() > 0)
			{
				//pos
				for (int iTrack = 0; iTrack < m_Mesh[imesh].m_subMesh[isub].posTrack.size(); iTrack++)
				{
					GetNextLine();
					sscanf_s(m_smbTokenData.c_str(), "%s%d%f%f%f", m_TempString,
						_countof(m_TempString),
						&m_Mesh[imesh].m_subMesh[isub].posTrack[iTrack].iTick,
						&m_Mesh[imesh].m_subMesh[isub].posTrack[iTrack].p.x,
						&m_Mesh[imesh].m_subMesh[isub].posTrack[iTrack].p.y,
						&m_Mesh[imesh].m_subMesh[isub].posTrack[iTrack].p.z);

				}

				//rot
				for (int iTrack = 0; iTrack < m_Mesh[imesh].m_subMesh[isub].rotTrack.size(); iTrack++)
				{
					GetNextLine();
					sscanf_s(m_smbTokenData.c_str(), "%s%d%f%f%f%f", m_TempString,
						_countof(m_TempString),
						&m_Mesh[imesh].m_subMesh[isub].rotTrack[iTrack].iTick,
						&m_Mesh[imesh].m_subMesh[isub].rotTrack[iTrack].q.x,
						&m_Mesh[imesh].m_subMesh[isub].rotTrack[iTrack].q.y,
						&m_Mesh[imesh].m_subMesh[isub].rotTrack[iTrack].q.z,
						&m_Mesh[imesh].m_subMesh[isub].rotTrack[iTrack].q.w);
				}

				//scl
				for (int iTrack = 0; iTrack < m_Mesh[imesh].m_subMesh[isub].sclTrack.size(); iTrack++)
				{
					GetNextLine();
					sscanf_s(m_smbTokenData.c_str(), "%s%d%f%f%f%f%f%f%f", m_TempString,
						_countof(m_TempString),
						&m_Mesh[imesh].m_subMesh[isub].sclTrack[iTrack].iTick,

						&m_Mesh[imesh].m_subMesh[isub].sclTrack[iTrack].p.x,
						&m_Mesh[imesh].m_subMesh[isub].sclTrack[iTrack].p.y,
						&m_Mesh[imesh].m_subMesh[isub].sclTrack[iTrack].p.z,

						&m_Mesh[imesh].m_subMesh[isub].sclTrack[iTrack].q.x,
						&m_Mesh[imesh].m_subMesh[isub].sclTrack[iTrack].q.y,
						&m_Mesh[imesh].m_subMesh[isub].sclTrack[iTrack].q.z,
						&m_Mesh[imesh].m_subMesh[isub].sclTrack[iTrack].q.w);
				}
			}
			else
			{
				//pos
				for (int iTrack = 0; iTrack < m_Mesh[imesh].posTrack.size(); iTrack++)
				{
					GetNextLine();
					sscanf_s(m_smbTokenData.c_str(), "%s%d%f%f%f", m_TempString,
						_countof(m_TempString),
						&m_Mesh[imesh].posTrack[iTrack].iTick,
						&m_Mesh[imesh].posTrack[iTrack].p.x,
						&m_Mesh[imesh].posTrack[iTrack].p.y,
						&m_Mesh[imesh].posTrack[iTrack].p.z);
				}

				//rot
				for (int iTrack = 0; iTrack < m_Mesh[imesh].rotTrack.size(); iTrack++)
				{
					GetNextLine();
					sscanf_s(m_smbTokenData.c_str(), "%s%d%f%f%f%f", m_TempString,
						_countof(m_TempString),
						&m_Mesh[imesh].rotTrack[iTrack].iTick,
						&m_Mesh[imesh].rotTrack[iTrack].q.x,
						&m_Mesh[imesh].rotTrack[iTrack].q.y,
						&m_Mesh[imesh].rotTrack[iTrack].q.z,
						&m_Mesh[imesh].rotTrack[iTrack].q.w);
				}

				//scl
				for (int iTrack = 0; iTrack < m_Mesh[imesh].sclTrack.size(); iTrack++)
				{
					GetNextLine();
					sscanf_s(m_smbTokenData.c_str(), "%s%d%f%f%f%f%f%f%f", m_TempString,
						_countof(m_TempString),
						&m_Mesh[imesh].sclTrack[iTrack].iTick,

						&m_Mesh[imesh].sclTrack[iTrack].p.x,
						&m_Mesh[imesh].sclTrack[iTrack].p.y,
						&m_Mesh[imesh].sclTrack[iTrack].p.z,

						&m_Mesh[imesh].sclTrack[iTrack].q.x,
						&m_Mesh[imesh].sclTrack[iTrack].q.y,
						&m_Mesh[imesh].sclTrack[iTrack].q.z,
						&m_Mesh[imesh].sclTrack[iTrack].q.w);
				}
			}
		}
		break;
		}
	}
}

bool C_CBYParser::Release()
{
	m_MtrlList.clear();
	m_ListTokens.clear();
	m_Mesh.clear();

	return true;
}

int  C_CBYParser::GetParentIndex(C_STR szParentName)
{
	for (int iObj = 0; iObj < m_Mesh.size(); iObj++)
	{
		C_STR szName = m_Mesh[iObj].m_szName;
		if (szName == szParentName)
		{
			return iObj;
		}
	}
	return -1;
}

C_CBYParser::C_CBYParser()
{
	m_pwcMeshData = nullptr;
	m_pmbMeshData = nullptr;
	m_iTokenindex = 0;
}


C_CBYParser::~C_CBYParser()
{
}
