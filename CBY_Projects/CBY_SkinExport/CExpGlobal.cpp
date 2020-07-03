#include "CExpGlobal.h"

void   CExpGlobal::InsertWeight(DWORD dwSrc, BYTE Bone, float fWeight)
{
	auto& bipedlist = m_BipedList[dwSrc];
	for (DWORD i = 0; i < 4; i++)
	{
		if (fWeight > bipedlist.weightList[i])
		{
			for (DWORD j = 3; j > i; j--)
			{
				bipedlist.weightList[j] = bipedlist.weightList[j - 1];
				bipedlist.idList[j] = bipedlist.idList[j - 1];
			}
			bipedlist.idList[i] = Bone;
			bipedlist.weightList[i] = fWeight;
			break;
		}
	}
}

int CExpGlobal::GetFindIndex(INode* node)
{
	for (int i = 0; i < m_pReadObjList.size(); i++)
	{
		if (m_pReadObjList[i] == node)
			return i;
	}

	return -1;
}
void CExpGlobal::GetSelectNode(INode* pNode)
{
	for (int i = 0; i < m_pReadObjList.size(); i++)
	{
		if (m_pReadObjList[i] == pNode)
		{
			m_ExpSelNodes.push_back(i);
			break;
		}
	}
}

void CExpGlobal::GetDecompAffine(TimeValue t, INode* pNode, AffineParts* ap)
{
	Matrix3 tm = pNode->GetNodeTM(t) * Inverse(pNode->GetParentTM(t));
	::decomp_affine(tm, ap);
}

void CExpGlobal::AddObject(INode* pNode)
{
	ObjectState os = pNode->EvalWorldState(m_AnimTime.Start());
	Object* pObj = os.obj;
	if (pObj != nullptr)
	{
		SClass_ID id = pObj->SuperClassID();
		switch (id)
		{
		case GEOMOBJECT_CLASS_ID:
		case HELPER_CLASS_ID:
		{
			m_pReadObjList.push_back(pNode);
		}
		break;
		}
	}
}

void CExpGlobal::AddMtl(INode* pNode)
{
	Mtl* pMtl = pNode->GetMtl();
	if (pMtl != nullptr)
	{
		bool bAdd = true;
		for (int i = 0; i < m_pReadMtlList.size(); i++)
		{
			if (m_pReadMtlList[i] == pMtl)
			{
				bAdd = false;
				break;
			}
		}
		if (bAdd)
		{
			m_pReadMtlList.push_back(pMtl);
		}
	}
}

void CExpGlobal::PreProcess(INode* pNode)
{
	if (pNode == nullptr)
	{
		return;
	}

	AddObject(pNode);
	AddMtl(pNode);

	int iNumChild = pNode->NumberOfChildren();
	for (int iChile = 0; iChile < iNumChild; iChile++)
	{
		PreProcess(pNode->GetChildNode(iChile));
	}
}

bool CExpGlobal::Init(Interface* pMax)
{
	m_pMax = pMax;
	m_AnimTime = m_pMax->GetAnimRange();
	m_pRootNode = pMax->GetRootNode();
	PreProcess(m_pRootNode);

	ZeroMemory(&m_Scene, sizeof(CScene));
	m_Scene.iFirstFrame = m_AnimTime.Start() / GetTicksPerFrame();
	m_Scene.iLastFrame = m_AnimTime.End() / GetTicksPerFrame();
	m_Scene.iFrameSpeed = GetFrameRate();
	m_Scene.iTickPerFrame = GetTicksPerFrame();
	m_Scene.iNumMesh = m_pReadObjList.size();
	m_Scene.iNumMaterial = m_pReadMtlList.size();
	return true;
}

void CExpGlobal::Release()
{
	m_pReadObjList.clear();
	m_pReadMtlList.clear();

	m_ObjectList.clear();
	m_MtlList.clear();
	m_ExpSelNodes.clear();
}

TCHAR* CExpGlobal::SaveFileDlg(TCHAR* szExt, TCHAR* szTitle)
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

bool CExpGlobal::Exporter()
{
	TCHAR  szFile[256] = { 0, };
	TCHAR* pszname;

	pszname = SaveFileDlg(_T("cby"), _T("Export"));
	if (pszname == NULL)
	{
		return false;
	}

	_tcscpy(szFile, pszname);


	FILE* fp;
	fp = _tfopen(szFile, _T("wt"));
	_ftprintf(fp, _T("%s %d %d %d %d %d %d\n"), L"*SCENE",
		m_Scene.iFirstFrame, m_Scene.iLastFrame, m_Scene.iFrameSpeed,
		m_Scene.iTickPerFrame, m_Scene.iNumMaterial, m_Scene.iNumMesh);

	for (int iMtrl = 0; iMtrl < m_Scene.iNumMaterial; iMtrl++)
	{

		_ftprintf(fp, _T("%s %d %d\n"), L"*MATERIAL", iMtrl, m_MtlList[iMtrl].subMtrl.size());
		if (m_MtlList[iMtrl].subMtrl.size() > 0)
			for (int iSubMtrl = 0; iSubMtrl < m_MtlList[iMtrl].subMtrl.size(); iSubMtrl++)
			{
				_ftprintf(fp, _T("%s %d %d\n"), L"*SUBMATERIAL", iSubMtrl, m_MtlList[iMtrl].subMtrl[iSubMtrl].texList.size());
				for (int iMap = 0; iMap < m_MtlList[iMtrl].subMtrl[iSubMtrl].texList.size(); iMap++)
				{
					_ftprintf(fp, _T("%s %d %d %s\n"), L"*TEXTURE", iMap,
						m_MtlList[iMtrl].subMtrl[iSubMtrl].texList[iMap].iSubNo,
						m_MtlList[iMtrl].subMtrl[iSubMtrl].texList[iMap].szTextureName);
				}
			}
		else
		{
			_ftprintf(fp, _T("%s %d %d\n"), L"TEXTURE", iMtrl,
				m_MtlList[iMtrl].texList.size());
			for (int iMap = 0; iMap < m_MtlList[iMtrl].texList.size(); iMap++)
			{
				_ftprintf(fp, _T("%s %d %d %s\n"), L"*TEXTURE", iMap, m_MtlList[iMtrl].texList[iMap].iSubNo, m_MtlList[iMtrl].texList[iMap].szTextureName);
			}
		}
	}

	for (int iObj = 0; iObj < m_Scene.iNumMesh; iObj++)
	{

		_ftprintf(fp, _T("%s %d %d %d %d %d %d %d %d %d\n"), L"*OBJECT", iObj,
			m_ObjectList[iObj].list.size(),
			m_ObjectList[iObj].m_iTexIndex,
			m_ObjectList[iObj].posTrack.size(),
			m_ObjectList[iObj].rotTrack.size(),
			m_ObjectList[iObj].sclTrack.size(),
			m_ObjectList[iObj].subMesh.size(),
			m_ObjectList[iObj].m_iBone,
			m_ObjectList[iObj].m_iRoot);

		_ftprintf(fp, _T("%s %s %s \n"), L"*NAME",
			m_ObjectList[iObj].m_szName,
			m_ObjectList[iObj].m_szParentName);

		_ftprintf(fp, _T("%s %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n"),
			L"*MATWORLD",
			m_ObjectList[iObj].m_matWorld._11,
			m_ObjectList[iObj].m_matWorld._12,
			m_ObjectList[iObj].m_matWorld._13,
			m_ObjectList[iObj].m_matWorld._14,
			m_ObjectList[iObj].m_matWorld._21,
			m_ObjectList[iObj].m_matWorld._22,
			m_ObjectList[iObj].m_matWorld._23,
			m_ObjectList[iObj].m_matWorld._24,
			m_ObjectList[iObj].m_matWorld._31,
			m_ObjectList[iObj].m_matWorld._32,
			m_ObjectList[iObj].m_matWorld._33,
			m_ObjectList[iObj].m_matWorld._34,
			m_ObjectList[iObj].m_matWorld._41,
			m_ObjectList[iObj].m_matWorld._42,
			m_ObjectList[iObj].m_matWorld._43,
			m_ObjectList[iObj].m_matWorld._44);


		if (m_ObjectList[iObj].subMesh.size() > 0)
		{

			for (int isub = 0; isub < m_ObjectList[iObj].subMesh.size(); isub++)
			{
				_ftprintf(fp, _T("%s %d %d %d %d %d %d \n"), L"*SUBMESH", iObj,
					m_ObjectList[iObj].subMesh[isub].list.size(),
					m_ObjectList[iObj].subMesh[isub].m_iTexIndex,
					m_ObjectList[iObj].subMesh[isub].posTrack.size(),
					m_ObjectList[iObj].subMesh[isub].rotTrack.size(),
					m_ObjectList[iObj].subMesh[isub].sclTrack.size()
					);

				for (int iV = 0; iV < m_ObjectList[iObj].subMesh[isub].list.size(); iV++)
				{
					_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f\n"), L"*VERTEX_P",
						m_ObjectList[iObj].subMesh[isub].list[iV].p.x,
						m_ObjectList[iObj].subMesh[isub].list[iV].p.y,
						m_ObjectList[iObj].subMesh[isub].list[iV].p.z);
				}

				
				for (int iV = 0; iV < m_ObjectList[iObj].subMesh[isub].list.size(); iV++)
				{
					_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f\n"), L"*VERTEX_N",
						m_ObjectList[iObj].subMesh[isub].list[iV].n.x,
						m_ObjectList[iObj].subMesh[isub].list[iV].n.y,
						m_ObjectList[iObj].subMesh[isub].list[iV].n.z);
				}

				
				for (int iV = 0; iV < m_ObjectList[iObj].subMesh[isub].list.size(); iV++)
				{
					_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f %10.4f\n"), L"*VERTEX_C",
						m_ObjectList[iObj].subMesh[isub].list[iV].c.x,
						m_ObjectList[iObj].subMesh[isub].list[iV].c.y,
						m_ObjectList[iObj].subMesh[isub].list[iV].c.z,
						m_ObjectList[iObj].subMesh[isub].list[iV].c.w);
				}

				
				for (int iV = 0; iV < m_ObjectList[iObj].subMesh[isub].list.size(); iV++)
				{
					_ftprintf(fp, _T("\t%s %10.4f %10.4f\n"), L"*VERTEX_T",
						m_ObjectList[iObj].subMesh[isub].list[iV].t.x,
						m_ObjectList[iObj].subMesh[isub].list[iV].t.y);
				}

				for (int iV = 0; iV < m_ObjectList[iObj].subMesh[isub].list.size(); iV++)
				{
					_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f %10.4f\n"), L"*VERTEX_I",
						m_ObjectList[iObj].subMesh[isub].list[iV].i[0],
						m_ObjectList[iObj].subMesh[isub].list[iV].i[1],
						m_ObjectList[iObj].subMesh[isub].list[iV].i[2],
						m_ObjectList[iObj].subMesh[isub].list[iV].i[3]);
				}

				for (int iV = 0; iV < m_ObjectList[iObj].subMesh[isub].list.size(); iV++)
				{
					_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f %10.4f\n"), L"*VERTEX_W",
						m_ObjectList[iObj].subMesh[isub].list[iV].w[0],
						m_ObjectList[iObj].subMesh[isub].list[iV].w[1],
						m_ObjectList[iObj].subMesh[isub].list[iV].w[2],
						m_ObjectList[iObj].subMesh[isub].list[iV].w[3]);
				}
			}
		}
		else
		{
			

			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f\n"), L"*VERTEX_P",
					m_ObjectList[iObj].list[iV].p.x,
					m_ObjectList[iObj].list[iV].p.y,
					m_ObjectList[iObj].list[iV].p.z);
			}

			
			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f\n"), L"*VERTEX_N",
					m_ObjectList[iObj].list[iV].n.x,
					m_ObjectList[iObj].list[iV].n.y,
					m_ObjectList[iObj].list[iV].n.z);
			}

			
			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f %10.4f\n"), L"*VERTEX_C",
					m_ObjectList[iObj].list[iV].c.x,
					m_ObjectList[iObj].list[iV].c.y,
					m_ObjectList[iObj].list[iV].c.z,
					m_ObjectList[iObj].list[iV].c.w);
			}

			
			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f\n"), L"*VERTEX_T",
					m_ObjectList[iObj].list[iV].t.x,
					m_ObjectList[iObj].list[iV].t.y);
			}

			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f %10.4f\n"), L"*VERTEX_I",
					m_ObjectList[iObj].list[iV].i[0],
					m_ObjectList[iObj].list[iV].i[1],
					m_ObjectList[iObj].list[iV].i[2],
					m_ObjectList[iObj].list[iV].i[3]);
			}

			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f %10.4f\n"), L"*VERTEX_W",
					m_ObjectList[iObj].list[iV].w[0],
					m_ObjectList[iObj].list[iV].w[1],
					m_ObjectList[iObj].list[iV].w[2],
					m_ObjectList[iObj].list[iV].w[3]);
			}
		}

		for (int iTrack = 0; iTrack < m_ObjectList[iObj].posTrack.size(); iTrack++)
		{
			_ftprintf(fp, _T("\t\t%s %d %10.4f %10.4f %10.4f\n"), L"*ANIPOS",
				m_ObjectList[iObj].posTrack[iTrack].iTick,
				m_ObjectList[iObj].posTrack[iTrack].p.x,
				m_ObjectList[iObj].posTrack[iTrack].p.y,
				m_ObjectList[iObj].posTrack[iTrack].p.z);
		}


		for (int iTrack = 0; iTrack < m_ObjectList[iObj].rotTrack.size(); iTrack++)
		{
			_ftprintf(fp, _T("\t\t%s %d %10.4f %10.4f %10.4f %10.4f\n"), L"*ANIROT",
				m_ObjectList[iObj].rotTrack[iTrack].iTick,
				m_ObjectList[iObj].rotTrack[iTrack].q.x,
				m_ObjectList[iObj].rotTrack[iTrack].q.y,
				m_ObjectList[iObj].rotTrack[iTrack].q.z,
				m_ObjectList[iObj].rotTrack[iTrack].q.w);
		}


		for (int iTrack = 0; iTrack < m_ObjectList[iObj].sclTrack.size(); iTrack++)
		{
			_ftprintf(fp, _T("\t\t%s %d %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n"), L"*ANISCL",
				m_ObjectList[iObj].sclTrack[iTrack].iTick,

				m_ObjectList[iObj].sclTrack[iTrack].p.x,
				m_ObjectList[iObj].sclTrack[iTrack].p.y,
				m_ObjectList[iObj].sclTrack[iTrack].p.z,

				m_ObjectList[iObj].sclTrack[iTrack].q.x,
				m_ObjectList[iObj].sclTrack[iTrack].q.y,
				m_ObjectList[iObj].sclTrack[iTrack].q.z,
				m_ObjectList[iObj].sclTrack[iTrack].q.w);
		}
		


		if (m_ObjectList[iObj].m_iBone == 1)
		{
			int temp = 0;
			_ftprintf(fp, _T("\t%s %d %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n"), L"*BONEBOX",
				temp,
				m_ObjectList[iObj].m_Bone.m_Pos.x,
				m_ObjectList[iObj].m_Bone.m_Pos.y,
				m_ObjectList[iObj].m_Bone.m_Pos.z,
				m_ObjectList[iObj].m_Bone.m_Box.pmax.x,
				m_ObjectList[iObj].m_Bone.m_Box.pmax.y,
				m_ObjectList[iObj].m_Bone.m_Box.pmax.z,
				m_ObjectList[iObj].m_Bone.m_Box.pmin.x,
				m_ObjectList[iObj].m_Bone.m_Box.pmin.y,
				m_ObjectList[iObj].m_Bone.m_Box.pmin.z);
		}
		else
		{
			int temp = 0;
			_ftprintf(fp, _T("\t%s %d %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n"), L"*BOX",
				temp,
				m_ObjectList[iObj].m_CharBox.m_Pos.x,
				m_ObjectList[iObj].m_CharBox.m_Pos.y,
				m_ObjectList[iObj].m_CharBox.m_Pos.z,
				m_ObjectList[iObj].m_CharBox.m_Box.pmax.x,
				m_ObjectList[iObj].m_CharBox.m_Box.pmax.y,
				m_ObjectList[iObj].m_CharBox.m_Box.pmax.z,
				m_ObjectList[iObj].m_CharBox.m_Box.pmin.x,
				m_ObjectList[iObj].m_CharBox.m_Box.pmin.y,
				m_ObjectList[iObj].m_CharBox.m_Box.pmin.z);
		}
	}
	
	_ftprintf(fp, _T("%s %d \n"), L"BONE_COUNT", m_pReadObjList.size());
	for (int ibone = 0; ibone < m_pReadObjList.size(); ibone++)
	{
		INode* pNode = m_pReadObjList[ibone];
		Matrix3 invTm = Inverse(pNode->GetNodeTM(m_AnimTime.Start()));
		D3D_MATRIX mat;
		DumpMatrix3(&invTm, mat);
		_ftprintf(fp, _T("%10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n"),
			mat._11,
			mat._12,
			mat._13,
			mat._14,
			mat._21,
			mat._22,
			mat._23,
			mat._24,
			mat._31,
			mat._32,
			mat._33,
			mat._34,
			mat._41,
			mat._42,
			mat._43,
			mat._44);
	}
	fclose(fp);

	return true;
}

bool CExpGlobal::Convert()
{
	int iNumMtl = m_pReadMtlList.size();
	m_MtlList.resize(iNumMtl);
	for (int iMtl = 0; iMtl < iNumMtl; iMtl++)
	{
		// 서브매터리얼 판단
		int iNumSubMtrl = m_pReadMtlList[iMtl]->NumSubMtls();
		if (iNumSubMtrl > 0)//있으면
		{
			m_MtlList[iMtl].subMtrl.resize(iNumSubMtrl);
			for (int iSubMtl = 0; iSubMtl < iNumSubMtrl; iSubMtl++)
			{
				Mtl* pSubMtl = m_pReadMtlList[iMtl]->GetSubMtl(iSubMtl);
				GetTexture(pSubMtl, m_MtlList[iMtl].subMtrl[iSubMtl]);
			}
		}
		else
		{
			// 없으면
			GetTexture(m_pReadMtlList[iMtl], m_MtlList[iMtl]);
		}
	}

	m_ObjectList.resize(m_pReadObjList.size());
	for (int iObj = 0; iObj < m_pReadObjList.size(); iObj++)
	{
		Object* pObj = m_pReadObjList[iObj]->GetObjectRef();
		m_ObjectList[iObj].m_iType = GEOM_MESH;

		if (pObj && pObj->ClassID() ==
			Class_ID(BONE_CLASS_ID, 0))
		{
			m_ObjectList[iObj].m_iType = HELPER_BONE;
		}
		if (pObj && pObj->ClassID() ==
			Class_ID(DUMMY_CLASS_ID, 0))
		{
			m_ObjectList[iObj].m_iType = HELPER_DUMMY;
		}
		// GEOM OR BIPED
		Control* pControl = m_pReadObjList[iObj]->GetTMController();
		if ((pControl->ClassID() == BIPBODY_CONTROL_CLASS_ID ||
				pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID))
		{
			m_ObjectList[iObj].m_iType = BYPED;
		}

		m_ObjectList[iObj].m_szName =FixupName(m_pReadObjList[iObj]->GetName());
		// 가상의 부모 노드(sceneRoot)
		INode* pParent = m_pReadObjList[iObj]->GetParentNode();
		if (pParent && !pParent->IsRootNode())
		{
			m_ObjectList[iObj].m_szParentName =
				FixupName(pParent->GetName());
		}
		else
		{
			m_ObjectList[iObj].m_szParentName = L"null";
			m_ObjectList[iObj].m_iRoot = 1;
		}

		Matrix3 matWorld = m_pReadObjList[iObj]->GetNodeTM(m_AnimTime.Start());
		DumpMatrix3(&matWorld, m_ObjectList[iObj].m_matWorld);
		GetMesh(m_pReadObjList[iObj], &m_ObjectList[iObj]);

		BOOL bPos, bRot, bScl;
		bPos = bRot = bScl = FALSE;
		if (CheckForAnimation(m_pReadObjList[iObj], bPos, bRot, bScl))					//애니메이션이 존재하는지 체크
		{
			if (bPos)
			{
				DumpPosSample(m_pReadObjList[iObj], &m_ObjectList[iObj]);
			}

			if (bRot)
			{
				DumpRotSample(m_pReadObjList[iObj], &m_ObjectList[iObj]);
			}

			if (bScl)
			{
				DumpSclSample(m_pReadObjList[iObj], &m_ObjectList[iObj]);
			}
		}

		if (m_ObjectList[iObj].m_iType == HELPER_BONE || m_ObjectList[iObj].m_iType == HELPER_DUMMY)
		{
			m_ObjectList[iObj].m_iBone = 1;

			Object* pBoxobj = m_pReadObjList[iObj]->EvalWorldState(m_AnimTime.Start()).obj;

			Matrix3 scamat(1), mat;
			mat = m_pReadObjList[iObj]->GetObjTMAfterWSM(m_AnimTime.Start());
			
			Point3 pos = mat.GetRow(3);
			DumpPoint3(pos, m_ObjectList[iObj].m_Bone.m_Pos);

			mat = m_pReadObjList[iObj]->GetObjectTM(m_AnimTime.Start());
			AffineParts part;
			//GetDecompAffine(m_AnimTime.Start(), m_pReadObjList[iObj], &part);
			decomp_affine(mat, &part);
			ApplyScaling(scamat, ScaleValue(part.k*part.f, part.u));

			Box3 box;
			pBoxobj->GetDeformBBox(m_AnimTime.Start(), box, &scamat);
			DumpPoint3(box.pmax, m_ObjectList[iObj].m_Bone.m_Box.pmax);
			DumpPoint3(box.pmin, m_ObjectList[iObj].m_Bone.m_Box.pmin);

			m_ObjectList[iObj].m_Bone.m_Box.pmax.x = m_ObjectList[iObj].m_Bone.m_Box.pmax.x + m_ObjectList[iObj].m_Bone.m_Pos.x;
			m_ObjectList[iObj].m_Bone.m_Box.pmax.y = m_ObjectList[iObj].m_Bone.m_Box.pmax.y + m_ObjectList[iObj].m_Bone.m_Pos.y;
			m_ObjectList[iObj].m_Bone.m_Box.pmax.z = m_ObjectList[iObj].m_Bone.m_Box.pmax.z + m_ObjectList[iObj].m_Bone.m_Pos.z;

			m_ObjectList[iObj].m_Bone.m_Box.pmin.x = m_ObjectList[iObj].m_Bone.m_Pos.x + m_ObjectList[iObj].m_Bone.m_Box.pmin.x;
			m_ObjectList[iObj].m_Bone.m_Box.pmin.y = m_ObjectList[iObj].m_Bone.m_Pos.y + m_ObjectList[iObj].m_Bone.m_Box.pmin.y;
			m_ObjectList[iObj].m_Bone.m_Box.pmin.z = m_ObjectList[iObj].m_Bone.m_Pos.z + m_ObjectList[iObj].m_Bone.m_Box.pmin.z;
		}
	}
	return true;
}

void CExpGlobal::SkinExporter()
{
	TCHAR  szFile[256] = { 0, };
	TCHAR* pszname;

	pszname = SaveFileDlg(_T("skn"), _T("Export"));
	if (pszname == NULL)
	{
		return;
	}

	_tcscpy(szFile, pszname);
	

	FILE* fp;
	fp = _tfopen(szFile, _T("wt"));
	_ftprintf(fp, _T("%s %d %d %d %d %d %d\n"), L"*SCENE",
		m_Scene.iFirstFrame, m_Scene.iLastFrame, m_Scene.iFrameSpeed,
		m_Scene.iTickPerFrame, m_Scene.iNumMaterial, m_ExpSelNodes.size());

	for (int iMtrl = 0; iMtrl < m_Scene.iNumMaterial; iMtrl++)
	{
		
		_ftprintf(fp, _T("%s %d %d\n"), L"*MATERIAL", iMtrl, m_MtlList[iMtrl].subMtrl.size());
		if(m_MtlList[iMtrl].subMtrl.size() > 0)
		for (int iSubMtrl = 0; iSubMtrl < m_MtlList[iMtrl].subMtrl.size(); iSubMtrl++)
		{
			_ftprintf(fp, _T("%s %d %d\n"), L"*SUBMATERIAL", iSubMtrl, m_MtlList[iMtrl].subMtrl[iSubMtrl].texList.size());
			for (int iMap = 0; iMap < m_MtlList[iMtrl].subMtrl[iSubMtrl].texList.size(); iMap++)
			{
				_ftprintf(fp, _T("%s %d %d %s\n"), L"*TEXTURE", iMap,
					m_MtlList[iMtrl].subMtrl[iSubMtrl].texList[iMap].iSubNo,
					m_MtlList[iMtrl].subMtrl[iSubMtrl].texList[iMap].szTextureName);
			}
		}
		else
		{
			_ftprintf(fp, _T("%s %d %d\n"), L"TEXTURE", iMtrl,
				m_MtlList[iMtrl].texList.size());
			for (int iMap = 0; iMap < m_MtlList[iMtrl].texList.size(); iMap++)
			{
				_ftprintf(fp, _T("%s %d %d %s\n"), L"*TEXTURE", iMap, m_MtlList[iMtrl].texList[iMap].iSubNo, m_MtlList[iMtrl].texList[iMap].szTextureName);
			}
		}
	}

	for (int iNode = 0; iNode < m_ExpSelNodes.size(); iNode++)
	{
		int iObj = m_ExpSelNodes[iNode];

		_ftprintf(fp, _T("%s %d %d %d %d %d %d %d\n"), L"*OBJECT", iObj,
			m_ObjectList[iObj].list.size(),
			m_ObjectList[iObj].m_iTexIndex,
			m_ObjectList[iObj].posTrack.size(),
			m_ObjectList[iObj].rotTrack.size(),
			m_ObjectList[iObj].sclTrack.size(),
			m_ObjectList[iObj].subMesh.size());

		_ftprintf(fp, _T("%s %s %s \n"), L"*NAME",
			m_ObjectList[iObj].m_szName,
			m_ObjectList[iObj].m_szParentName);

		_ftprintf(fp, _T("%s %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n"),
			L"*MATWORLD",
			m_ObjectList[iObj].m_matWorld._11,
			m_ObjectList[iObj].m_matWorld._12,
			m_ObjectList[iObj].m_matWorld._13,
			m_ObjectList[iObj].m_matWorld._14,
			m_ObjectList[iObj].m_matWorld._21,
			m_ObjectList[iObj].m_matWorld._22,
			m_ObjectList[iObj].m_matWorld._23,
			m_ObjectList[iObj].m_matWorld._24,
			m_ObjectList[iObj].m_matWorld._31,
			m_ObjectList[iObj].m_matWorld._32,
			m_ObjectList[iObj].m_matWorld._33,
			m_ObjectList[iObj].m_matWorld._34,
			m_ObjectList[iObj].m_matWorld._41,
			m_ObjectList[iObj].m_matWorld._42,
			m_ObjectList[iObj].m_matWorld._43,
			m_ObjectList[iObj].m_matWorld._44);


		if (m_ObjectList[iObj].subMesh.size() > 0)
		{

			for (int isub = 0; isub < m_ObjectList[iObj].subMesh.size(); isub++)
			{
				_ftprintf(fp, _T("%s %d %d %d %d %d %d \n"), L"*SUBMESH", iObj,
					m_ObjectList[iObj].subMesh[isub].list.size(),
					m_ObjectList[iObj].subMesh[isub].m_iTexIndex,
					m_ObjectList[iObj].subMesh[isub].posTrack.size(),
					m_ObjectList[iObj].subMesh[isub].rotTrack.size(),
					m_ObjectList[iObj].subMesh[isub].sclTrack.size()
				);

				for (int iV = 0; iV < m_ObjectList[iObj].subMesh[isub].list.size(); iV++)
				{
					_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f\n"), L"*VERTEX_P",
						m_ObjectList[iObj].subMesh[isub].list[iV].p.x,
						m_ObjectList[iObj].subMesh[isub].list[iV].p.y,
						m_ObjectList[iObj].subMesh[isub].list[iV].p.z);
				}

				
				for (int iV = 0; iV < m_ObjectList[iObj].subMesh[isub].list.size(); iV++)
				{
					_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f\n"), L"*VERTEX_N",
						m_ObjectList[iObj].subMesh[isub].list[iV].n.x,
						m_ObjectList[iObj].subMesh[isub].list[iV].n.y,
						m_ObjectList[iObj].subMesh[isub].list[iV].n.z);
				}

				
				for (int iV = 0; iV < m_ObjectList[iObj].subMesh[isub].list.size(); iV++)
				{
					_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f %10.4f\n"), L"*VERTEX_C",
						m_ObjectList[iObj].subMesh[isub].list[iV].c.x,
						m_ObjectList[iObj].subMesh[isub].list[iV].c.y,
						m_ObjectList[iObj].subMesh[isub].list[iV].c.z,
						m_ObjectList[iObj].subMesh[isub].list[iV].c.w);
				}

				
				for (int iV = 0; iV < m_ObjectList[iObj].subMesh[isub].list.size(); iV++)
				{
					_ftprintf(fp, _T("\t%s %10.4f %10.4f\n"), L"*VERTEX_T",
						m_ObjectList[iObj].subMesh[isub].list[iV].t.x,
						m_ObjectList[iObj].subMesh[isub].list[iV].t.y);
				}

				
				for (int iV = 0; iV < m_ObjectList[iObj].subMesh[isub].list.size(); iV++)
				{
					_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f %10.4f\n"), L"*VERTEX_I",
						m_ObjectList[iObj].subMesh[isub].list[iV].i[0],
						m_ObjectList[iObj].subMesh[isub].list[iV].i[1],
						m_ObjectList[iObj].subMesh[isub].list[iV].i[2],
						m_ObjectList[iObj].subMesh[isub].list[iV].i[3]);
				}
				
				for (int iV = 0; iV < m_ObjectList[iObj].subMesh[isub].list.size(); iV++)
				{
					_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f %10.4f\n"), L"*VERTEX_W",
						m_ObjectList[iObj].subMesh[isub].list[iV].w[0],
						m_ObjectList[iObj].subMesh[isub].list[iV].w[1],
						m_ObjectList[iObj].subMesh[isub].list[iV].w[2],
						m_ObjectList[iObj].subMesh[isub].list[iV].w[3]);
				}
				


				for (int iTrack = 0; iTrack < m_ObjectList[iObj].subMesh[isub].posTrack.size(); iTrack++)
				{
					_ftprintf(fp, _T("\t\t%s %d %10.4f %10.4f %10.4f\n"), L"*ANIPOS",
						m_ObjectList[iObj].subMesh[isub].posTrack[iTrack].iTick,
						m_ObjectList[iObj].subMesh[isub].posTrack[iTrack].p.x,
						m_ObjectList[iObj].subMesh[isub].posTrack[iTrack].p.y,
						m_ObjectList[iObj].subMesh[isub].posTrack[iTrack].p.z);
				}


				for (int iTrack = 0; iTrack < m_ObjectList[iObj].subMesh[isub].rotTrack.size(); iTrack++)
				{
					_ftprintf(fp, _T("\t\t%s %d %10.4f %10.4f %10.4f %10.4f\n"), L"*ANIROT",
						m_ObjectList[iObj].subMesh[isub].rotTrack[iTrack].iTick,
						m_ObjectList[iObj].subMesh[isub].rotTrack[iTrack].q.x,
						m_ObjectList[iObj].subMesh[isub].rotTrack[iTrack].q.y,
						m_ObjectList[iObj].subMesh[isub].rotTrack[iTrack].q.z,
						m_ObjectList[iObj].subMesh[isub].rotTrack[iTrack].q.w);
				}


				for (int iTrack = 0; iTrack < m_ObjectList[iObj].subMesh[isub].sclTrack.size(); iTrack++)
				{
					_ftprintf(fp, _T("\t\t%s %d %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n"), L"*ANISCL",
						m_ObjectList[iObj].subMesh[isub].sclTrack[iTrack].iTick,

						m_ObjectList[iObj].subMesh[isub].sclTrack[iTrack].p.x,
						m_ObjectList[iObj].subMesh[isub].sclTrack[iTrack].p.y,
						m_ObjectList[iObj].subMesh[isub].sclTrack[iTrack].p.z,

						m_ObjectList[iObj].subMesh[isub].sclTrack[iTrack].q.x,
						m_ObjectList[iObj].subMesh[isub].sclTrack[iTrack].q.y,
						m_ObjectList[iObj].subMesh[isub].sclTrack[iTrack].q.z,
						m_ObjectList[iObj].subMesh[isub].sclTrack[iTrack].q.w);
				}
			}
		}
		else
		{
			

			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f\n"), L"*VERTEX_P",
					m_ObjectList[iObj].list[iV].p.x,
					m_ObjectList[iObj].list[iV].p.y,
					m_ObjectList[iObj].list[iV].p.z);
			}

			
			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f\n"), L"*VERTEX_N",
					m_ObjectList[iObj].list[iV].n.x,
					m_ObjectList[iObj].list[iV].n.y,
					m_ObjectList[iObj].list[iV].n.z);
			}

			
			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f %10.4f\n"), L"*VERTEX_C",
					m_ObjectList[iObj].list[iV].c.x,
					m_ObjectList[iObj].list[iV].c.y,
					m_ObjectList[iObj].list[iV].c.z,
					m_ObjectList[iObj].list[iV].c.w);
			}

			
			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f\n"), L"*VERTEX_T",
					m_ObjectList[iObj].list[iV].t.x,
					m_ObjectList[iObj].list[iV].t.y);
			}

			
			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f %10.4f\n"), L"*VERTEX_I",
					m_ObjectList[iObj].list[iV].i[0],
					m_ObjectList[iObj].list[iV].i[1],
					m_ObjectList[iObj].list[iV].i[2],
					m_ObjectList[iObj].list[iV].i[3]);
			}
			
			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f %10.4f\n"), L"*VERTEX_W",
					m_ObjectList[iObj].list[iV].w[0],
					m_ObjectList[iObj].list[iV].w[1],
					m_ObjectList[iObj].list[iV].w[2],
					m_ObjectList[iObj].list[iV].w[3]);
			}

			

			for (int iTrack = 0; iTrack < m_ObjectList[iObj].posTrack.size(); iTrack++)
			{
				_ftprintf(fp, _T("\t\t%s %d %10.4f %10.4f %10.4f\n"), L"*ANIPOS",
					m_ObjectList[iObj].posTrack[iTrack].iTick,
					m_ObjectList[iObj].posTrack[iTrack].p.x,
					m_ObjectList[iObj].posTrack[iTrack].p.y,
					m_ObjectList[iObj].posTrack[iTrack].p.z);
			}


			for (int iTrack = 0; iTrack < m_ObjectList[iObj].rotTrack.size(); iTrack++)
			{
				_ftprintf(fp, _T("\t\t%s %d %10.4f %10.4f %10.4f %10.4f\n"), L"*ANIROT",
					m_ObjectList[iObj].rotTrack[iTrack].iTick,
					m_ObjectList[iObj].rotTrack[iTrack].q.x,
					m_ObjectList[iObj].rotTrack[iTrack].q.y,
					m_ObjectList[iObj].rotTrack[iTrack].q.z,
					m_ObjectList[iObj].rotTrack[iTrack].q.w);
			}


			for (int iTrack = 0; iTrack < m_ObjectList[iObj].sclTrack.size(); iTrack++)
			{
				_ftprintf(fp, _T("\t\t%s %d %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n"), L"*ANISCL",
					m_ObjectList[iObj].sclTrack[iTrack].iTick,

					m_ObjectList[iObj].sclTrack[iTrack].p.x,
					m_ObjectList[iObj].sclTrack[iTrack].p.y,
					m_ObjectList[iObj].sclTrack[iTrack].p.z,

					m_ObjectList[iObj].sclTrack[iTrack].q.x,
					m_ObjectList[iObj].sclTrack[iTrack].q.y,
					m_ObjectList[iObj].sclTrack[iTrack].q.z,
					m_ObjectList[iObj].sclTrack[iTrack].q.w);
			}
		}
		int temp = 0;
		_ftprintf(fp, _T("\t%s %d %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n"), L"*BOX",
			temp,
			m_ObjectList[iObj].m_CharBox.m_Pos.x,
			m_ObjectList[iObj].m_CharBox.m_Pos.y,
			m_ObjectList[iObj].m_CharBox.m_Pos.z,
			m_ObjectList[iObj].m_CharBox.m_Box.pmax.x,
			m_ObjectList[iObj].m_CharBox.m_Box.pmax.y,
			m_ObjectList[iObj].m_CharBox.m_Box.pmax.z,
			m_ObjectList[iObj].m_CharBox.m_Box.pmin.x,
			m_ObjectList[iObj].m_CharBox.m_Box.pmin.y,
			m_ObjectList[iObj].m_CharBox.m_Box.pmin.z);
	}
	_ftprintf(fp, _T("%s %d \n"), L"BONE_COUNT", m_pReadObjList.size());

	for (int ibone = 0; ibone < m_pReadObjList.size(); ibone++)
	{
		INode* pNode = m_pReadObjList[ibone];
		Matrix3 invTm = Inverse(pNode->GetNodeTM(m_AnimTime.Start()));
		D3D_MATRIX mat;
		DumpMatrix3(&invTm, mat);
		_ftprintf(fp, _T("%10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n"),
			mat._11,
			mat._12,
			mat._13,
			mat._14,
			mat._21,
			mat._22,
			mat._23,
			mat._24,
			mat._31,
			mat._32,
			mat._33,
			mat._34,
			mat._41,
			mat._42,
			mat._43,
			mat._44);
	}
	fclose(fp);
}

void CExpGlobal::MatrixExporter()
{
	TCHAR  szFile[256] = { 0, };
	TCHAR* pszname;

	pszname = SaveFileDlg(_T("mtr"), _T("Export"));
	if (pszname == NULL)
	{
		return;
	}

	_tcscpy(szFile, pszname);

	FILE* fp;
	fp = _tfopen(szFile, _T("wt"));
	_ftprintf(fp, _T("%s %d %d %d %d %d %d\n"), L"*SCENE",
		m_Scene.iFirstFrame, m_Scene.iLastFrame, m_Scene.iFrameSpeed,
		m_Scene.iTickPerFrame, 0, m_Scene.iNumMesh);


	for (int iObj = 0; iObj < m_Scene.iNumMesh; iObj++)
	{
		_ftprintf(fp, _T("%s %d %d %d %d %d %d %d %d %d\n"), L"*OBJECT", iObj,
			(m_ObjectList[iObj].m_iType == BYPED) ? m_ObjectList[iObj].list.size() : 0,
			-1,
			m_ObjectList[iObj].posTrack.size(),
			m_ObjectList[iObj].rotTrack.size(),
			m_ObjectList[iObj].sclTrack.size(),
			m_ObjectList[iObj].subMesh.size(),
			m_ObjectList[iObj].m_iBone,
			m_ObjectList[iObj].m_iRoot);

		_ftprintf(fp, _T("%s %s %s \n"), L"*NAME",
			m_ObjectList[iObj].m_szName,
			m_ObjectList[iObj].m_szParentName);

		_ftprintf(fp, _T("%s %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n"),
			L"*MATWORLD",
			m_ObjectList[iObj].m_matWorld._11,
			m_ObjectList[iObj].m_matWorld._12,
			m_ObjectList[iObj].m_matWorld._13,
			m_ObjectList[iObj].m_matWorld._14,
			m_ObjectList[iObj].m_matWorld._21,
			m_ObjectList[iObj].m_matWorld._22,
			m_ObjectList[iObj].m_matWorld._23,
			m_ObjectList[iObj].m_matWorld._24,
			m_ObjectList[iObj].m_matWorld._31,
			m_ObjectList[iObj].m_matWorld._32,
			m_ObjectList[iObj].m_matWorld._33,
			m_ObjectList[iObj].m_matWorld._34,
			m_ObjectList[iObj].m_matWorld._41,
			m_ObjectList[iObj].m_matWorld._42,
			m_ObjectList[iObj].m_matWorld._43,
			m_ObjectList[iObj].m_matWorld._44);

		if (m_ObjectList[iObj].m_iType == BYPED)
		{
			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f\n"), L"*VERTEX_P",
					m_ObjectList[iObj].list[iV].p.x,
					m_ObjectList[iObj].list[iV].p.y,
					m_ObjectList[iObj].list[iV].p.z);
			}

			
			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f\n"), L"*VERTEX_N",
					m_ObjectList[iObj].list[iV].n.x,
					m_ObjectList[iObj].list[iV].n.y,
					m_ObjectList[iObj].list[iV].n.z);
			}

			
			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f %10.4f\n"), L"*VERTEX_C",
					m_ObjectList[iObj].list[iV].c.x,
					m_ObjectList[iObj].list[iV].c.y,
					m_ObjectList[iObj].list[iV].c.z,
					m_ObjectList[iObj].list[iV].c.w);
			}

			
			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f\n"), L"*VERTEX_T",
					m_ObjectList[iObj].list[iV].t.x,
					m_ObjectList[iObj].list[iV].t.y);
			}
			
			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f %10.4f\n"), L"*VERTEX_I",
					m_ObjectList[iObj].list[iV].i[0],
					m_ObjectList[iObj].list[iV].i[1],
					m_ObjectList[iObj].list[iV].i[2],
					m_ObjectList[iObj].list[iV].i[3]);
			}

			for (int iV = 0; iV < m_ObjectList[iObj].list.size(); iV++)
			{
				_ftprintf(fp, _T("\t%s %10.4f %10.4f %10.4f %10.4f\n"), L"*VERTEX_W",
					m_ObjectList[iObj].list[iV].w[0],
					m_ObjectList[iObj].list[iV].w[1],
					m_ObjectList[iObj].list[iV].w[2],
					m_ObjectList[iObj].list[iV].w[3]);
			}
		}

		

		for (int iTrack = 0; iTrack < m_ObjectList[iObj].posTrack.size(); iTrack++)
		{
			_ftprintf(fp, _T("\t\t%s %d %10.4f %10.4f %10.4f\n"), L"*ANIPOS",
				m_ObjectList[iObj].posTrack[iTrack].iTick,
				m_ObjectList[iObj].posTrack[iTrack].p.x,
				m_ObjectList[iObj].posTrack[iTrack].p.y,
				m_ObjectList[iObj].posTrack[iTrack].p.z);
		}


		for (int iTrack = 0; iTrack < m_ObjectList[iObj].rotTrack.size(); iTrack++)
		{
			_ftprintf(fp, _T("\t\t%s %d %10.4f %10.4f %10.4f %10.4f\n"), L"*ANIROT",
				m_ObjectList[iObj].rotTrack[iTrack].iTick,
				m_ObjectList[iObj].rotTrack[iTrack].q.x,
				m_ObjectList[iObj].rotTrack[iTrack].q.y,
				m_ObjectList[iObj].rotTrack[iTrack].q.z,
				m_ObjectList[iObj].rotTrack[iTrack].q.w);
		}


		for (int iTrack = 0; iTrack < m_ObjectList[iObj].sclTrack.size(); iTrack++)
		{
			_ftprintf(fp, _T("\t\t%s %d %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n"), L"*ANISCL",
				m_ObjectList[iObj].sclTrack[iTrack].iTick,

				m_ObjectList[iObj].sclTrack[iTrack].p.x,
				m_ObjectList[iObj].sclTrack[iTrack].p.y,
				m_ObjectList[iObj].sclTrack[iTrack].p.z,

				m_ObjectList[iObj].sclTrack[iTrack].q.x,
				m_ObjectList[iObj].sclTrack[iTrack].q.y,
				m_ObjectList[iObj].sclTrack[iTrack].q.z,
				m_ObjectList[iObj].sclTrack[iTrack].q.w);
		}
		
		if (m_ObjectList[iObj].m_iBone==1)
		{
			int temp = 0;
			_ftprintf(fp, _T("\t%s %d %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n"), L"*BONEBOX",
				temp,
				m_ObjectList[iObj].m_Bone.m_Pos.x,
				m_ObjectList[iObj].m_Bone.m_Pos.y,
				m_ObjectList[iObj].m_Bone.m_Pos.z,
				m_ObjectList[iObj].m_Bone.m_Box.pmax.x,
				m_ObjectList[iObj].m_Bone.m_Box.pmax.y,
				m_ObjectList[iObj].m_Bone.m_Box.pmax.z,
				m_ObjectList[iObj].m_Bone.m_Box.pmin.x,
				m_ObjectList[iObj].m_Bone.m_Box.pmin.y,
				m_ObjectList[iObj].m_Bone.m_Box.pmin.z);
		}

	}

	/*int iObj = m_ExpSelNodes[0];
	int temp = 0;
	_ftprintf(fp, _T("\t%s %d %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n"), L"*BOX",
		temp,
		m_ObjectList[iObj].m_CharBox.m_Pos.x,
		m_ObjectList[iObj].m_CharBox.m_Pos.y,
		m_ObjectList[iObj].m_CharBox.m_Pos.z,
		m_ObjectList[iObj].m_CharBox.m_Box.pmax.x,
		m_ObjectList[iObj].m_CharBox.m_Box.pmax.y,
		m_ObjectList[iObj].m_CharBox.m_Box.pmax.z,
		m_ObjectList[iObj].m_CharBox.m_Box.pmin.x,
		m_ObjectList[iObj].m_CharBox.m_Box.pmin.y,
		m_ObjectList[iObj].m_CharBox.m_Box.pmin.z);*/

	_ftprintf(fp, _T("%s %d \n"), L"BONE_COUNT", m_pReadObjList.size());

	for (int ibone = 0; ibone < m_pReadObjList.size(); ibone++)
	{
		INode* pNode = m_pReadObjList[ibone];
		Matrix3 invTm = Inverse(pNode->GetNodeTM(m_AnimTime.Start()));
		D3D_MATRIX mat;
		DumpMatrix3(&invTm, mat);
		_ftprintf(fp, _T("%10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n"),
			mat._11,
			mat._12,
			mat._13,
			mat._14,
			mat._21,
			mat._22,
			mat._23,
			mat._24,
			mat._31,
			mat._32,
			mat._33,
			mat._34,
			mat._41,
			mat._42,
			mat._43,
			mat._44);
	}
	fclose(fp);
}


void CExpGlobal::GetTexture(Mtl* pMtl, CMtrl& tMtrl)
{
	int iSubMap = pMtl->NumSubTexmaps();
	for (int iMap = 0; iMap < iSubMap; iMap++)
	{
		Texmap* tex = pMtl->GetSubTexmap(iMap);
		if (tex != nullptr)
		{
			Class_ID id = tex->ClassID();
			if (id == Class_ID(BMTEX_CLASS_ID, 0x00))
			{
				CSubMap tMap;
				BitmapTex* pBitTex = (BitmapTex*)tex;
				MSTR mapName = pBitTex->GetMapName();
				MSTR szDir, szName;
				SplitPathFile(mapName, &szDir, &szName);
				tMap.szTextureName = szName;
				tMap.iSubNo = iMap;
				tMtrl.texList.push_back(tMap);
			}
		}
	}
}

void CExpGlobal::DumpPosSample(INode* pNode, CMesh* pMesh)
{

	TimeValue start = m_AnimTime.Start();
	TimeValue end = m_AnimTime.End();
	AffineParts ap;

	for (TimeValue t = start; t <= end; t += GetTicksPerFrame())
	{
		GetDecompAffine(t, pNode, &ap);

		CAnimTrack pAnim;
		pAnim.iTick = t;
		pAnim.p.x = ap.t.x;
		pAnim.p.y = ap.t.z;
		pAnim.p.z = ap.t.y;
		pMesh->posTrack.push_back(pAnim);
	}
	/*AffineParts ap;
	GetDecompAffine(m_AnimTime.Start(), pNode, &ap);
	CAnimTrack track, trackLast;
	ZeroMemory(&track, sizeof(CAnimTrack));
	track.iTick = m_AnimTime.Start();
	DumpPoint3(ap.t, track.p);
	pMesh->posTrack.push_back(track);
	trackLast = pMesh->posTrack[0];
	bool bStart = true;
	for (TimeValue  t = m_AnimTime.Start() + GetTicksPerFrame(); t < m_AnimTime.End(); t += GetTicksPerFrame())
	{
		GetDecompAffine(t, pNode, &ap);

		CAnimTrack track;
		ZeroMemory(&track, sizeof(CAnimTrack));
		track.iTick = t;
		DumpPoint3(ap.t,track.p);
		if (!EqualPoint3(trackLast.p, ap.t))
		{
			if ((t - GetTicksPerFrame()) !=
				pMesh->posTrack[pMesh->posTrack.size() - 1].iTick)
			{
				pMesh->posTrack.push_back(track);
			}
			trackLast = track;
		}
		trackLast = track;
	}

	GetDecompAffine(m_AnimTime.End(), pNode, &ap);
	ZeroMemory(&trackLast, sizeof(CAnimTrack));
	trackLast.iTick = m_AnimTime.End();
	DumpPoint3(ap.t, trackLast.p);
	pMesh->posTrack.push_back(trackLast);*/
}
void CExpGlobal::DumpRotSample(INode* pNode, CMesh* pMesh)
{
	TimeValue start = m_AnimTime.Start();
	TimeValue end = m_AnimTime.End();

	AffineParts ap;
	for (TimeValue t = start; t <= end; t += GetTicksPerFrame())
	{
		GetDecompAffine(t, pNode, &ap);

		CAnimTrack pAnim;
		pAnim.iTick = t;
		pAnim.q.x = ap.q.x;
		pAnim.q.y = ap.q.z;
		pAnim.q.z = ap.q.y;
		pAnim.q.w = ap.q.w;
		pMesh->rotTrack.push_back(pAnim);
	}
	/*AffineParts ap;
	float vAngle;
	Point3 vAxis;
	GetDecompAffine(m_AnimTime.Start(), pNode, &ap);

	CAnimTrack track, trackLast;
	ZeroMemory(&track, sizeof(CAnimTrack));
	track.iTick = m_AnimTime.Start();
	DumpQuat(ap.q, track.q);
	pMesh->rotTrack.push_back(track);
	trackLast = pMesh->rotTrack[0];

	for (TimeValue  t = m_AnimTime.Start() + GetTicksPerFrame(); t < m_AnimTime.End(); t += GetTicksPerFrame())
	{
		GetDecompAffine(t, pNode, &ap);
		ZeroMemory(&track, sizeof(CAnimTrack));
		track.iTick = t;
		DumpQuat(ap.q, track.q);
		if (!EqualQuat(trackLast.q, ap.q))
		{
			if ((t - GetTicksPerFrame()) !=
				pMesh->rotTrack[pMesh->rotTrack.size() - 1].iTick)
			{
				pMesh->rotTrack.push_back(trackLast);
			}
			pMesh->rotTrack.push_back(track);
		}
		trackLast = track;
	}

	GetDecompAffine(m_AnimTime.End(), pNode, &ap);
	ZeroMemory(&trackLast, sizeof(CAnimTrack));
	trackLast.iTick = m_AnimTime.End();
	DumpQuat(ap.q, trackLast.q);
	pMesh->rotTrack.push_back(trackLast);*/
}
void CExpGlobal::DumpSclSample(INode* pNode, CMesh* pMesh)
{
	AffineParts ap;
	float vAngle;
	Point3 vAxis;
	GetDecompAffine(m_AnimTime.Start(), pNode, &ap);

	CAnimTrack track, trackLast;
	ZeroMemory(&track, sizeof(CAnimTrack));
	track.iTick = m_AnimTime.Start();
	DumpPoint3(ap.k, track.p);
	DumpQuat(ap.u, track.q);
	pMesh->sclTrack.push_back(track);
	trackLast = pMesh->sclTrack[0];

	for (TimeValue  t = m_AnimTime.Start() + GetTicksPerFrame(); t < m_AnimTime.End(); t += GetTicksPerFrame())
	{
		GetDecompAffine(t, pNode, &ap);
		ZeroMemory(&track, sizeof(CAnimTrack));
		track.iTick = t;
		DumpPoint3(ap.k, track.p);
		DumpQuat(ap.u, track.q);
		if (!EqualPoint3(trackLast.p, ap.k))
		{
			if ((t - GetTicksPerFrame()) !=
				pMesh->sclTrack[pMesh->sclTrack.size() - 1].iTick)
			{
				pMesh->sclTrack.push_back(track);
			}
			trackLast = track;
		}
	}

	GetDecompAffine(m_AnimTime.End(), pNode, &ap);
	ZeroMemory(&trackLast, sizeof(CAnimTrack));
	trackLast.iTick = m_AnimTime.End();
	DumpQuat(ap.q, trackLast.q);
	pMesh->sclTrack.push_back(trackLast);
}

void CExpGlobal::DumpMatrix3(Matrix3* m, D3D_MATRIX& mat)
{
	Point3 row;
	row = m->GetRow(0);
	mat._11 = row.x; mat._13 = row.y; mat._12 = row.z;
	row = m->GetRow(2);
	mat._21 = row.x; mat._23 = row.y; mat._22 = row.z;
	row = m->GetRow(1);
	mat._31 = row.x; mat._33 = row.y; mat._32 = row.z;
	row = m->GetRow(3);
	mat._41 = row.x; mat._43 = row.y; mat._42 = row.z;
	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;
}

void CExpGlobal::Matrix3convert(Matrix3& befor, Matrix3& after)
{
	Point3 row,r1,r2,r3,r4;
	row = befor.GetRow(0);
	r1.x = row.x; r1.z = row.y; r1.y = row.z;
	row = befor.GetRow(2);
	r2.x = row.x; r2.z = row.y; r2.y = row.z;
	row = befor.GetRow(1);
	r3.x = row.x; r3.z = row.y; r3.y = row.z;
	row = befor.GetRow(3);
	r4.x = row.x; r4.z = row.y; r4.y = row.z;

	after.SetRow(0, r1);
	after.SetRow(1, r2);
	after.SetRow(2, r3);
	after.SetRow(3, r4);
}

void CExpGlobal::DumpPoint3(Point3& pDest, Point3& pSrc)
{
	pSrc.x = pDest.x;
	pSrc.y = pDest.z;
	pSrc.z = pDest.y;
}

void CExpGlobal::DumpQuat(Quat& pDest, Quat& pSrc)
{
	pSrc.x = pDest.x;
	pSrc.y = pDest.z;
	pSrc.z = pDest.y;
	pSrc.w = pDest.w;
}

void CExpGlobal::GetMesh(INode* pnode, CMesh* pmesh)
{
	Matrix3 tm = pnode->GetObjTMAfterWSM(m_AnimTime.Start());
	ObjectState os = pnode->EvalWorldState(m_AnimTime.Start());
	Object* pObj = os.obj;
	BOOL needDel;
	if (pObj == nullptr || (pObj->SuperClassID() != GEOMOBJECT_CLASS_ID))
	{
		return;
	}

	TriObject* tri = GetTriObjectFromNoode(pnode, m_AnimTime.Start(), needDel);
	if (tri == nullptr)
	{
		return;
	}

	Mesh* mesh = &tri->GetMesh();

	mesh->buildBoundingBox();
	Box3 box = mesh->getBoundingBox(&tm);
	Point3 pos = tm.GetRow(3);
	DumpPoint3(pos, pmesh->m_CharBox.m_Pos);

	DumpPoint3(box.pmax, pmesh->m_CharBox.m_Box.pmax);
	DumpPoint3(box.pmin, pmesh->m_CharBox.m_Box.pmin);

	pmesh->m_CharBox.m_Box.pmax.x = pmesh->m_CharBox.m_Box.pmax.x + pmesh->m_CharBox.m_Pos.x;
	pmesh->m_CharBox.m_Box.pmax.y = pmesh->m_CharBox.m_Box.pmax.y + pmesh->m_CharBox.m_Pos.y;
	pmesh->m_CharBox.m_Box.pmax.z = pmesh->m_CharBox.m_Box.pmax.z + pmesh->m_CharBox.m_Pos.z;

	pmesh->m_CharBox.m_Box.pmin.x = pmesh->m_CharBox.m_Pos.x + pmesh->m_CharBox.m_Box.pmin.x;
	pmesh->m_CharBox.m_Box.pmin.y = pmesh->m_CharBox.m_Pos.y + pmesh->m_CharBox.m_Box.pmin.y;
	pmesh->m_CharBox.m_Box.pmin.z = pmesh->m_CharBox.m_Pos.z + pmesh->m_CharBox.m_Box.pmin.z;

	Mtl* pSrc = pnode->GetMtl();
	pmesh->m_iTexIndex = -1;
	bool bSubMtrl = false;
	
	for (int iMtrl = 0; iMtrl < m_MtlList.size(); iMtrl++)
	{
		if (m_pReadMtlList[iMtrl] == pSrc)
		{
			pmesh->m_iTexIndex = iMtrl;
			if (m_MtlList[iMtrl].subMtrl.size() > 0)
			{
				bSubMtrl = true;
			}
			break;
		}
	}
	if (bSubMtrl)
	{
		pmesh->subMesh.resize(
			m_MtlList[pmesh->m_iTexIndex].subMtrl.size());
	}
	else
	{
		pmesh->list.resize(mesh->numFaces * 3);
	}

	m_BipedList.clear();

	Modifier* PhysiqueModifier = FindModifier(pnode, Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B));
	Modifier* SkinModifier = FindModifier(pnode, SKIN_CLASSID);
	if (PhysiqueModifier) 
	{
		ExportPhysiqueData(pnode, PhysiqueModifier);
	}
	else if (SkinModifier) 
	{
		ExportSkinData(pnode, SkinModifier);
	}

	bool bipedInsert = false;
	if (!(PhysiqueModifier || SkinModifier))
	{
		// track, parent
		INode* pParent = pnode->GetParentNode();
		if (pParent && !pParent->IsRootNode())
		{
			bipedInsert = true;
		}
		if (pmesh->posTrack.size() > 0 ||
			pmesh->rotTrack.size() > 0 ||
			pmesh->sclTrack.size() > 0)
		{
			bipedInsert = true;
		}
		
	}

	for (int iFace = 0; iFace < mesh->numFaces; iFace++)
	{
		SetPNCTData(pnode,iFace, pmesh, mesh, tm, bSubMtrl, bipedInsert);
	}

	if (needDel == TRUE)
	{
		delete tri;
	}
}

Modifier* CExpGlobal::FindModifier(INode *nodePtr, Class_ID classID)
{
	Object *ObjectPtr = nodePtr->GetObjectRef();
	if (!ObjectPtr)
	{
		return NULL;
	}
	while (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID && ObjectPtr)
	{
		IDerivedObject *DerivedObjectPtr = (IDerivedObject *)(ObjectPtr);

		int ModStackIndex = 0;
		while (ModStackIndex < DerivedObjectPtr->NumModifiers())
		{
			Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);

			if (ModifierPtr->ClassID() == classID)
			{
				return ModifierPtr;
			}

			ModStackIndex++;
		}
		ObjectPtr = DerivedObjectPtr->GetObjRef();
	}
	return NULL;
}

void CExpGlobal::ExportPhysiqueData(INode* pNode, Modifier*Modifier)
{
	IPhysiqueExport* phyExport =
		(IPhysiqueExport*)Modifier->GetInterface(I_PHYEXPORT);
	IPhyContextExport* mcExport =
		(IPhyContextExport*)phyExport->GetContextInterface(pNode);

	mcExport->ConvertToRigid(true);
	mcExport->AllowBlending(true);

	int iNumVertex = mcExport->GetNumberVertices();
	m_BipedList.resize(iNumVertex);

	for (int i = 0; i < iNumVertex; i++)
	{
		m_BipedList[i].idList.resize(4);
		m_BipedList[i].weightList.resize(4);

		IPhyVertexExport * vi = mcExport->GetVertexInterface(i);
		int iType = vi->GetVertexType();
		switch (iType)
		{
		case RIGID_BLENDED_TYPE:
		{
			IPhyBlendedRigidVertex* vx =
				(IPhyBlendedRigidVertex*)vi;
			BipedVertex vertex;
			int iNumWeight = vx->GetNumberNodes();
			vertex.iNumWeight = iNumWeight;
			for (int weight = 0; weight < iNumWeight; weight++)
			{
				INode* node = vx->GetNode(weight);
				InsertWeight(i, GetFindIndex(node), vx->GetWeight(weight));
			}
			m_BipedList.push_back(vertex);
		}break;
		case RIGID_NON_BLENDED_TYPE:
		{
			IPhyRigidVertex* vx = (IPhyRigidVertex*)vi;
			INode* node = vx->GetNode();
			BipedVertex vertex;
			vertex.iNumWeight = 1;
			InsertWeight(i, GetFindIndex(node), 1.0f);
		}break;
		}
	}

	phyExport->ReleaseContextInterface(mcExport);
	Modifier->ReleaseInterface(I_PHYEXPORT, phyExport);
}

void CExpGlobal::ExportSkinData(INode* pNode, Modifier* Modifier)
{
	ISkin* skin = (ISkin*)Modifier->GetInterface(I_SKIN);
	ISkinContextData* skinData = skin->GetContextInterface(pNode);
	if (skinData)
	{
		int iNumVertex = skinData->GetNumPoints();
		m_BipedList.resize(iNumVertex);
		for (int i = 0; i < iNumVertex; i++)
		{
			m_BipedList[i].idList.resize(4);
			m_BipedList[i].weightList.resize(4);

			int iNumWeight = skinData->GetNumAssignedBones(i);
			for (int iv = 0; iv < iNumWeight; iv++)
			{
				int boneIndex = skinData->GetAssignedBone(i, iv);
				INode* pNode = skin->GetBone(boneIndex);
				float fWeight = skinData->GetBoneWeight(i, iv);
				InsertWeight(i, GetFindIndex(pNode), fWeight);
			}
		}
	}
}

bool CExpGlobal::TMNegParity(Matrix3 &m)
{
	Point3 vRow = CrossProd(m.GetRow(0), m.GetRow(1));
	return (DotProd(vRow, m.GetRow(2)) < 0.0) ? 1 : 0;
}

void CExpGlobal::SetPNCTData(INode* pnode,int Face, CMesh* pmesh, Mesh* mesh, Matrix3 tm, bool bSubMtrl, bool bipedInsert)
{
	
		int iSubMtrl = mesh->faces[Face].getMatID();
		BOOL negScale = TMNegParity(tm);
		int i0, i1, i2;
		if (negScale)
		{
			i0 = 2;
			i1 = 0;
			i2 = 1;
		}
		else
		{
			i0 = 0;
			i1 = 2;
			i2 = 1;
		}

		PNCTIW_VERTEX pnct[3];
		if (m_BipedList.size() > 0)
		{
			BipedVertex& bv0 = m_BipedList[mesh->faces[Face].v[i0]];
			if (bv0.iNumWeight >= 5)
			{
				int kk = 0;
			}
			for (int i = 0; i < 4; i++)
			{
				pnct[0].i[i] = bv0.idList[i];
				pnct[0].w[i] = bv0.weightList[i];
			}
			BipedVertex& bv1 = m_BipedList[mesh->faces[Face].v[i1]];
			if (bv1.iNumWeight >= 5)
			{
				int kk = 0;
			}
			for (int i = 0; i < 4; i++)
			{
				pnct[1].i[i] = bv1.idList[i];
				pnct[1].w[i] = bv1.weightList[i];
			}
			BipedVertex& bv2 = m_BipedList[mesh->faces[Face].v[i2]];
			if (bv2.iNumWeight >= 5)
			{
				int kk = 0;
			}
			for (int i = 0; i < 4; i++)
			{
				pnct[2].i[i] = bv2.idList[i];
				pnct[2].w[i] = bv2.weightList[i];
			}
		}

		if (bipedInsert == true)
		{
			int iBone = GetFindIndex(pnode);
			pnct[0].i[0] = iBone;
			pnct[0].w[0] = 1.0f;
			pnct[1].i[0] = iBone;
			pnct[1].w[0] = 1.0f;
			pnct[2].i[0] = iBone;
			pnct[2].w[0] = 1.0f;
		}
		
		Point3 v1, v2, v3;
		v1 = tm * mesh->verts[mesh->faces[Face].v[i0]];
		v2 = tm * mesh->verts[mesh->faces[Face].v[i1]];
		v3 = tm * mesh->verts[mesh->faces[Face].v[i2]];

		DumpPoint3(v1, pnct[0].p);
		DumpPoint3(v2, pnct[1].p);
		DumpPoint3(v3, pnct[2].p);


		if (mesh->numTVerts > 0)
		{
			pnct[0].t.x = mesh->tVerts[mesh->tvFace[Face].t[i0]].x;
			pnct[0].t.y = 1.0f - mesh->tVerts[mesh->tvFace[Face].t[i0]].y;
			pnct[1].t.x = mesh->tVerts[mesh->tvFace[Face].t[i1]].x;
			pnct[1].t.y = 1.0f - mesh->tVerts[mesh->tvFace[Face].t[i1]].y;
			pnct[2].t.x = mesh->tVerts[mesh->tvFace[Face].t[i2]].x;
			pnct[2].t.y = 1.0f - mesh->tVerts[mesh->tvFace[Face].t[i2]].y;
		}
		if (mesh->numCVerts > 0)
		{
			pnct[0].c.x = mesh->vertCol[mesh->vcFace[Face].t[i0]].x;
			pnct[0].c.y = mesh->vertCol[mesh->vcFace[Face].t[i0]].y;
			pnct[0].c.z = mesh->vertCol[mesh->vcFace[Face].t[i0]].z;
			pnct[0].c.w = 1.0f;

			pnct[1].c.x = mesh->vertCol[mesh->vcFace[Face].t[i1]].x;
			pnct[1].c.y = mesh->vertCol[mesh->vcFace[Face].t[i1]].y;
			pnct[1].c.z = mesh->vertCol[mesh->vcFace[Face].t[i1]].z;
			pnct[1].c.w = 1.0f;

			pnct[2].c.x = mesh->vertCol[mesh->vcFace[Face].t[i2]].x;
			pnct[2].c.y = mesh->vertCol[mesh->vcFace[Face].t[i2]].y;
			pnct[2].c.z = mesh->vertCol[mesh->vcFace[Face].t[i2]].z;
			pnct[2].c.w = 1.0f;
		}
		else
		{
			pnct[0].c.x = 1.0f;
			pnct[0].c.y = 1.0f;
			pnct[0].c.z = 1.0f;
			pnct[0].c.w = 1.0f;

			pnct[1].c.x = 1.0f;
			pnct[1].c.y = 1.0f;
			pnct[1].c.z = 1.0f;
			pnct[1].c.w = 1.0f;

			pnct[2].c.x = 1.0f;
			pnct[2].c.y = 1.0f;
			pnct[2].c.z = 1.0f;
			pnct[2].c.w = 1.0f;
		}

		mesh->buildNormals();
		DWORD vert0 = mesh->faces[Face].getVert(i0);
		DWORD vert1 = mesh->faces[Face].getVert(i1);
		DWORD vert2 = mesh->faces[Face].getVert(i2);

		Point3 vn0 = GetVertexNormal(mesh, Face, mesh->getRVertPtr(vert0));
		Point3 vn1 = GetVertexNormal(mesh, Face, mesh->getRVertPtr(vert1));
		Point3 vn2 = GetVertexNormal(mesh, Face, mesh->getRVertPtr(vert2));

		DumpPoint3(vn0, pnct[0].n);
		DumpPoint3(vn1, pnct[1].n);
		DumpPoint3(vn2, pnct[2].n);

		if (bSubMtrl)
		{
			if (iSubMtrl >= pmesh->subMesh.size())
			{
				iSubMtrl = 0;
			}
			pmesh->subMesh[iSubMtrl].list.push_back(pnct[0]);
			pmesh->subMesh[iSubMtrl].list.push_back(pnct[1]);
			pmesh->subMesh[iSubMtrl].list.push_back(pnct[2]);
		}
		else
		{
			pmesh->list[Face * 3 + 0] = pnct[0];
			pmesh->list[Face * 3 + 1] = pnct[1];
			pmesh->list[Face * 3 + 2] = pnct[2];
		}
	
	
}

Point3  CExpGlobal::GetVertexNormal(Mesh* mesh, int iFace, RVertex* rv)
{
	Face* f = &mesh->faces[iFace];
	DWORD smGroup = f->smGroup;
	int numNormals = rv->rFlags & NORCT_MASK;
	Point3 vertexNormal;

	if (rv->rFlags & SPECIFIED_NORMAL) {
		vertexNormal = rv->rn.getNormal();
	}

	else if (numNormals && smGroup) 
	{
		if (numNormals == 1) 
		{
			vertexNormal = rv->rn.getNormal();
		}
		else 
		{
			for (int i = 0; i < numNormals; i++) 
			{
				if (rv->ern[i].getSmGroup() & smGroup) 
				{
					vertexNormal += rv->ern[i].getNormal();
				}
			}
			//vertexNormal /= numNormals;
		}
	}
	else 
	{
		vertexNormal = mesh->getFaceNormal(iFace);
	}

	return vertexNormal;
}

TriObject* CExpGlobal::GetTriObjectFromNoode(INode* pNode, int iStartTick, BOOL& needDel)
{
	needDel = FALSE;
	Object *obj = pNode->EvalWorldState(iStartTick).obj;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		TriObject *tri = (TriObject*)obj->ConvertToType(iStartTick, Class_ID(TRIOBJ_CLASS_ID,0));
		if (obj != tri)
		{
			needDel = TRUE;
		}
		return tri;
	}
	return nullptr;
}

bool CExpGlobal::CheckForAnimation(INode* pNode, BOOL& bPos, BOOL& bRot, BOOL& bScl)
{
	// start tick 정보 -> last tick 비교	
	TimeValue iDeltaTick = GetTicksPerFrame();
	TimeValue end = m_AnimTime.End();
	TimeValue start = m_AnimTime.Start() + iDeltaTick;

	AffineParts ap;
	GetDecompAffine(m_AnimTime.Start(), pNode, &ap);

	// start tick info;
	Point3 firstRotAxis, firstScale, firstSclAxis, vAxis;
	float  firstRotAngle, firstSclAngle, fAngle;
	Point3 firstPos = ap.t;
	AngAxisFromQ(ap.q, &firstRotAngle, firstRotAxis);
	AngAxisFromQ(ap.u, &firstSclAngle, firstSclAxis);
	firstScale = ap.k;

	for (TimeValue t = start; t <= end; t += iDeltaTick)
	{
		GetDecompAffine(t, pNode, &ap);
		if (bPos == FALSE && !EqualPoint3(firstPos, ap.t))
		{
			bPos = TRUE;
		}
		AngAxisFromQ(ap.q, &fAngle, vAxis);
		if (bRot == FALSE &&
			(!EqualPoint3(firstRotAxis, vAxis) ||
				!EqualPoint1(firstRotAngle, fAngle)))
		{
			bRot = TRUE;
		}
		if (bScl == FALSE &&
			!EqualPoint3(firstScale, ap.k))
		{
			bScl = TRUE;
		}
		if (bPos && bRot && bScl) break;
	}
	return (bPos || bRot || bScl);
}

bool CExpGlobal::EqualPoint1(float p1, float p2)
{
	if (fabs(p1 - p2) > ZERO)
		return false;
	return true;
}
bool CExpGlobal::EqualPoint2(Point2 p1, Point2 p2)
{
	if (fabs(p1.x - p2.x) > ZERO)
		return false;
	if (fabs(p1.y - p2.y) > ZERO)
		return false;
	return true;
}
bool CExpGlobal::EqualPoint3(Point3 p1, Point3 p2)
{
	if (fabs(p1.x - p2.x) > ZERO)
		return false;
	if (fabs(p1.y - p2.y) > ZERO)
		return false;
	if (fabs(p1.z - p2.z) > ZERO)
		return false;

	return true;
}
bool CExpGlobal::EqualPoint4(Point4 p1, Point4 p2)
{
	if (fabs(p1.x - p2.x) > ZERO)
		return false;
	if (fabs(p1.y - p2.y) > ZERO)
		return false;
	if (fabs(p1.z - p2.z) > ZERO)
		return false;
	if (fabs(p1.w - p2.w) > ZERO)
		return false;
	return true;
}
bool CExpGlobal::EqualQuat(Quat p1, Quat p2)
{
	float fAngle1, fAngle2;
	Point3 vAxis1, vAxis2;
	AngAxisFromQ(p1, &fAngle1, vAxis1);
	AngAxisFromQ(p2, &fAngle2, vAxis2);

	if ((!EqualPoint3(vAxis1, vAxis2) ||
		!EqualPoint1(fAngle1, fAngle2)))
	{
		return false;
	}
	return true;
}

TCHAR*	CExpGlobal::FixupName(MSTR name)
{
	memset(m_tmpBuffer, 0, sizeof(TCHAR)*MAX_PATH);

	TCHAR* cPtr;
	_tcscpy(m_tmpBuffer, name);
	cPtr = m_tmpBuffer;

	while (*cPtr)
	{
		if (*cPtr == '"')		*cPtr = SINGLE_QUOTE;
		else if (*cPtr == ' ' || *cPtr <= CTL_CHARS)
			*cPtr = _T('_');
		cPtr++;
	}
	return m_tmpBuffer;
}

CExpGlobal::CExpGlobal()
{
	m_pMax = nullptr;
	m_pRootNode = nullptr;
}


CExpGlobal::~CExpGlobal()
{
}
