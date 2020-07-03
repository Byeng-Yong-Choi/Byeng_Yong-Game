#include "stdafx.h"
#include "CBY_Parser.h"

int  CBY_Parser::GetParentIndex(C_STR szParentName)
{
	for (int iObj = 0; iObj < m_ObjList.size(); iObj++)
	{
		C_STR szName = m_ObjList[iObj]->m_szName;
		if (szName == szParentName)
		{
			return iObj;
		}
	}
	return -1;
}
bool CBY_Parser::SkinLoad(const TCHAR* szFileName)
{
	if (OpenStream(szFileName) == FALSE) return false;

	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %d %d %d %d %d %d"),
		m_pString,
		&m_Scene.iFirstFrame,
		&m_Scene.iLastFrame,
		&m_Scene.iFrameSpeed,
		&m_Scene.iTickPerFrame,
		&m_Scene.iNumMaterial,
		&m_Scene.iNumMesh);

	int iTemp = 0;
	int iNumSubMtrl = 0;
	int iNumSubTexmap = 0;

	m_MtlList.resize(m_Scene.iNumMaterial);
	for (int iMtrl = 0; iMtrl < m_Scene.iNumMaterial; iMtrl++)
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s %d %d"), m_pString, &iTemp, &iNumSubMtrl);

		if (iNumSubMtrl > 0)
		{
			m_MtlList[iMtrl].SubMaterial.resize(iNumSubMtrl);
			for (int iSubMtrl = 0;
				iSubMtrl < iNumSubMtrl; iSubMtrl++)
			{
				int isubo;
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s%d%d"),
					m_pString,
					&isubo,
					&iNumSubTexmap);
				m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList.resize(iNumSubTexmap);

				for (int iMap = 0;
					iMap < iNumSubTexmap; iMap++)
				{
					_fgetts(m_pBuffer, 256, m_pStream);
					_stscanf(m_pBuffer, _T("%s %d %d %s\n"), m_pString, &iTemp, &m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].iSubNo,
						m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].szTextureName);

				}
			}
		}
		else
		{
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s %d %d"), m_pString, &iTemp, &iNumSubTexmap);
			m_MtlList[iMtrl].texList.resize(iNumSubTexmap);
			for (int iMap = 0;
				iMap < iNumSubTexmap; iMap++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s %d %d %s"),
					m_pString,
					&iTemp,
					&m_MtlList[iMtrl].texList[iMap].iSubNo,
					m_MtlList[iMtrl].texList[iMap].szTextureName);

			}
		}
	}

	m_ObjList.resize(m_Scene.iNumMesh);
	int iNumArray[6];
	int iNumCnt = 0;
	for (int iObj = 0; iObj < m_Scene.iNumMesh; iObj++)
	{
		m_ObjList[iObj] = new CBY_MeshSkin;
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s %d %d %d %d %d %d %d\n"),
			m_pString,
			&iTemp,
			&iNumArray[0],
			&iNumArray[1],
			&iNumArray[2],
			&iNumArray[3],
			&iNumArray[4],
			&iNumArray[5]
		);
		m_ObjList[iObj]->listSkin.resize(iNumArray[0]);
		m_ObjList[iObj]->m_iTexIndex = iNumArray[1];
		m_ObjList[iObj]->posTrack.resize(iNumArray[2]);
		m_ObjList[iObj]->rotTrack.resize(iNumArray[3]);
		m_ObjList[iObj]->sclTrack.resize(iNumArray[4]);
		m_ObjList[iObj]->subMeshSkin.resize(iNumArray[5]);


		_fgetts(m_pBuffer, 256, m_pStream);
		TCHAR szName[255] = { 0, };
		TCHAR szParentName[255] = { 0, };
		_stscanf(m_pBuffer, _T("%s %s %s"),
			m_pString,
			szName,
			szParentName);
		m_ObjList[iObj]->m_szName = wtm(szName);
		m_ObjList[iObj]->m_szParentName = wtm(szParentName);

		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"),
			m_pString,
			&m_ObjList[iObj]->m_matWorld._11,
			&m_ObjList[iObj]->m_matWorld._12,
			&m_ObjList[iObj]->m_matWorld._13,
			&m_ObjList[iObj]->m_matWorld._14,
			&m_ObjList[iObj]->m_matWorld._21,
			&m_ObjList[iObj]->m_matWorld._22,
			&m_ObjList[iObj]->m_matWorld._23,
			&m_ObjList[iObj]->m_matWorld._24,
			&m_ObjList[iObj]->m_matWorld._31,
			&m_ObjList[iObj]->m_matWorld._32,
			&m_ObjList[iObj]->m_matWorld._33,
			&m_ObjList[iObj]->m_matWorld._34,
			&m_ObjList[iObj]->m_matWorld._41,
			&m_ObjList[iObj]->m_matWorld._42,
			&m_ObjList[iObj]->m_matWorld._43,
			&m_ObjList[iObj]->m_matWorld._44);

		if (m_ObjList[iObj]->subMeshSkin.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh <
				m_ObjList[iObj]->subMeshSkin.size(); iSubMesh++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s %d %d %d %d %d %d"),
					m_pString,
					&iTemp,
					&iNumArray[0],
					&iNumArray[1],
					&iNumArray[2],
					&iNumArray[3],
					&iNumArray[4],
					&iNumArray[5]);


				m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin.resize(iNumArray[0]);
				m_ObjList[iObj]->subMeshSkin[iSubMesh].m_iTexIndex = iNumArray[1];
				m_ObjList[iObj]->subMeshSkin[iSubMesh].posTrack.resize(iNumArray[2]);
				m_ObjList[iObj]->subMeshSkin[iSubMesh].rotTrack.resize(iNumArray[3]);
				m_ObjList[iObj]->subMeshSkin[iSubMesh].sclTrack.resize(iNumArray[4]);
				//m_ObjList[iObj].subMeshSkin[iSubMesh].subMeshSkin.resize(iNumArray[5]);


				//position
				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin.size(); iV++)
				{
					_fgetts(m_pBuffer, 256, m_pStream);
					_stscanf(m_pBuffer,
						_T("%s %f %f %f"),
						m_pString,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].p.x,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].p.y,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].p.z);
				}

				//nomal
				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin.size(); iV++)
				{
					_fgetts(m_pBuffer, 256, m_pStream);
					_stscanf(m_pBuffer,
						_T("%s %f %f %f"),
						m_pString,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].n.x,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].n.y,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].n.z);
				}

				//color
				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin.size(); iV++)
				{
					_fgetts(m_pBuffer, 256, m_pStream);
					_stscanf(m_pBuffer,
						_T("%s %f %f %f %f"),
						m_pString,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].c.x,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].c.y,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].c.z,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].c.w);
				}

				//texture
				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin.size(); iV++)
				{
					_fgetts(m_pBuffer, 256, m_pStream);
					_stscanf(m_pBuffer,
						_T("%s %f %f"),
						m_pString,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].t.x,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].t.y);
				}

				//index
				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin.size(); iV++)
				{
					_fgetts(m_pBuffer, 256, m_pStream);
					_stscanf(m_pBuffer,
						_T("%s %f %f %f %f"),
						m_pString,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].i.x,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].i.y,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].i.z,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].i.w);
				}

				//weight
				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin.size(); iV++)
				{
					_fgetts(m_pBuffer, 256, m_pStream);
					_stscanf(m_pBuffer,
						_T("%s %f %f %f %f"),
						m_pString,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].w.x,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].w.y,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].w.z,
						&m_ObjList[iObj]->subMeshSkin[iSubMesh].listSkin[iV].w.w);
				}

			}
		}
		else
		{
			//position
			for (int iV = 0;
				iV < m_ObjList[iObj]->listSkin.size(); iV++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer,
					_T("%s %f %f %f"),
					m_pString,
					&m_ObjList[iObj]->listSkin[iV].p.x,
					&m_ObjList[iObj]->listSkin[iV].p.y,
					&m_ObjList[iObj]->listSkin[iV].p.z);
			}

			//nomal
			for (int iV = 0;
				iV < m_ObjList[iObj]->listSkin.size(); iV++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer,
					_T("%s %f %f %f"),
					m_pString,
					&m_ObjList[iObj]->listSkin[iV].n.x,
					&m_ObjList[iObj]->listSkin[iV].n.y,
					&m_ObjList[iObj]->listSkin[iV].n.z);
			}

			//color
			for (int iV = 0;
				iV < m_ObjList[iObj]->listSkin.size(); iV++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer,
					_T("%s %f %f %f %f"),
					m_pString,
					&m_ObjList[iObj]->listSkin[iV].c.x,
					&m_ObjList[iObj]->listSkin[iV].c.y,
					&m_ObjList[iObj]->listSkin[iV].c.z,
					&m_ObjList[iObj]->listSkin[iV].c.w);
			}

			//texture
			for (int iV = 0;
				iV < m_ObjList[iObj]->listSkin.size(); iV++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer,
					_T("%s %f %f"),
					m_pString,
					&m_ObjList[iObj]->listSkin[iV].t.x,
					&m_ObjList[iObj]->listSkin[iV].t.y);
			}

			//index
			for (int iV = 0;
				iV < m_ObjList[iObj]->listSkin.size(); iV++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer,
					_T("%s %f %f %f %f"),
					m_pString,
					&m_ObjList[iObj]->listSkin[iV].i.x,
					&m_ObjList[iObj]->listSkin[iV].i.y,
					&m_ObjList[iObj]->listSkin[iV].i.z,
					&m_ObjList[iObj]->listSkin[iV].i.w);
			}

			//weight
			for (int iV = 0;
				iV < m_ObjList[iObj]->listSkin.size(); iV++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer,
					_T("%s %f %f %f %f"),
					m_pString,
					&m_ObjList[iObj]->listSkin[iV].w.x,
					&m_ObjList[iObj]->listSkin[iV].w.y,
					&m_ObjList[iObj]->listSkin[iV].w.z,
					&m_ObjList[iObj]->listSkin[iV].w.w);
			}
		}


		// animation
		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->posTrack.size(); iTrack++)
		{
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%d %f %f %f"),
				&m_ObjList[iObj]->posTrack[iTrack].iTick,
				&m_ObjList[iObj]->posTrack[iTrack].p.x,
				&m_ObjList[iObj]->posTrack[iTrack].p.y,
				&m_ObjList[iObj]->posTrack[iTrack].p.z);
		}
		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->rotTrack.size(); iTrack++)
		{
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%d %f %f %f %f"),
				&m_ObjList[iObj]->rotTrack[iTrack].iTick,
				&m_ObjList[iObj]->rotTrack[iTrack].q.x,
				&m_ObjList[iObj]->rotTrack[iTrack].q.y,
				&m_ObjList[iObj]->rotTrack[iTrack].q.z,
				&m_ObjList[iObj]->rotTrack[iTrack].q.w);
		}
		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->sclTrack.size(); iTrack++)
		{
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%d %f %f %f %f %f %f %f"),
				&m_ObjList[iObj]->sclTrack[iTrack].iTick,

				&m_ObjList[iObj]->sclTrack[iTrack].p.x,
				&m_ObjList[iObj]->sclTrack[iTrack].p.y,
				&m_ObjList[iObj]->sclTrack[iTrack].p.z,

				&m_ObjList[iObj]->sclTrack[iTrack].q.x,
				&m_ObjList[iObj]->sclTrack[iTrack].q.y,
				&m_ObjList[iObj]->sclTrack[iTrack].q.z,
				&m_ObjList[iObj]->sclTrack[iTrack].q.w);
		}

		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s %d %f %f %f %f %f %f %f %f %f"),
			m_pString,
			&iTemp,
			&m_ObjList[iObj]->m_CharBox.vCenter.x,
			&m_ObjList[iObj]->m_CharBox.vCenter.y,
			&m_ObjList[iObj]->m_CharBox.vCenter.z,

			&m_ObjList[iObj]->m_CharBox.vMax.x,
			&m_ObjList[iObj]->m_CharBox.vMax.y,
			&m_ObjList[iObj]->m_CharBox.vMax.z,

			&m_ObjList[iObj]->m_CharBox.vMin.x,
			&m_ObjList[iObj]->m_CharBox.vMin.y,
			&m_ObjList[iObj]->m_CharBox.vMin.z);
	}
	int iBoneCount = 0;
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %d"), m_pString, &iBoneCount);

	D3DXMATRIX mat;
	for (int ibone = 0; ibone < iBoneCount; ibone++)
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"),
			&mat._11,
			&mat._12,
			&mat._13,
			&mat._14,
			&mat._21,
			&mat._22,
			&mat._23,
			&mat._24,
			&mat._31,
			&mat._32,
			&mat._33,
			&mat._34,
			&mat._41,
			&mat._42,
			&mat._43,
			&mat._44);

		m_matBoneBindPoss.push_back(mat);
	}
	CloseStream();
	Convert();
	return true;
}

bool CBY_Parser::SkinTextToBinary()
{
	TCHAR  szFile[256] = { 0, };
	TCHAR* pszname;

	pszname = SaveFileDlg(_T("skn"), _T("Export"));
	if (pszname == NULL)
	{
		return false;
	}

	_tcscpy(szFile, pszname);


	FILE* fp;
	fp = _tfopen(szFile, _T("wb"));

	int ibuf[6] = { m_Scene.iFirstFrame, m_Scene.iLastFrame,
		m_Scene.iFrameSpeed,m_Scene.iTickPerFrame, m_Scene.iNumMaterial, m_Scene.iNumMesh};
	TCHAR cbuf[256];
	fwrite((void*)ibuf, sizeof(int), 6, fp);
	
	for (int iMtrl = 0; iMtrl < m_Scene.iNumMaterial; iMtrl++)
	{
		ibuf[0] = iMtrl;
		ibuf[1]  = m_MtlList[iMtrl].SubMaterial.size();
		fwrite((void*)ibuf, sizeof(int), 2, fp);

		if (m_MtlList[iMtrl].SubMaterial.size() > 0)
		{
			for (int iSubMtrl = 0; iSubMtrl < m_MtlList[iMtrl].SubMaterial.size(); iSubMtrl++)
			{
				ibuf[0] = iSubMtrl;
				ibuf[1] = m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList.size();
				fwrite((void*)ibuf, sizeof(int), 2, fp);

				for (int iMap = 0; iMap < m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList.size(); iMap++)
				{
					ibuf[0] = iMap;
					ibuf[1] = m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].iSubNo;
					int length[1] = { _tcslen(m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].szTextureName) };
					_tcscpy_s(cbuf, m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].szTextureName);
					fwrite((void*)ibuf, sizeof(int), 2, fp);
					fwrite((void*)length, sizeof(int), 1, fp);
					fwrite((void*)cbuf, sizeof(TCHAR), length[0], fp);
				}
			}
		}
		else
		{
			ibuf[0] = iMtrl;
			ibuf[1] = m_MtlList[iMtrl].texList.size();
			fwrite((void*)ibuf, sizeof(int), 2, fp);
			for (int iMap = 0; iMap < m_MtlList[iMtrl].texList.size(); iMap++)
			{
				ibuf[0] = iMap;
				ibuf[1] = m_MtlList[iMtrl].texList[iMap].iSubNo;
				int length[1] = { _tcslen(m_MtlList[iMtrl].texList[iMap].szTextureName) };
				_tcscpy_s(cbuf, m_MtlList[iMtrl].texList[iMap].szTextureName);
				fwrite((void*)ibuf, sizeof(int), 2, fp);
				fwrite((void*)length, sizeof(int), 1, fp);
				fwrite((void*)cbuf, sizeof(TCHAR), length[0], fp);
			}
		}
	}

	for (int iObj = 0; iObj < m_Scene.iNumMesh; iObj++)
	{
		int objibuf[7] = { m_ObjList[iObj]->vblistSkin.size(), m_ObjList[iObj]->iblistSkin.size(),
			m_ObjList[iObj]->m_iTexIndex, m_ObjList[iObj]->posTrack.size(),
		m_ObjList[iObj]->rotTrack.size(), m_ObjList[iObj]->sclTrack.size(), m_ObjList[iObj]->subMeshSkin.size()};
		fwrite((void*)objibuf, sizeof(int), 7, fp);

		std::wstring str;
		str = mtw(m_ObjList[iObj]->m_szName);
		_tcscpy_s(cbuf, str.c_str());
		int length[1] = { str.length() };
		fwrite((void*)length, sizeof(int), 1, fp);
		fwrite((void*)cbuf, sizeof(TCHAR), length[0], fp);

		str = mtw(m_ObjList[iObj]->m_szParentName);
		_tcscpy_s(cbuf, str.c_str());
		length[0] = str.length();
		fwrite((void*)length, sizeof(int), 1, fp);
		fwrite((void*)cbuf, sizeof(TCHAR), length[0], fp);

		float fBuf[16] = { 
			m_ObjList[iObj]->m_matWorld._11,
			m_ObjList[iObj]->m_matWorld._12,
			m_ObjList[iObj]->m_matWorld._13,
			m_ObjList[iObj]->m_matWorld._14,
			m_ObjList[iObj]->m_matWorld._21,
			m_ObjList[iObj]->m_matWorld._22,
			m_ObjList[iObj]->m_matWorld._23,
			m_ObjList[iObj]->m_matWorld._24,
			m_ObjList[iObj]->m_matWorld._31,
			m_ObjList[iObj]->m_matWorld._32,
			m_ObjList[iObj]->m_matWorld._33,
			m_ObjList[iObj]->m_matWorld._34,
			m_ObjList[iObj]->m_matWorld._41,
			m_ObjList[iObj]->m_matWorld._42,
			m_ObjList[iObj]->m_matWorld._43,
			m_ObjList[iObj]->m_matWorld._44
		};
		fwrite((void*)fBuf, sizeof(float), 16, fp);

		float verfbuf[10];

		if (m_ObjList[iObj]->subMeshSkin.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh <
				m_ObjList[iObj]->subMeshSkin.size(); iSubMesh++)
			{
				int subibuf[7] =
				{
					iSubMesh,
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(),
					m_ObjList[iObj]->subMeshSkin[iSubMesh].iblistSkin.size(),
					m_ObjList[iObj]->subMeshSkin[iSubMesh].m_iTexIndex,
					m_ObjList[iObj]->subMeshSkin[iSubMesh].posTrack.size(),
					m_ObjList[iObj]->subMeshSkin[iSubMesh].rotTrack.size(),
					m_ObjList[iObj]->subMeshSkin[iSubMesh].sclTrack.size()
				};
				fwrite((void*)subibuf, sizeof(int), 6, fp);

				
				for (int iV = 0; iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
				{
					verfbuf[0] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].p.x;
					verfbuf[1] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].p.y;
					verfbuf[2] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].p.z;
					
					fwrite((void*)verfbuf, sizeof(float), 3, fp);
				}


				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
				{
					verfbuf[0] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].n.x;
					verfbuf[1] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].n.y;
					verfbuf[2] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].n.z;

					fwrite((void*)verfbuf, sizeof(float), 3, fp);
				}


				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
				{
					verfbuf[0] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].c.x;
					verfbuf[1] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].c.y;
					verfbuf[2] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].c.z;
					verfbuf[3] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].c.w;

					fwrite((void*)verfbuf, sizeof(float), 4, fp);
				}


				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
				{
					verfbuf[0] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].t.x;
					verfbuf[1] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].t.y;

					fwrite((void*)verfbuf, sizeof(float), 2, fp);
				}


				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
				{
					verfbuf[0] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].i.x;
					verfbuf[1] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].i.y;
					verfbuf[2] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].i.z;
					verfbuf[3] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].i.w;

					fwrite((void*)verfbuf, sizeof(float), 4, fp);
				}

				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
				{
					verfbuf[0] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].w.x;
					verfbuf[1] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].w.y;
					verfbuf[2] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].w.z;
					verfbuf[3] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].w.w;

					fwrite((void*)verfbuf, sizeof(float), 4, fp);
				}

				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
				{
					verfbuf[0] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].tan.x;
					verfbuf[1] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].tan.y;
					verfbuf[2] = m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].tan.z;
					fwrite((void*)verfbuf, sizeof(float), 3, fp);
				}

				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].iblistSkin.size(); iV++)
				{
					int indexbuf[1] = { 0, };
					indexbuf[0] = m_ObjList[iObj]->subMeshSkin[iSubMesh].iblistSkin[iV];
					fwrite((void*)indexbuf, sizeof(int), 1, fp);
				}
			}
		}
		else
		{
			for (int iV = 0; iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
			{
				verfbuf[0] = m_ObjList[iObj]->vblistSkin[iV].p.x;
				verfbuf[1] = m_ObjList[iObj]->vblistSkin[iV].p.y;
				verfbuf[2] = m_ObjList[iObj]->vblistSkin[iV].p.z;

				fwrite((void*)verfbuf, sizeof(float), 3, fp);
			}


			for (int iV = 0;
				iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
			{
				verfbuf[0] = m_ObjList[iObj]->vblistSkin[iV].n.x;
				verfbuf[1] = m_ObjList[iObj]->vblistSkin[iV].n.y;
				verfbuf[2] = m_ObjList[iObj]->vblistSkin[iV].n.z;

				fwrite((void*)verfbuf, sizeof(float), 3, fp);
			}


			for (int iV = 0;
				iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
			{
				verfbuf[0] = m_ObjList[iObj]->vblistSkin[iV].c.x;
				verfbuf[1] = m_ObjList[iObj]->vblistSkin[iV].c.y;
				verfbuf[2] = m_ObjList[iObj]->vblistSkin[iV].c.z;
				verfbuf[3] = m_ObjList[iObj]->vblistSkin[iV].c.w;

				fwrite((void*)verfbuf, sizeof(float), 4, fp);
			}


			for (int iV = 0;
				iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
			{
				verfbuf[0] = m_ObjList[iObj]->vblistSkin[iV].t.x;
				verfbuf[1] = m_ObjList[iObj]->vblistSkin[iV].t.y;

				fwrite((void*)verfbuf, sizeof(float), 2, fp);
			}


			for (int iV = 0;
				iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
			{
				verfbuf[0] = m_ObjList[iObj]->vblistSkin[iV].i.x;
				verfbuf[1] = m_ObjList[iObj]->vblistSkin[iV].i.y;
				verfbuf[2] = m_ObjList[iObj]->vblistSkin[iV].i.z;
				verfbuf[3] = m_ObjList[iObj]->vblistSkin[iV].i.w;

				fwrite((void*)verfbuf, sizeof(float), 4, fp);
			}

			for (int iV = 0;
				iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
			{
				verfbuf[0] = m_ObjList[iObj]->vblistSkin[iV].w.x;
				verfbuf[1] = m_ObjList[iObj]->vblistSkin[iV].w.y;
				verfbuf[2] = m_ObjList[iObj]->vblistSkin[iV].w.z;
				verfbuf[3] = m_ObjList[iObj]->vblistSkin[iV].w.w;

				fwrite((void*)verfbuf, sizeof(float), 4, fp);
			}

			for (int iV = 0;
				iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
			{
				verfbuf[0] = m_ObjList[iObj]->vblistSkin[iV].tan.x;
				verfbuf[1] = m_ObjList[iObj]->vblistSkin[iV].tan.y;
				verfbuf[2] = m_ObjList[iObj]->vblistSkin[iV].tan.z;
				fwrite((void*)verfbuf, sizeof(float), 3, fp);
			}

			for (int iV = 0;
				iV < m_ObjList[iObj]->iblistSkin.size(); iV++)
			{
				int indexbuf[1] = { 0, };
				indexbuf[0] = m_ObjList[iObj]->iblistSkin[iV];
				fwrite((void*)indexbuf, sizeof(int), 1, fp);
			}
		}

		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->posTrack.size(); iTrack++)
		{

			verfbuf[0] = m_ObjList[iObj]->posTrack[iTrack].iTick;
			verfbuf[1] = m_ObjList[iObj]->posTrack[iTrack].p.x;
			verfbuf[2] = m_ObjList[iObj]->posTrack[iTrack].p.y;
			verfbuf[3] = m_ObjList[iObj]->posTrack[iTrack].p.z;

			fwrite((void*)verfbuf, sizeof(float), 4, fp);
		}

		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->rotTrack.size(); iTrack++)
		{
			verfbuf[0] = m_ObjList[iObj]->rotTrack[iTrack].iTick;
			verfbuf[1] = m_ObjList[iObj]->rotTrack[iTrack].q.x;
			verfbuf[2] = m_ObjList[iObj]->rotTrack[iTrack].q.y;
			verfbuf[3] = m_ObjList[iObj]->rotTrack[iTrack].q.z;
			verfbuf[4] = m_ObjList[iObj]->rotTrack[iTrack].q.w;

			fwrite((void*)verfbuf, sizeof(float), 5, fp);
		}
		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->sclTrack.size(); iTrack++)
		{
			verfbuf[0] = m_ObjList[iObj]->sclTrack[iTrack].iTick;

			verfbuf[1] = m_ObjList[iObj]->sclTrack[iTrack].p.x;
			verfbuf[2] = m_ObjList[iObj]->sclTrack[iTrack].p.y;
			verfbuf[3] = m_ObjList[iObj]->sclTrack[iTrack].p.z;

			verfbuf[4] = m_ObjList[iObj]->sclTrack[iTrack].q.x;
			verfbuf[5] = m_ObjList[iObj]->sclTrack[iTrack].q.y;
			verfbuf[6] = m_ObjList[iObj]->sclTrack[iTrack].q.z;
			verfbuf[7] = m_ObjList[iObj]->sclTrack[iTrack].q.w;

			fwrite((void*)verfbuf, sizeof(float), 8, fp);
		}

		verfbuf[0] = iObj;

		verfbuf[1] = m_ObjList[iObj]->m_CharBox.vCenter.x;
		verfbuf[2] = m_ObjList[iObj]->m_CharBox.vCenter.y;
		verfbuf[3] = m_ObjList[iObj]->m_CharBox.vCenter.z;

		verfbuf[4] = m_ObjList[iObj]->m_CharBox.vMax.x;
		verfbuf[5] = m_ObjList[iObj]->m_CharBox.vMax.y;
		verfbuf[6] = m_ObjList[iObj]->m_CharBox.vMax.z;
		verfbuf[7] = m_ObjList[iObj]->m_CharBox.vMin.x;
		verfbuf[8] = m_ObjList[iObj]->m_CharBox.vMin.y;
		verfbuf[9] = m_ObjList[iObj]->m_CharBox.vMin.z;
		fwrite((void*)verfbuf, sizeof(float), 10, fp);
	}

	int objsize[1] = { m_matBoneBindPoss.size() };
	fwrite((void*)objsize, sizeof(int), 1, fp);

	for (int ibone = 0; ibone < m_matBoneBindPoss.size(); ibone++)
	{
		float fbone[16];
		fbone[0] = m_matBoneBindPoss[ibone]._11;
		fbone[1] = m_matBoneBindPoss[ibone]._12;
		fbone[2] = m_matBoneBindPoss[ibone]._13;
		fbone[3] = m_matBoneBindPoss[ibone]._14;
		fbone[4] = m_matBoneBindPoss[ibone]._21;
		fbone[5] = m_matBoneBindPoss[ibone]._22;
		fbone[6] = m_matBoneBindPoss[ibone]._23;
		fbone[7] = m_matBoneBindPoss[ibone]._24;
		fbone[8] = m_matBoneBindPoss[ibone]._31;
		fbone[9] = m_matBoneBindPoss[ibone]._32;
		fbone[10] = m_matBoneBindPoss[ibone]._33;
		fbone[11] = m_matBoneBindPoss[ibone]._34;
		fbone[12] = m_matBoneBindPoss[ibone]._41;
		fbone[13] = m_matBoneBindPoss[ibone]._42;
		fbone[14] = m_matBoneBindPoss[ibone]._43;
		fbone[15] = m_matBoneBindPoss[ibone]._44;

		fwrite((void*)fbone, sizeof(float), 16, fp);
	}
	fclose(fp);
	return true;
}
bool  CBY_Parser::SkinBinaryLoad(const TCHAR* szFileName)
{
	m_pStream = _tfopen(szFileName, _T("rb"));
	if (m_pStream == nullptr)
	{
		return false;
	}

	int ibuf[6] = {0, };
	fread((void*)ibuf, sizeof(int), 6, m_pStream);
	m_Scene.iFirstFrame	=	ibuf[0];
	m_Scene.iLastFrame	=	ibuf[1];
	m_Scene.iFrameSpeed	=	ibuf[2];
	m_Scene.iTickPerFrame =	ibuf[3];
	m_Scene.iNumMaterial =ibuf[4];
	m_Scene.iNumMesh =ibuf[5];

	m_MtlList.resize(m_Scene.iNumMaterial);

	for (int iMtrl = 0; iMtrl < m_Scene.iNumMaterial; iMtrl++)
	{
		fread((void*)ibuf, sizeof(int), 2, m_pStream);

		if (ibuf[1] > 0)
		{
			m_MtlList[iMtrl].SubMaterial.resize(ibuf[1]);
			for (int iSubMtrl = 0; iSubMtrl < m_MtlList[iMtrl].SubMaterial.size(); iSubMtrl++)
			{
				fread((void*)ibuf, sizeof(int), 2, m_pStream);
				m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList.resize(ibuf[1]);

				for (int iMap = 0; iMap < m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList.size(); iMap++)
				{
					TCHAR cbuf[256] = { 0, };
					fread((void*)ibuf, sizeof(int), 2, m_pStream);
					m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].iSubNo = ibuf[1];

					int length[1];
					fread((void*)length, sizeof(int), 1, m_pStream);

					fread((void*)cbuf, sizeof(TCHAR), length[0], m_pStream);
					_tcsncpy_s(m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].szTextureName, cbuf, sizeof(TCHAR)*length[0]);
				}
			}
		}
		else
		{
			fread((void*)ibuf, sizeof(int), 2, m_pStream);
			m_MtlList[iMtrl].texList.resize(ibuf[1]);

			for (int iMap = 0; iMap < m_MtlList[iMtrl].texList.size(); iMap++)
			{
				TCHAR cbuf[256] = { 0, };
				fread((void*)ibuf, sizeof(int), 2, m_pStream);
				m_MtlList[iMtrl].texList[iMap].iSubNo = ibuf[1];

				int length[1];
				fread((void*)length, sizeof(int), 1, m_pStream);

				fread((void*)cbuf, sizeof(TCHAR), length[0], m_pStream);
				_tcsncpy_s(m_MtlList[iMtrl].texList[iMap].szTextureName, cbuf, sizeof(TCHAR)*length[0]);
			}
		}
	}
	m_ObjList.resize(m_Scene.iNumMesh);
	for (int iObj = 0; iObj < m_Scene.iNumMesh; iObj++)
	{
		m_ObjList[iObj] = new CBY_MeshSkin;
		TCHAR cbuf[256] = { 0, };
		TCHAR cbuf2[256] = { 0, };
		
		int objibuf[7];
		fread((void*)objibuf, sizeof(int), 7, m_pStream);

		m_ObjList[iObj]->vblistSkin.resize(objibuf[0]);
		m_ObjList[iObj]->iblistSkin.resize(objibuf[1]);
		m_ObjList[iObj]->m_iTexIndex= objibuf[2];
		m_ObjList[iObj]->posTrack.resize(objibuf[3]);
		m_ObjList[iObj]->rotTrack.resize(objibuf[4]);
		m_ObjList[iObj]->sclTrack.resize(objibuf[5]);
		m_ObjList[iObj]->subMeshSkin.resize(objibuf[6]);

		
		int length[1];
		fread((void*)length, sizeof(int), 1, m_pStream);

		fread((void*)cbuf, sizeof(TCHAR), length[0], m_pStream);
		m_ObjList[iObj]->m_szName = wtm(cbuf);							//자기 이름

		fread((void*)length, sizeof(int), 1, m_pStream);
		fread((void*)cbuf2, sizeof(TCHAR), length[0], m_pStream);		//부모 이름
		m_ObjList[iObj]->m_szParentName = wtm(cbuf2);

		float fBuf[16];
		fread((void*)fBuf, sizeof(float), 16, m_pStream);
		m_ObjList[iObj]->m_matWorld._11 = fBuf[0];
		m_ObjList[iObj]->m_matWorld._12	= fBuf[1];
		m_ObjList[iObj]->m_matWorld._13	= fBuf[2];
		m_ObjList[iObj]->m_matWorld._14	= fBuf[3];
		m_ObjList[iObj]->m_matWorld._21	= fBuf[4];
		m_ObjList[iObj]->m_matWorld._22	= fBuf[5];
		m_ObjList[iObj]->m_matWorld._23	= fBuf[6];
		m_ObjList[iObj]->m_matWorld._24	= fBuf[7];
		m_ObjList[iObj]->m_matWorld._31	= fBuf[8];
		m_ObjList[iObj]->m_matWorld._32	= fBuf[9];
		m_ObjList[iObj]->m_matWorld._33	= fBuf[10];
		m_ObjList[iObj]->m_matWorld._34	= fBuf[11];
		m_ObjList[iObj]->m_matWorld._41	= fBuf[12];
		m_ObjList[iObj]->m_matWorld._42	= fBuf[13];
		m_ObjList[iObj]->m_matWorld._43	= fBuf[14];
		m_ObjList[iObj]->m_matWorld._44	= fBuf[15];

		float verfbuf[10];

		if (m_ObjList[iObj]->subMeshSkin.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh <
				m_ObjList[iObj]->subMeshSkin.size(); iSubMesh++)
			{
				int subibuf[7];
				fread((void*)subibuf, sizeof(int), 7, m_pStream);
				m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.resize(subibuf[1]);
				m_ObjList[iObj]->subMeshSkin[iSubMesh].iblistSkin.resize(subibuf[2]);
				m_ObjList[iObj]->subMeshSkin[iSubMesh].m_iTexIndex = subibuf[3];
				m_ObjList[iObj]->subMeshSkin[iSubMesh].posTrack.resize(subibuf[4]);
				m_ObjList[iObj]->subMeshSkin[iSubMesh].rotTrack.resize(subibuf[5]);
				m_ObjList[iObj]->subMeshSkin[iSubMesh].sclTrack.resize(subibuf[6]);


				for (int iV = 0; iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
				{
					fread((void*)verfbuf, sizeof(float), 3, m_pStream);
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].p.x = verfbuf[0];
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].p.y = verfbuf[1];
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].p.z = verfbuf[2];
				}


				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
				{
					fread((void*)verfbuf, sizeof(float), 3, m_pStream);
					 m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].n.x = verfbuf[0];
					 m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].n.y = verfbuf[1];
					 m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].n.z = verfbuf[2];
				}


				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
				{
					fread((void*)verfbuf, sizeof(float), 4, m_pStream);
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].c.x = verfbuf[0];
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].c.y = verfbuf[1];
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].c.z = verfbuf[2];
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].c.w = verfbuf[3];
				}


				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
				{
					fread((void*)verfbuf, sizeof(float), 2, m_pStream);
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].t.x = verfbuf[0];
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].t.y = verfbuf[1];
				}


				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
				{
					fread((void*)verfbuf, sizeof(float), 4, m_pStream);
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].i.x = verfbuf[0];
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].i.y = verfbuf[1];
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].i.z = verfbuf[2];
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].i.w = verfbuf[3];
				}

				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
				{
					fread((void*)verfbuf, sizeof(float), 4, m_pStream);
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].w.x = verfbuf[0];
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].w.y = verfbuf[1];
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].w.z = verfbuf[2];
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].w.w = verfbuf[3];
				}

				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin.size(); iV++)
				{
					fread((void*)verfbuf, sizeof(float), 3, m_pStream);

					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].tan.x = verfbuf[0];
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].tan.y = verfbuf[1];
					m_ObjList[iObj]->subMeshSkin[iSubMesh].vblistSkin[iV].tan.z = verfbuf[2];
				}

				for (int iV = 0;
					iV < m_ObjList[iObj]->subMeshSkin[iSubMesh].iblistSkin.size(); iV++)
				{
					int indexbuf[1] = { 0, };
					fread((void*)indexbuf, sizeof(int), 1, m_pStream);

					m_ObjList[iObj]->subMeshSkin[iSubMesh].iblistSkin[iV] = indexbuf[0];
				}
			}
		}
		else
		{
			for (int iV = 0; iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
			{
				fread((void*)verfbuf, sizeof(float), 3, m_pStream);
				m_ObjList[iObj]->vblistSkin[iV].p.x = verfbuf[0];
				m_ObjList[iObj]->vblistSkin[iV].p.y = verfbuf[1];
				m_ObjList[iObj]->vblistSkin[iV].p.z = verfbuf[2];
			}


			for (int iV = 0;
				iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
			{
				fread((void*)verfbuf, sizeof(float), 3, m_pStream);
				m_ObjList[iObj]->vblistSkin[iV].n.x = verfbuf[0];
				m_ObjList[iObj]->vblistSkin[iV].n.y = verfbuf[1];
				m_ObjList[iObj]->vblistSkin[iV].n.z = verfbuf[2];
			}


			for (int iV = 0;
				iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
			{
				fread((void*)verfbuf, sizeof(float), 4, m_pStream);
				m_ObjList[iObj]->vblistSkin[iV].c.x = verfbuf[0];
				m_ObjList[iObj]->vblistSkin[iV].c.y = verfbuf[1];
				m_ObjList[iObj]->vblistSkin[iV].c.z = verfbuf[2];
				m_ObjList[iObj]->vblistSkin[iV].c.w = verfbuf[3];
			}


			for (int iV = 0;
				iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
			{
				fread((void*)verfbuf, sizeof(float), 2, m_pStream);
				m_ObjList[iObj]->vblistSkin[iV].t.x = verfbuf[0];
				m_ObjList[iObj]->vblistSkin[iV].t.y = verfbuf[1];
			}


			for (int iV = 0;
				iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
			{
				fread((void*)verfbuf, sizeof(float), 4, m_pStream);
				m_ObjList[iObj]->vblistSkin[iV].i.x = verfbuf[0];
				m_ObjList[iObj]->vblistSkin[iV].i.y = verfbuf[1];
				m_ObjList[iObj]->vblistSkin[iV].i.z = verfbuf[2];
				m_ObjList[iObj]->vblistSkin[iV].i.w = verfbuf[3];
			}

			for (int iV = 0;
				iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
			{
				fread((void*)verfbuf, sizeof(float), 4, m_pStream);
				m_ObjList[iObj]->vblistSkin[iV].w.x = verfbuf[0];
				m_ObjList[iObj]->vblistSkin[iV].w.y = verfbuf[1];
				m_ObjList[iObj]->vblistSkin[iV].w.z = verfbuf[2];
				m_ObjList[iObj]->vblistSkin[iV].w.w = verfbuf[3];
			}

			for (int iV = 0;
				iV < m_ObjList[iObj]->vblistSkin.size(); iV++)
			{
				fread((void*)verfbuf, sizeof(float), 3, m_pStream);

				m_ObjList[iObj]->vblistSkin[iV].tan.x = verfbuf[0];
				m_ObjList[iObj]->vblistSkin[iV].tan.y = verfbuf[1];
				m_ObjList[iObj]->vblistSkin[iV].tan.z = verfbuf[2];
			}

			for (int iV = 0;
				iV < m_ObjList[iObj]->iblistSkin.size(); iV++)
			{
				int indexbuf[1] = { 0, };
				fread((void*)indexbuf, sizeof(int), 1, m_pStream);

				m_ObjList[iObj]->iblistSkin[iV] = indexbuf[0];
			}
		}

		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->posTrack.size(); iTrack++)
		{
			fread((void*)verfbuf, sizeof(float), 4, m_pStream);
			m_ObjList[iObj]->posTrack[iTrack].iTick = verfbuf[0];
			m_ObjList[iObj]->posTrack[iTrack].p.x = verfbuf[1];
			m_ObjList[iObj]->posTrack[iTrack].p.y = verfbuf[2];
			m_ObjList[iObj]->posTrack[iTrack].p.z = verfbuf[3];
		}

		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->rotTrack.size(); iTrack++)
		{
			fread((void*)verfbuf, sizeof(float), 5, m_pStream);
			m_ObjList[iObj]->rotTrack[iTrack].iTick = verfbuf[0];
			m_ObjList[iObj]->rotTrack[iTrack].q.x = verfbuf[1];
			m_ObjList[iObj]->rotTrack[iTrack].q.y = verfbuf[2];
			m_ObjList[iObj]->rotTrack[iTrack].q.z = verfbuf[3];
			m_ObjList[iObj]->rotTrack[iTrack].q.w = verfbuf[4];
		}
		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->sclTrack.size(); iTrack++)
		{
			fread((void*)verfbuf, sizeof(float), 8, m_pStream);
			m_ObjList[iObj]->sclTrack[iTrack].iTick = verfbuf[0];

			m_ObjList[iObj]->sclTrack[iTrack].p.x = verfbuf[1];
			m_ObjList[iObj]->sclTrack[iTrack].p.y = verfbuf[2];
			m_ObjList[iObj]->sclTrack[iTrack].p.z = verfbuf[3];

			m_ObjList[iObj]->sclTrack[iTrack].q.x = verfbuf[4];
			m_ObjList[iObj]->sclTrack[iTrack].q.y = verfbuf[5];
			m_ObjList[iObj]->sclTrack[iTrack].q.z = verfbuf[6];
			m_ObjList[iObj]->sclTrack[iTrack].q.w = verfbuf[7];
		}

		fread((void*)verfbuf, sizeof(float), 10, m_pStream);

		m_ObjList[iObj]->m_CharBox.vCenter.x = verfbuf[1];
		m_ObjList[iObj]->m_CharBox.vCenter.y = verfbuf[2];
		m_ObjList[iObj]->m_CharBox.vCenter.z = verfbuf[3];

		m_ObjList[iObj]->m_CharBox.vMax.x = verfbuf[4];
		m_ObjList[iObj]->m_CharBox.vMax.y = verfbuf[5];
		m_ObjList[iObj]->m_CharBox.vMax.z = verfbuf[6];
		m_ObjList[iObj]->m_CharBox.vMin.x = verfbuf[7];
		m_ObjList[iObj]->m_CharBox.vMin.y = verfbuf[8];
		m_ObjList[iObj]->m_CharBox.vMin.z = verfbuf[9];
	}

	int objsize[1];
	fread((void*)objsize, sizeof(int), 1, m_pStream);
	m_matBoneBindPoss.resize(objsize[0]);

	for (int ibone = 0; ibone < m_matBoneBindPoss.size(); ibone++)
	{
		float fbone[16];
		fread((void*)fbone, sizeof(float), 16, m_pStream);

		m_matBoneBindPoss[ibone]._11 = fbone[0];
		m_matBoneBindPoss[ibone]._12 = fbone[1];
		m_matBoneBindPoss[ibone]._13 = fbone[2];
		m_matBoneBindPoss[ibone]._14 = fbone[3];
		m_matBoneBindPoss[ibone]._21 = fbone[4];
		m_matBoneBindPoss[ibone]._22 = fbone[5];
		m_matBoneBindPoss[ibone]._23 = fbone[6];
		m_matBoneBindPoss[ibone]._24 = fbone[7];
		m_matBoneBindPoss[ibone]._31 = fbone[8];
		m_matBoneBindPoss[ibone]._32 = fbone[9];
		m_matBoneBindPoss[ibone]._33 = fbone[10];
		m_matBoneBindPoss[ibone]._34 = fbone[11];
		m_matBoneBindPoss[ibone]._41 = fbone[12];
		m_matBoneBindPoss[ibone]._42 = fbone[13];
		m_matBoneBindPoss[ibone]._43 = fbone[14];
		m_matBoneBindPoss[ibone]._44 = fbone[15];
	}
	fclose(m_pStream);

	return true;
}
bool  CBY_Parser::MtrLoad(const TCHAR* szFileName)
{
	if (OpenStream(szFileName) == FALSE) return false;

	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %d %d %d %d %d %d"),
		m_pString,
		&m_Scene.iFirstFrame,
		&m_Scene.iLastFrame,
		&m_Scene.iFrameSpeed,
		&m_Scene.iTickPerFrame,
		&m_Scene.iNumMaterial,
		&m_Scene.iNumMesh);

	int iTemp = 0;
	int iNumSubMtrl = 0;
	int iNumSubTexmap = 0;

	m_MtlList.resize(m_Scene.iNumMaterial);
	for (int iMtrl = 0; iMtrl < m_Scene.iNumMaterial; iMtrl++)
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s %d %d"), m_pString, &iTemp, &iNumSubMtrl);

		if (iNumSubMtrl > 0)
		{
			m_MtlList[iMtrl].SubMaterial.resize(iNumSubMtrl);
			for (int iSubMtrl = 0;
				iSubMtrl < iNumSubMtrl; iSubMtrl++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s%d%d"),
					m_pString,
					&iTemp,
					&iNumSubTexmap);
				m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList.resize(iNumSubTexmap);

				for (int iMap = 0;
					iMap < iNumSubTexmap; iMap++)
				{
					_fgetts(m_pBuffer, 256, m_pStream);
					_stscanf(m_pBuffer, _T("%s %d %d %s\n"), m_pString, &iTemp, &m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].iSubNo,
						m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].szTextureName);

				}
			}
		}
		else
		{
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s %d %d"), m_pString, &iTemp, &iNumSubTexmap);
			m_MtlList[iMtrl].texList.resize(iNumSubTexmap);
			for (int iMap = 0;
				iMap < iNumSubTexmap; iMap++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s %d %d %s"),
					m_pString,
					&iTemp,
					&m_MtlList[iMtrl].texList[iMap].iSubNo,
					m_MtlList[iMtrl].texList[iMap].szTextureName);

			}
		}
	}

	m_ObjList.resize(m_Scene.iNumMesh);
	int iNumArray[8];
	int iNumCnt = 0;
	for (int iObj = 0; iObj < m_Scene.iNumMesh; iObj++)
	{
		m_ObjList[iObj] = new CBY_MeshSkin;
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s %d %d %d %d %d %d %d %d %d"),
			m_pString,
			&iTemp,
			&iNumArray[0],
			&iNumArray[1],
			&iNumArray[2],
			&iNumArray[3],
			&iNumArray[4],
			&iNumArray[5],
			&iNumArray[6],
			&iNumArray[7]
		);
		m_ObjList[iObj]->listSkin.resize(iNumArray[0]);
		m_ObjList[iObj]->m_iTexIndex = iNumArray[1];
		m_ObjList[iObj]->posTrack.resize(iNumArray[2]);
		m_ObjList[iObj]->rotTrack.resize(iNumArray[3]);
		m_ObjList[iObj]->sclTrack.resize(iNumArray[4]);
		m_ObjList[iObj]->subMeshSkin.resize(iNumArray[5]);

		m_Bone.push_back(0);
		m_Root.push_back(0);
		if (iNumArray[6] == 1)
		{
			m_ObjList[iObj]->m_bBone = true;
			m_Bone[iObj] = 1;
		}
		if (iNumArray[7] == 1)
		{
			m_ObjList[iObj]->m_bRoot = true;
			m_Root[iObj] = 1;
		}

		_fgetts(m_pBuffer, 256, m_pStream);
		TCHAR szName[255] = { 0, };
		TCHAR szParentName[255] = { 0, };
		_stscanf(m_pBuffer, _T("%s %s %s"),
			m_pString,
			szName,
			szParentName);
		m_ObjList[iObj]->m_szName = wtm(szName);
		m_ObjList[iObj]->m_szParentName = wtm(szParentName);

		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"),
			m_pString,
			&m_ObjList[iObj]->m_matWorld._11,
			&m_ObjList[iObj]->m_matWorld._12,
			&m_ObjList[iObj]->m_matWorld._13,
			&m_ObjList[iObj]->m_matWorld._14,
			&m_ObjList[iObj]->m_matWorld._21,
			&m_ObjList[iObj]->m_matWorld._22,
			&m_ObjList[iObj]->m_matWorld._23,
			&m_ObjList[iObj]->m_matWorld._24,
			&m_ObjList[iObj]->m_matWorld._31,
			&m_ObjList[iObj]->m_matWorld._32,
			&m_ObjList[iObj]->m_matWorld._33,
			&m_ObjList[iObj]->m_matWorld._34,
			&m_ObjList[iObj]->m_matWorld._41,
			&m_ObjList[iObj]->m_matWorld._42,
			&m_ObjList[iObj]->m_matWorld._43,
			&m_ObjList[iObj]->m_matWorld._44);

		// animation
		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->posTrack.size(); iTrack++)
		{
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s %d %f %f %f"),
				m_pString,
				&m_ObjList[iObj]->posTrack[iTrack].iTick,
				&m_ObjList[iObj]->posTrack[iTrack].p.x,
				&m_ObjList[iObj]->posTrack[iTrack].p.y,
				&m_ObjList[iObj]->posTrack[iTrack].p.z);
		}
		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->rotTrack.size(); iTrack++)
		{
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s %d %f %f %f %f"),
				m_pString,
				&m_ObjList[iObj]->rotTrack[iTrack].iTick,
				&m_ObjList[iObj]->rotTrack[iTrack].q.x,
				&m_ObjList[iObj]->rotTrack[iTrack].q.y,
				&m_ObjList[iObj]->rotTrack[iTrack].q.z,
				&m_ObjList[iObj]->rotTrack[iTrack].q.w);
		}
		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->sclTrack.size(); iTrack++)
		{
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s %d %f %f %f %f %f %f %f"),
				m_pString,
				&m_ObjList[iObj]->sclTrack[iTrack].iTick,

				&m_ObjList[iObj]->sclTrack[iTrack].p.x,
				&m_ObjList[iObj]->sclTrack[iTrack].p.y,
				&m_ObjList[iObj]->sclTrack[iTrack].p.z,

				&m_ObjList[iObj]->sclTrack[iTrack].q.x,
				&m_ObjList[iObj]->sclTrack[iTrack].q.y,
				&m_ObjList[iObj]->sclTrack[iTrack].q.z,
				&m_ObjList[iObj]->sclTrack[iTrack].q.w);
		}

		if (m_ObjList[iObj]->m_bBone)
		{
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s %d %f %f %f %f %f %f %f %f %f"),
				m_pString,
				&iTemp,
				&m_ObjList[iObj]->m_Bone.m_vPos.x,
				&m_ObjList[iObj]->m_Bone.m_vPos.y,
				&m_ObjList[iObj]->m_Bone.m_vPos.z,

				&m_ObjList[iObj]->m_Bone.m_Box.vMax.x,
				&m_ObjList[iObj]->m_Bone.m_Box.vMax.y,
				&m_ObjList[iObj]->m_Bone.m_Box.vMax.z,

				&m_ObjList[iObj]->m_Bone.m_Box.vMin.x,
				&m_ObjList[iObj]->m_Bone.m_Box.vMin.y,
				&m_ObjList[iObj]->m_Bone.m_Box.vMin.z);
		}
	}

	int iBoneCount = 0;
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %d"), m_pString, &iBoneCount);
	D3DXMATRIX mat;
	for (int ibone = 0; ibone < iBoneCount; ibone++)
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"),
			&mat._11,
			&mat._12,
			&mat._13,
			&mat._14,
			&mat._21,
			&mat._22,
			&mat._23,
			&mat._24,
			&mat._31,
			&mat._32,
			&mat._33,
			&mat._34,
			&mat._41,
			&mat._42,
			&mat._43,
			&mat._44);

		m_matBoneBindPoss.push_back(mat);
	}

	CloseStream();
}
bool  CBY_Parser::MtrTextToBinary()
{
	TCHAR  szFile[256] = { 0, };
	TCHAR* pszname;

	pszname = SaveFileDlg(_T("mtr"), _T("Export"));
	if (pszname == NULL)
	{
		return false;
	}

	_tcscpy(szFile, pszname);


	FILE* fp;
	fp = _tfopen(szFile, _T("wb"));

	int ibuf[6] = { m_Scene.iFirstFrame, m_Scene.iLastFrame,
		m_Scene.iFrameSpeed,m_Scene.iTickPerFrame, m_Scene.iNumMaterial, m_Scene.iNumMesh };
	TCHAR cbuf[256];
	fwrite((void*)ibuf, sizeof(int), 6, fp);

	for (int iMtrl = 0; iMtrl < m_Scene.iNumMaterial; iMtrl++)
	{
		ibuf[0] = iMtrl;
		ibuf[1] = m_MtlList[iMtrl].SubMaterial.size();
		fwrite((void*)ibuf, sizeof(int), 2, fp);

		if (m_MtlList[iMtrl].SubMaterial.size() > 0)
		{
			for (int iSubMtrl = 0; iSubMtrl < m_MtlList[iMtrl].SubMaterial.size(); iSubMtrl++)
			{
				ibuf[0] = iSubMtrl;
				ibuf[1] = m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList.size();
				fwrite((void*)ibuf, sizeof(int), 2, fp);

				for (int iMap = 0; iMap < m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList.size(); iMap++)
				{
					ibuf[0] = iMap;
					ibuf[1] = m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].iSubNo;
					int length[1] = { _tcslen(m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].szTextureName) };
					_tcscpy_s(cbuf, m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].szTextureName);
					fwrite((void*)ibuf, sizeof(int), 2, fp);
					fwrite((void*)length, sizeof(int), 1, fp);
					fwrite((void*)cbuf, sizeof(TCHAR), length[0], fp);
				}
			}
		}
		else
		{
			ibuf[0] = iMtrl;
			ibuf[1] = m_MtlList[iMtrl].texList.size();
			fwrite((void*)ibuf, sizeof(int), 2, fp);
			for (int iMap = 0; iMap < m_MtlList[iMtrl].texList.size(); iMap++)
			{
				ibuf[0] = iMap;
				ibuf[1] = m_MtlList[iMtrl].texList[iMap].iSubNo;
				int length[1] = { _tcslen(m_MtlList[iMtrl].texList[iMap].szTextureName) };
				_tcscpy_s(cbuf, m_MtlList[iMtrl].texList[iMap].szTextureName);
				fwrite((void*)ibuf, sizeof(int), 2, fp);
				fwrite((void*)length, sizeof(int), 1, fp);
				fwrite((void*)cbuf, sizeof(TCHAR), length[0], fp);
			}
		}
	}

	for (int iObj = 0; iObj < m_Scene.iNumMesh; iObj++)
	{
		int objibuf[8] = { m_ObjList[iObj]->listSkin.size(), m_ObjList[iObj]->m_iTexIndex, m_ObjList[iObj]->posTrack.size(),
		m_ObjList[iObj]->rotTrack.size(), m_ObjList[iObj]->sclTrack.size(), m_ObjList[iObj]->subMeshSkin.size(),
		m_Bone[iObj], m_Root[iObj] };
		fwrite((void*)objibuf, sizeof(int), 8, fp);

		std::wstring str;
		str = mtw(m_ObjList[iObj]->m_szName);
		_tcscpy_s(cbuf, str.c_str());
		int length[1] = { str.length() };
		fwrite((void*)length, sizeof(int), 1, fp);
		fwrite((void*)cbuf, sizeof(TCHAR), length[0], fp);

		str = mtw(m_ObjList[iObj]->m_szParentName);
		_tcscpy_s(cbuf, str.c_str());
		length[0] = str.length();
		fwrite((void*)length, sizeof(int), 1, fp);
		fwrite((void*)cbuf, sizeof(TCHAR), length[0], fp);

		float fBuf[16] = {
			m_ObjList[iObj]->m_matWorld._11,
			m_ObjList[iObj]->m_matWorld._12,
			m_ObjList[iObj]->m_matWorld._13,
			m_ObjList[iObj]->m_matWorld._14,
			m_ObjList[iObj]->m_matWorld._21,
			m_ObjList[iObj]->m_matWorld._22,
			m_ObjList[iObj]->m_matWorld._23,
			m_ObjList[iObj]->m_matWorld._24,
			m_ObjList[iObj]->m_matWorld._31,
			m_ObjList[iObj]->m_matWorld._32,
			m_ObjList[iObj]->m_matWorld._33,
			m_ObjList[iObj]->m_matWorld._34,
			m_ObjList[iObj]->m_matWorld._41,
			m_ObjList[iObj]->m_matWorld._42,
			m_ObjList[iObj]->m_matWorld._43,
			m_ObjList[iObj]->m_matWorld._44
		};
		fwrite((void*)fBuf, sizeof(float), 16, fp);

		float verfbuf[10];

		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->posTrack.size(); iTrack++)
		{

			verfbuf[0] = m_ObjList[iObj]->posTrack[iTrack].iTick;
			verfbuf[1] = m_ObjList[iObj]->posTrack[iTrack].p.x;
			verfbuf[2] = m_ObjList[iObj]->posTrack[iTrack].p.y;
			verfbuf[3] = m_ObjList[iObj]->posTrack[iTrack].p.z;

			fwrite((void*)verfbuf, sizeof(float), 4, fp);
		}

		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->rotTrack.size(); iTrack++)
		{
			verfbuf[0] = m_ObjList[iObj]->rotTrack[iTrack].iTick;
			verfbuf[1] = m_ObjList[iObj]->rotTrack[iTrack].q.x;
			verfbuf[2] = m_ObjList[iObj]->rotTrack[iTrack].q.y;
			verfbuf[3] = m_ObjList[iObj]->rotTrack[iTrack].q.z;
			verfbuf[4] = m_ObjList[iObj]->rotTrack[iTrack].q.w;

			fwrite((void*)verfbuf, sizeof(float), 5, fp);
		}
		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->sclTrack.size(); iTrack++)
		{
			verfbuf[0] = m_ObjList[iObj]->sclTrack[iTrack].iTick;

			verfbuf[1] = m_ObjList[iObj]->sclTrack[iTrack].p.x;
			verfbuf[2] = m_ObjList[iObj]->sclTrack[iTrack].p.y;
			verfbuf[3] = m_ObjList[iObj]->sclTrack[iTrack].p.z;

			verfbuf[4] = m_ObjList[iObj]->sclTrack[iTrack].q.x;
			verfbuf[5] = m_ObjList[iObj]->sclTrack[iTrack].q.y;
			verfbuf[6] = m_ObjList[iObj]->sclTrack[iTrack].q.z;
			verfbuf[7] = m_ObjList[iObj]->sclTrack[iTrack].q.w;

			fwrite((void*)verfbuf, sizeof(float), 8, fp);
		}

		if (m_ObjList[iObj]->m_bBone)
		{
			verfbuf[0] = iObj;
			verfbuf[1] = m_ObjList[iObj]->m_Bone.m_vPos.x;
			verfbuf[2] = m_ObjList[iObj]->m_Bone.m_vPos.y;
			verfbuf[3] = m_ObjList[iObj]->m_Bone.m_vPos.z;

			verfbuf[4] = m_ObjList[iObj]->m_Bone.m_Box.vMax.x;
			verfbuf[5] = m_ObjList[iObj]->m_Bone.m_Box.vMax.y;
			verfbuf[6] = m_ObjList[iObj]->m_Bone.m_Box.vMax.z;
			verfbuf[7] = m_ObjList[iObj]->m_Bone.m_Box.vMin.x;
			verfbuf[8] = m_ObjList[iObj]->m_Bone.m_Box.vMin.y;
			verfbuf[9] = m_ObjList[iObj]->m_Bone.m_Box.vMin.z;
			fwrite((void*)verfbuf, sizeof(float), 10, fp);
		}
	}

	int objsize[1] = { m_matBoneBindPoss.size() };
	fwrite((void*)objsize, sizeof(int), 1, fp);

	for (int ibone = 0; ibone < m_matBoneBindPoss.size(); ibone++)
	{
		float fbone[16];
		fbone[0] = m_matBoneBindPoss[ibone]._11;
		fbone[1] = m_matBoneBindPoss[ibone]._12;
		fbone[2] = m_matBoneBindPoss[ibone]._13;
		fbone[3] = m_matBoneBindPoss[ibone]._14;
		fbone[4] = m_matBoneBindPoss[ibone]._21;
		fbone[5] = m_matBoneBindPoss[ibone]._22;
		fbone[6] = m_matBoneBindPoss[ibone]._23;
		fbone[7] = m_matBoneBindPoss[ibone]._24;
		fbone[8] = m_matBoneBindPoss[ibone]._31;
		fbone[9] = m_matBoneBindPoss[ibone]._32;
		fbone[10] = m_matBoneBindPoss[ibone]._33;
		fbone[11] = m_matBoneBindPoss[ibone]._34;
		fbone[12] = m_matBoneBindPoss[ibone]._41;
		fbone[13] = m_matBoneBindPoss[ibone]._42;
		fbone[14] = m_matBoneBindPoss[ibone]._43;
		fbone[15] = m_matBoneBindPoss[ibone]._44;

		fwrite((void*)fbone, sizeof(float), 16, fp);
	}
	fclose(fp);
	return true;
}
bool  CBY_Parser::MtrBinaryLoad(const TCHAR* szFileName)
{
	m_pStream = _tfopen(szFileName, _T("rb"));
	if (m_pStream == nullptr)
	{
		return false;
	}

	int ibuf[6] = { 0, };
	fread((void*)ibuf, sizeof(int), 6, m_pStream);
	m_Scene.iFirstFrame = ibuf[0];
	m_Scene.iLastFrame = ibuf[1];
	m_Scene.iFrameSpeed = ibuf[2];
	m_Scene.iTickPerFrame = ibuf[3];
	m_Scene.iNumMaterial = ibuf[4];
	m_Scene.iNumMesh = ibuf[5];

	m_MtlList.resize(m_Scene.iNumMaterial);

	for (int iMtrl = 0; iMtrl < m_Scene.iNumMaterial; iMtrl++)
	{
		fread((void*)ibuf, sizeof(int), 2, m_pStream);

		if (ibuf[1] > 0)
		{
			m_MtlList[iMtrl].SubMaterial.resize(ibuf[1]);
			for (int iSubMtrl = 0; iSubMtrl < m_MtlList[iMtrl].SubMaterial.size(); iSubMtrl++)
			{
				fread((void*)ibuf, sizeof(int), 2, m_pStream);
				m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList.resize(ibuf[1]);

				for (int iMap = 0; iMap < m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList.size(); iMap++)
				{
					TCHAR cbuf[256] = { 0, };
					fread((void*)ibuf, sizeof(int), 2, m_pStream);
					m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].iSubNo = ibuf[1];

					int length[1];
					fread((void*)length, sizeof(int), 1, m_pStream);

					fread((void*)cbuf, sizeof(TCHAR), length[0], m_pStream);
					_tcsncpy_s(m_MtlList[iMtrl].SubMaterial[iSubMtrl].texList[iMap].szTextureName, cbuf, sizeof(TCHAR)*length[0]);
				}
			}
		}
		else
		{
			fread((void*)ibuf, sizeof(int), 2, m_pStream);
			m_MtlList[iMtrl].texList.resize(ibuf[1]);

			for (int iMap = 0; iMap < m_MtlList[iMtrl].texList.size(); iMap++)
			{
				TCHAR cbuf[256] = { 0, };
				fread((void*)ibuf, sizeof(int), 2, m_pStream);
				m_MtlList[iMtrl].texList[iMap].iSubNo = ibuf[1];

				int length[1];
				fread((void*)length, sizeof(int), 1, m_pStream);

				fread((void*)cbuf, sizeof(TCHAR), length[0], m_pStream);
				_tcsncpy_s(m_MtlList[iMtrl].texList[iMap].szTextureName, cbuf, sizeof(TCHAR)*length[0]);
			}
		}
	}
	m_ObjList.resize(m_Scene.iNumMesh);
	for (int iObj = 0; iObj < m_Scene.iNumMesh; iObj++)
	{
		m_ObjList[iObj] = new CBY_MeshSkin;
		TCHAR cbuf[256] = { 0, };
		TCHAR cbuf2[256] = { 0, };

		int objibuf[8];
		fread((void*)objibuf, sizeof(int), 8, m_pStream);

		m_ObjList[iObj]->listSkin.resize(objibuf[0]);
		m_ObjList[iObj]->m_iTexIndex = objibuf[1];
		m_ObjList[iObj]->posTrack.resize(objibuf[2]);
		m_ObjList[iObj]->rotTrack.resize(objibuf[3]);
		m_ObjList[iObj]->sclTrack.resize(objibuf[4]);
		m_ObjList[iObj]->subMeshSkin.resize(objibuf[5]);
		
		if (objibuf[6] == 1)
		{
			m_ObjList[iObj]->m_bBone = true;
		}
		if (objibuf[7] == 1)
		{
			m_ObjList[iObj]->m_bRoot = true;
		}


		int length[1];
		fread((void*)length, sizeof(int), 1, m_pStream);

		fread((void*)cbuf, sizeof(TCHAR), length[0], m_pStream);
		m_ObjList[iObj]->m_szName = wtm(cbuf);							//자기 이름

		fread((void*)length, sizeof(int), 1, m_pStream);
		fread((void*)cbuf2, sizeof(TCHAR), length[0], m_pStream);		//부모 이름
		m_ObjList[iObj]->m_szParentName = wtm(cbuf2);

		float fBuf[16];
		fread((void*)fBuf, sizeof(float), 16, m_pStream);
		m_ObjList[iObj]->m_matWorld._11 = fBuf[0];
		m_ObjList[iObj]->m_matWorld._12 = fBuf[1];
		m_ObjList[iObj]->m_matWorld._13 = fBuf[2];
		m_ObjList[iObj]->m_matWorld._14 = fBuf[3];
		m_ObjList[iObj]->m_matWorld._21 = fBuf[4];
		m_ObjList[iObj]->m_matWorld._22 = fBuf[5];
		m_ObjList[iObj]->m_matWorld._23 = fBuf[6];
		m_ObjList[iObj]->m_matWorld._24 = fBuf[7];
		m_ObjList[iObj]->m_matWorld._31 = fBuf[8];
		m_ObjList[iObj]->m_matWorld._32 = fBuf[9];
		m_ObjList[iObj]->m_matWorld._33 = fBuf[10];
		m_ObjList[iObj]->m_matWorld._34 = fBuf[11];
		m_ObjList[iObj]->m_matWorld._41 = fBuf[12];
		m_ObjList[iObj]->m_matWorld._42 = fBuf[13];
		m_ObjList[iObj]->m_matWorld._43 = fBuf[14];
		m_ObjList[iObj]->m_matWorld._44 = fBuf[15];

		float verfbuf[10];
		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->posTrack.size(); iTrack++)
		{
			fread((void*)verfbuf, sizeof(float), 4, m_pStream);
			m_ObjList[iObj]->posTrack[iTrack].iTick = verfbuf[0];
			m_ObjList[iObj]->posTrack[iTrack].p.x = verfbuf[1];
			m_ObjList[iObj]->posTrack[iTrack].p.y = verfbuf[2];
			m_ObjList[iObj]->posTrack[iTrack].p.z = verfbuf[3];
		}

		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->rotTrack.size(); iTrack++)
		{
			fread((void*)verfbuf, sizeof(float), 5, m_pStream);
			m_ObjList[iObj]->rotTrack[iTrack].iTick = verfbuf[0];
			m_ObjList[iObj]->rotTrack[iTrack].q.x = verfbuf[1];
			m_ObjList[iObj]->rotTrack[iTrack].q.y = verfbuf[2];
			m_ObjList[iObj]->rotTrack[iTrack].q.z = verfbuf[3];
			m_ObjList[iObj]->rotTrack[iTrack].q.w = verfbuf[4];
		}
		for (int iTrack = 0;
			iTrack < m_ObjList[iObj]->sclTrack.size(); iTrack++)
		{
			fread((void*)verfbuf, sizeof(float), 8, m_pStream);
			m_ObjList[iObj]->sclTrack[iTrack].iTick = verfbuf[0];

			m_ObjList[iObj]->sclTrack[iTrack].p.x = verfbuf[1];
			m_ObjList[iObj]->sclTrack[iTrack].p.y = verfbuf[2];
			m_ObjList[iObj]->sclTrack[iTrack].p.z = verfbuf[3];

			m_ObjList[iObj]->sclTrack[iTrack].q.x = verfbuf[4];
			m_ObjList[iObj]->sclTrack[iTrack].q.y = verfbuf[5];
			m_ObjList[iObj]->sclTrack[iTrack].q.z = verfbuf[6];
			m_ObjList[iObj]->sclTrack[iTrack].q.w = verfbuf[7];
		}

		if (m_ObjList[iObj]->m_bBone)
		{
			fread((void*)verfbuf, sizeof(float), 10, m_pStream);
			iObj= verfbuf[0];
			m_ObjList[iObj]->m_Bone.m_vPos.x = verfbuf[1];
			m_ObjList[iObj]->m_Bone.m_vPos.y = verfbuf[2];
			m_ObjList[iObj]->m_Bone.m_vPos.z = verfbuf[3];

			m_ObjList[iObj]->m_Bone.m_Box.vMax.x = verfbuf[4];
			m_ObjList[iObj]->m_Bone.m_Box.vMax.y = verfbuf[5];
			m_ObjList[iObj]->m_Bone.m_Box.vMax.z = verfbuf[6];
			m_ObjList[iObj]->m_Bone.m_Box.vMin.x = verfbuf[7];
			m_ObjList[iObj]->m_Bone.m_Box.vMin.y = verfbuf[8];
			m_ObjList[iObj]->m_Bone.m_Box.vMin.z = verfbuf[9];
		}
	}

	int objsize[1];
	fread((void*)objsize, sizeof(int), 1, m_pStream);
	m_matBoneBindPoss.resize(objsize[0]);

	for (int ibone = 0; ibone < m_matBoneBindPoss.size(); ibone++)
	{
		float fbone[16];
		fread((void*)fbone, sizeof(float), 16, m_pStream);

		m_matBoneBindPoss[ibone]._11 = fbone[0];
		m_matBoneBindPoss[ibone]._12 = fbone[1];
		m_matBoneBindPoss[ibone]._13 = fbone[2];
		m_matBoneBindPoss[ibone]._14 = fbone[3];
		m_matBoneBindPoss[ibone]._21 = fbone[4];
		m_matBoneBindPoss[ibone]._22 = fbone[5];
		m_matBoneBindPoss[ibone]._23 = fbone[6];
		m_matBoneBindPoss[ibone]._24 = fbone[7];
		m_matBoneBindPoss[ibone]._31 = fbone[8];
		m_matBoneBindPoss[ibone]._32 = fbone[9];
		m_matBoneBindPoss[ibone]._33 = fbone[10];
		m_matBoneBindPoss[ibone]._34 = fbone[11];
		m_matBoneBindPoss[ibone]._41 = fbone[12];
		m_matBoneBindPoss[ibone]._42 = fbone[13];
		m_matBoneBindPoss[ibone]._43 = fbone[14];
		m_matBoneBindPoss[ibone]._44 = fbone[15];
	}
	fclose(m_pStream);

	return true;
}

void	CBY_Parser::CreateVIData(CBY_MeshSkin* mesh)
{
	HRESULT hr = S_OK;
	mesh->iblistSkin.clear();
	if (mesh->listSkin.size() <= 0)
	{
		return;
	}
	mesh->vblistSkin.reserve(mesh->listSkin.size());
	for (int iVertex = 0; iVertex < mesh->listSkin.size(); iVertex++)
	{
		int iPos = -1;
		for (int i = 0; i < mesh->vblistSkin.size(); i++)
		{
			if (mesh->vblistSkin[i] == mesh->listSkin[iVertex])
			{
				iPos = i;
				break;
			}
		}
		if (iPos < 0)
		{
			mesh->vblistSkin.push_back(mesh->listSkin[iVertex]);
			iPos = mesh->vblistSkin.size() - 1;
		}
		mesh->iblistSkin.push_back(iPos);
	}
}

void CBY_Parser::VertexDataTan(CBY_MeshSkin* mesh)
{
	D3DXVECTOR3 vTangent, vBiNormal, vNormal;

	int iIndex = 0;
	int i0, i1, i2, i3, i4, i5;

	for (int iIndex = 0; iIndex < mesh->iblistSkin.size(); iIndex += 3)
	{
		i0 = mesh->iblistSkin[iIndex + 0];
		i1 = mesh->iblistSkin[iIndex + 1];
		i2 = mesh->iblistSkin[iIndex + 2];

		CreateTangentSpaceVectors(&mesh->vblistSkin[i0].p, &mesh->vblistSkin[i1].p, &mesh->vblistSkin[i2].p,
			mesh->vblistSkin[i0].t, mesh->vblistSkin[i1].t, mesh->vblistSkin[i2].t,
			&vTangent, &vBiNormal, &mesh->vblistSkin[i0].n);

		mesh->vblistSkin[i0].tan += vTangent;

		i0 = mesh->iblistSkin[iIndex + 1];
		i1 = mesh->iblistSkin[iIndex + 2];
		i2 = mesh->iblistSkin[iIndex + 0];

		CreateTangentSpaceVectors(&mesh->vblistSkin[i0].p, &mesh->vblistSkin[i1].p, &mesh->vblistSkin[i2].p,
			mesh->vblistSkin[i0].t, mesh->vblistSkin[i1].t, mesh->vblistSkin[i2].t,
			&vTangent, &vBiNormal, &mesh->vblistSkin[i0].n);

		mesh->vblistSkin[i0].tan += vTangent;

		i0 = mesh->iblistSkin[iIndex + 2];
		i1 = mesh->iblistSkin[iIndex + 0];
		i2 = mesh->iblistSkin[iIndex + 1];

		CreateTangentSpaceVectors(&mesh->vblistSkin[i0].p, &mesh->vblistSkin[i1].p, &mesh->vblistSkin[i2].p,
			mesh->vblistSkin[i0].t, mesh->vblistSkin[i1].t, mesh->vblistSkin[i2].t,
			&vTangent, &vBiNormal, &mesh->vblistSkin[i0].n);

		mesh->vblistSkin[i0].tan += vTangent;

	}
	for (int i = 0; i < mesh->vblistSkin.size(); i++)
	{
		D3DXVec3Normalize(&mesh->vblistSkin[i].tan, &mesh->vblistSkin[i].tan);
	}
}

void CBY_Parser::CreateTangentSpaceVectors(D3DXVECTOR3 *v0,
	D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR2 uv0,
	D3DXVECTOR2 uv1, D3DXVECTOR2 uv2,
	D3DXVECTOR3 *vTangent, D3DXVECTOR3 *vBiNormal, D3DXVECTOR3 *vNormal)
{
	D3DXVECTOR3 vEdge1 = *v1 - *v0;
	D3DXVECTOR3 vEdge2 = *v2 - *v0;
	D3DXVec3Normalize(&vEdge1, &vEdge1);
	D3DXVec3Normalize(&vEdge2, &vEdge2);
	// UV delta
	D3DXVECTOR2 deltaUV1 = uv1 - uv0;
	D3DXVECTOR2 deltaUV2 = uv2 - uv0;
	D3DXVec2Normalize(&deltaUV1, &deltaUV1);
	D3DXVec2Normalize(&deltaUV2, &deltaUV2);


	D3DXVECTOR3 biNormal;
	float fDet = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	if (fabsf(fDet) < 1e-6f)
	{
		*vTangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		biNormal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		*vTangent = (vEdge1 * deltaUV2.y - vEdge2 * deltaUV1.y)*fDet;
		biNormal = (vEdge2 * deltaUV1.x - vEdge1 * deltaUV2.x)*fDet;
	}
	D3DXVec3Normalize(vTangent, vTangent);
	D3DXVec3Normalize(&biNormal, &biNormal);

	D3DXVec3Cross(vBiNormal, vNormal, vTangent);
	float crossinv = (D3DXVec3Dot(vBiNormal, &biNormal) < 0.0f) ? -1.0f : 1.0f;
	*vBiNormal *= crossinv;
}

void    CBY_Parser::Convert()
{
	for (int iObj = 0; iObj < m_ObjList.size(); iObj++)
	{
		CBY_MeshSkin* mesh = m_ObjList[iObj];
		if (mesh->subMeshSkin.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh < mesh->subMeshSkin.size(); iSubMesh++)
			{
				mesh->subMeshSkin[iSubMesh].m_iNumVertex =
					mesh->subMeshSkin[iSubMesh].listSkin.size();
				CreateVIData(&mesh->subMeshSkin[iSubMesh]);
				VertexDataTan(&mesh->subMeshSkin[iSubMesh]);
			}
		}
		else
		{
			CreateVIData(mesh);
			VertexDataTan(mesh);
		}
	}
}

BOOL  CBY_Parser::OpenStream(const TCHAR* filename)
{
	m_pStream = _tfopen(filename, _T("rt"));
	if (m_pStream == nullptr)
	{
		return FALSE;
	}
	return TRUE;
}
BOOL  CBY_Parser::CloseStream()
{
	fclose(m_pStream);
	return TRUE;
}

bool CBY_Parser::Release()
{
	for (int i = 0; i < m_ObjList.size(); i++)
	{
		delete m_ObjList[i];
	}
	m_matBoneBindPoss.clear();
	m_MtlList.clear();
	return true;
}

void CBY_Parser::Clear()
{
	m_pStream=nullptr;
	m_pBuffer[256] = { 0, };
	m_pString[256] = { 0, };
	m_iTemp=0;
	for (int i = 0; i < m_ObjList.size(); i++)
	{
		delete m_ObjList[i];
	}
	m_ObjList.clear();
	m_matBoneBindPoss.clear();
	m_MtlList.clear();
	m_Bone.clear();
	m_Root.clear();
}
CBY_Parser::CBY_Parser()
{
	m_pStream = nullptr;
}
CBY_Parser::~CBY_Parser()
{
}

TCHAR* CBY_Parser::SaveFileDlg(TCHAR* szExt, TCHAR* szTitle)
{
	OPENFILENAME    ofn;
	TCHAR            szFile[256], szFileTitle[256];
	static TCHAR     *szFilter;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	_tcscpy_s(szFile, _T("*."));
	_tcscat_s(szFile, szExt);
	_tcscat_s(szFile, _T("\0"));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFilter = szFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0L;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = szTitle;
	ofn.Flags = 0L;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = szExt;

	if (!GetSaveFileName(&ofn)) //GetOpenFileName
	{
		return NULL;
	}

	return szFile;
}
