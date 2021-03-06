#include "CBY_Character.h"
#include "CBY_Object.h"
#include "CBY_CharMgr.h"

namespace CBY
{
	bool CBY_Character::SkinLoad(T_STR pszLoad)
	{
		int index, list;
		index = I_CHARACTER.SkinLoad(pszLoad, m_obj.m_pd3dDevice, m_obj.m_pContext);
		m_SkinObjIndex.push_back(index);

		list = m_ObjList.size();
		m_ObjList.push_back(std::make_shared<CBY_SkinObj>());
		*m_ObjList[list] = *I_CHARACTER.m_SkinLoadList[index];

		m_SkinOriginalBox = m_ObjList[0]->m_ObjList[0]->m_CharBox;
		//m_SkinOriginalBox.vMax.z *= 2;
		//m_SkinOriginalBox.vMin.z *= 1.5;

		SetCharBox();
		return true;
	}

	bool CBY_Character::CharacterLoad(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context,T_STR pszLoad, const TCHAR* ShaderFileName, const TCHAR* TexFileName, const CHAR* VSName, const CHAR* PSName)
	{
		m_CharData.Load(pszLoad.c_str());
		Create(pd3dDevice, Context, ShaderFileName, TexFileName, VSName, PSName);

		m_dwCameraSocket = m_CharData.m_data.m_dwCameraSocket;

		for (DWORD dw = 0; dw < m_CharData.m_data.SknNameList.size(); dw++)
		{
			SkinLoad(m_CharData.m_data.SknNameList[dw]);
		}

		if (m_CharData.m_data.m_CharType == CHAR_FRAMETYPE)
		{
			SetStateNum(m_CharData.m_data.dwStateNum);

			for (DWORD dwState = 0; dwState < m_CharData.m_data.dwStateNum; dwState++)
			{
				SetCharAction(dwState, m_CharData.m_data.StateList[dwState].m_iStartFrame,
					m_CharData.m_data.StateList[dwState].m_iEndFrame);
			}

			for (DWORD dw = 0; dw < m_CharData.m_data.MtrList.size(); dw++)
			{
				BoneLoad(m_CharData.m_data.MtrList[dw].MtrName);
			}
		}
		else
		{
			SetStateNum(m_CharData.m_data.dwStateNum);

			for (DWORD dw = 0; dw < m_CharData.m_data.MtrList.size(); dw++)
			{
				if (m_CharData.m_data.MtrList[dw].State == -1)
				{
					BoneLoad(m_CharData.m_data.MtrList[dw].MtrName);
				}
				else
				{
					SetMTRApply(m_CharData.m_data.MtrList[dw].MtrName, m_CharData.m_data.MtrList[dw].State);
				}
			}
		}
		SetColBoxList(m_CharData.m_data.ColBoxList);
		m_CharData.m_data.ColBoxList.clear();


		if(m_CharData.m_data.ObjList.size()>0)
		{
			for (DWORD dw = 0; dw < m_CharData.m_data.ObjList.size(); dw++)
			{
				for (DWORD ob = 0; ob < m_CharData.m_data.SknNameList.size(); ob++)
				{
					ObjLoad(m_CharData.m_data.ObjList[dw].objdata.SknNameList[0]);
				}

				if (m_CharData.m_data.ObjList[dw].objdata.m_CharType == CHAR_FRAMETYPE)
				{
					m_ObjectList[dw]->SetStateNum(m_CharData.m_data.ObjList[dw].objdata.dwStateNum);

					for (DWORD dwState = 0; dwState < m_CharData.m_data.ObjList[dw].objdata.dwStateNum; dwState++)
					{
						SetCharAction(dwState, m_CharData.m_data.ObjList[dw].objdata.StateList[dwState].m_iStartFrame,
							m_CharData.m_data.ObjList[dw].objdata.StateList[dwState].m_iEndFrame);
					}

					for (DWORD ob = 0; ob < m_CharData.m_data.MtrList.size(); ob++)
					{
						m_ObjectList[dw]->BoneLoad(m_CharData.m_data.ObjList[dw].objdata.MtrList[ob].MtrName);
					}
				}
				else
				{
					m_ObjectList[dw]->SetStateNum(m_CharData.m_data.ObjList[dw].objdata.dwStateNum);

					for (DWORD ob = 0; ob < m_CharData.m_data.ObjList[dw].objdata.MtrList.size(); ob++)
					{
						if (m_CharData.m_data.ObjList[dw].objdata.MtrList[ob].State == -1)
						{
							m_ObjectList[dw]->BoneLoad(m_CharData.m_data.ObjList[dw].objdata.MtrList[ob].MtrName);
						}
						else
						{
							m_ObjectList[dw]->SetMTRApply(m_CharData.m_data.ObjList[dw].objdata.MtrList[ob].MtrName,
								m_CharData.m_data.ObjList[dw].objdata.MtrList[ob].State);
						}
					}
				}

				SetObjectSocket(m_CharData.m_data.ObjList[dw].Socket, m_CharData.m_data.ObjList[dw].ObjSocket, m_CharData.m_data.ObjList[dw].FireSocket, dw);
				m_ObjectList[dw]->SetState(0);
				m_ObjectList[dw]->SetMovePos(m_CharData.m_data.ObjList[dw].vpos);
				m_ObjectList[dw]->SetRot(m_CharData.m_data.ObjList[dw].vRot);
				m_ObjectList[dw]->SetScale(m_CharData.m_data.ObjList[dw].fScale);
			}
		}
		else if (m_CharData.m_data.m_dwWeapon>0)
		{
			m_dwFireSocket = m_CharData.m_data.m_dwFireSocket;
		}
			 

		SetState(0);
		return true;
	}

	bool CBY_Character::ObjLoad(T_STR pszSkinLoad)
	{
		std::shared_ptr<CBY_Weapon> obj = std::make_shared<CBY_Weapon>();
		obj->Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/SkinShader.txt", nullptr, "VSSKIN", "PS");
		obj->SkinLoad(pszSkinLoad);
		//obj->BoneLoad(pszMtrLoad);
		m_ObjectList.push_back(obj);
		return true;
	}

	bool CBY_Character::Init()
	{
		CBY_Object::Init();
		return true;
	}

	bool CBY_Character::Frame()
	{
		m_fElapseTick = 0;
		if (m_dwAniType == CHAR_FRAMETYPE)
		{
			m_Bone->Update(m_StateList[m_dwState].m_iStartFrame, m_StateList[m_dwState].m_iEndFrame,
				m_fElapseTick, m_pMatrixList);
		}

		else if (m_dwAniType == CHAR_MTRTYPE)
		{
			m_Bone->MTRUpdate(0, 0, 0, m_pMatrixList);
		}

		for (int iBone = 0; iBone < m_ObjList[0]->m_matBoneBindPoss.size(); iBone++)
		{
			D3DXMATRIX mat = m_ObjList[0]->m_matBoneBindPoss[iBone] * m_pMatrixList[iBone];		//바인드 포즈는 공통이다
			D3DXMatrixTranspose(&m_cbBoneWorld.g_matConstBoneWorld[iBone], &mat);
		}

		D3D11_MAPPED_SUBRESOURCE mapResource;
		m_obj.m_pContext->Map(m_pCBConstBoneWorld.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
		memcpy(mapResource.pData, &m_cbBoneWorld, sizeof(CBConstBoneWorld));
		m_obj.m_pContext->Unmap(m_pCBConstBoneWorld.Get(), 0);

		return true;
	}

	void CBY_Character::SetObjectSocket(int iSocket, int iObjSocket, int iFireSocket,int iobj)
	{
		if (m_ObjectList.size() < 0)
		{
			return;
		}
		m_ObjectList[iobj]->SetSocket(iSocket);
		m_ObjectList[iobj]->SetObjSocke(iObjSocket);
		m_ObjectList[iobj]->SetFireSocket(iFireSocket);
	}

	bool CBY_Character::Render()
	{
		PreRender();
		CharPostRender();
		for (int i = 0; i < m_ObjectList.size(); i++)
		{
			m_ObjectList[i]->Render();
		}
		//{
		//	m_CharBox.SetMatrix(nullptr, &m_matView, &m_matProj);		//디버깅용
		//	m_CharBox.Render();
			/*for (int iBox = 0; iBox < m_BoxList.size(); iBox++)
			{
				m_BoxList[iBox].SetMatrix(nullptr, &m_matView, &m_matProj);
				m_BoxList[iBox].Render();
			}*/
		

		ID3D11ShaderResourceView* ppSRVNULL[1] = { NULL };
		m_obj.m_pContext->PSSetShaderResources(0, 1, ppSRVNULL);
		m_obj.m_pContext->PSSetShaderResources(1, 1, ppSRVNULL);
		m_obj.m_pContext->PSSetShaderResources(2, 1, ppSRVNULL);
		m_obj.m_pContext->PSSetShaderResources(3, 1, ppSRVNULL);
		m_obj.m_pContext->PSSetShaderResources(4, 1, ppSRVNULL);
		m_obj.m_pContext->PSSetShaderResources(5, 1, ppSRVNULL);

		if (g_bDebug)
		{
			m_CharBox.Render();
		}
		return true;
	}

	bool CBY_Character::Release()
	{
		CBY_Object::Release();

		m_ObjList.clear();
		for (int i = 0; i < m_ObjectList.size(); i++)
		{
			m_ObjectList[i]->Release();
		}
		m_CharBox.Release();
		return true;
	}

	bool CBY_Character::NonCharacterRender()
	{
		PreRender();
		for (int i = 0; i < m_ObjectList.size(); i++)
		{
			m_ObjectList[i]->Render();
		}
		//for (int iBox = 0; iBox < m_BoxList.size(); iBox++)
		//{
			//m_BoxList[iBox].SetMatrix(nullptr, &m_matView, &m_matProj);
			//m_BoxList[iBox].Render();
		//}
		return true;
	}

	void CBY_Character::ColBoxUpdate(D3DXMATRIX world)
	{
		for (int iBox = 0; iBox < m_BoxList.size(); iBox++)
		{
			D3DXMATRIX matworld, matRot;
			matworld = m_pMatrixList[m_BoxList[iBox].GetBoneIndex()] * (world);
			D3DXVECTOR3 vScale, vPos, vSize;
			D3DXQUATERNION qRot;
			D3DXMatrixDecompose(&vScale, &qRot, &vPos, &matworld);
			D3DXMatrixRotationQuaternion(&matRot, &qRot);

			vSize = m_BoxList[iBox].GetInitBoxSize();
			vSize.x *= vScale.x;
			vSize.y *= vScale.y;
			vSize.z *= vScale.z;
			//vPos += m_BoxList[iBox].GetInitPos();

			m_BoxList[iBox].CreateBox(m_BoxList[iBox].GetBoneIndex(),
				vPos, abs(vSize.x), abs(vSize.y), abs(vSize.z), matRot);

			m_BoxList[iBox].UpdateBoxAxis(matRot);
		}
	}

	void CBY_Character::SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
	{
		KG_Model::SetMatrix(world, view, proj);
	
		if (world != nullptr)
		{
			if (m_iRoot != -1)
			{
				D3DXMATRIX matWorld,matTrans;
				D3DXMatrixIdentity(&matTrans);
				matTrans._41 = m_pMatrixList[m_iRoot]._41 - m_matRootPos._41;
				matTrans._42 = m_pMatrixList[m_iRoot]._42 - m_matRootPos._42;
				matTrans._43 = m_pMatrixList[m_iRoot]._43 - m_matRootPos._43;
				matWorld = (*world)*matTrans;
				//matWorld._41 += m_pMatrixList[m_iRoot]._41;
				//matWorld._42 += m_pMatrixList[m_iRoot]._42;
				//matWorld._43 += m_pMatrixList[m_iRoot]._43;
				BoxUpdate(matWorld);
				ColBoxUpdate(matWorld);
			}
			else
			{
				BoxUpdate(*world);
				ColBoxUpdate(*world);
			}
		}
		SetRayUpdate();
		for (int i = 0; i < m_ObjectList.size(); i++)
		{
			m_ObjectList[i]->SetMatrix(&(m_matWorld), &m_matView, &m_matProj);
		}
	}

	void CBY_Character::SetRayOrigin(D3DXVECTOR3 pos)
	{
		D3DXVECTOR3 vpos = pos + (m_CharBox.GetBox().vAxis[2] * m_CharBox.GetBox().fExtent[2]);
		D3DXVECTOR3 vTrans = m_CharBox.GetBox().vAxis[0] * (m_CharBox.GetBox().fExtent[0] / 2.0f);
		m_RayCast[RAY_FRONT][0].m_Orig = vpos;
		m_RayCast[RAY_FRONT][1].m_Orig = vpos;
		m_RayCast[RAY_FRONT][1].m_Orig -= vTrans;
		m_RayCast[RAY_FRONT][2].m_Orig = vpos;
		m_RayCast[RAY_FRONT][2].m_Orig += vTrans;

		vpos = pos - (m_CharBox.GetBox().vAxis[2] * m_CharBox.GetBox().fExtent[2]);
		m_RayCast[RAY_BACK][0].m_Orig = vpos;
		m_RayCast[RAY_BACK][1].m_Orig = vpos;
		m_RayCast[RAY_BACK][1].m_Orig -= vTrans;
		m_RayCast[RAY_BACK][2].m_Orig = vpos;
		m_RayCast[RAY_BACK][2].m_Orig += vTrans;

		vpos = pos - (m_CharBox.GetBox().vAxis[0] * m_CharBox.GetBox().fExtent[0]);
		vTrans = m_CharBox.GetBox().vAxis[2] * (m_CharBox.GetBox().fExtent[2] / 2.0f);
		m_RayCast[RAY_LEFT][0].m_Orig = vpos;
		m_RayCast[RAY_LEFT][1].m_Orig = vpos;
		m_RayCast[RAY_LEFT][1].m_Orig -= vTrans;
		m_RayCast[RAY_LEFT][2].m_Orig = vpos;
		m_RayCast[RAY_LEFT][2].m_Orig += vTrans;

		vpos = pos + (m_CharBox.GetBox().vAxis[0] * m_CharBox.GetBox().fExtent[0]);
		m_RayCast[RAY_RIGHT][0].m_Orig = vpos;
		m_RayCast[RAY_RIGHT][1].m_Orig = vpos;
		m_RayCast[RAY_RIGHT][1].m_Orig -= vTrans;
		m_RayCast[RAY_RIGHT][2].m_Orig = vpos;
		m_RayCast[RAY_RIGHT][2].m_Orig += vTrans;
	}

	void CBY_Character::SetRayUpdate()
	{
		for (int i = 0; i < RAY_DIRSIZE; i++)
		{
			m_RayCast[RAY_FRONT][i].m_Dir = -m_CharBox.GetBox().vAxis[2];
			//m_RayCast[RAY_FRONT][i].m_fMax = m_CharBox.GetBox().fExtent[2] + m_vRaySize[RAY_FRONT];
			m_RayCast[RAY_FRONT][i].m_fMax = m_vRaySize[RAY_FRONT];
		}

		for (int i = 0; i < RAY_DIRSIZE; i++)
		{
			m_RayCast[RAY_BACK][i].m_Dir = m_CharBox.GetBox().vAxis[2];
			//m_RayCast[RAY_BACK][i].m_fMax = m_CharBox.GetBox().fExtent[2] + m_vRaySize[RAY_BACK];
			m_RayCast[RAY_BACK][i].m_fMax =  m_vRaySize[RAY_BACK];
		}

		for (int i = 0; i < RAY_DIRSIZE; i++)
		{
			m_RayCast[RAY_LEFT][i].m_Dir = m_CharBox.GetBox().vAxis[0];
			//m_RayCast[RAY_LEFT][i].m_fMax = m_CharBox.GetBox().fExtent[0] + m_vRaySize[RAY_LEFT];
			m_RayCast[RAY_LEFT][i].m_fMax = m_vRaySize[RAY_LEFT];
		}

		for (int i = 0; i < RAY_DIRSIZE; i++)
		{
			m_RayCast[RAY_RIGHT][i].m_Dir = -m_CharBox.GetBox().vAxis[0];
			//m_RayCast[RAY_RIGHT][i].m_fMax = m_CharBox.GetBox().fExtent[0] + m_vRaySize[RAY_RIGHT];
			m_RayCast[RAY_RIGHT][i].m_fMax = m_vRaySize[RAY_RIGHT];
		}
		SetRayOrigin(m_CharBox.GetBox().vCenter);
	}

	void CBY_Character::SetCharBox()
	{
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);
		D3DXVECTOR3 size = m_SkinOriginalBox.vMax - m_SkinOriginalBox.vCenter;
		//size.y /= 2.0f;
		//m_SkinOriginalBox.vCenter.y += size.y;
		m_CharBox.Create(m_obj.m_pd3dDevice, m_obj.m_pContext, L"../../data/shader/DefaultShader.txt", nullptr, "VSmat", "PSVC");	//디버깅용 박스
		m_CharBox.CreateBox(0,
			m_ObjList[0]->m_ObjList[0]->m_CharBox.vCenter, size.x, size.y, size.z, mat);
		m_CharBox.SetInitBoxSize(size);
		SetRayUpdate();
	}

	void CBY_Character::SetCharAction(int state, float start, float end)
	{

		if (state >= m_CharData.m_data.StateList.size())
		{
			Char_State_Info info;
			info.m_iStartFrame = start;
			info.m_iEndFrame = end;
			m_CharData.m_data.StateList.push_back(info);
		}
		else
		{
			m_CharData.m_data.StateList[state].m_iStartFrame = start;
			m_CharData.m_data.StateList[state].m_iEndFrame = end;
		}

		SetFrameTime(state, start, end);
	}

	void CBY_Character::SetRootMat()
	{
		m_Bone->MTRUpdate(0, 0, 0, m_pMatrixList);
		m_matRootPos = m_pMatrixList[m_iRoot];
	}

	D3DXVECTOR3 CBY_Character::GetFirePos(int iObj)
	{

		if (m_ObjectList.size() > 0) //캐릭터가 오브젝트를 개별로 들고 있을경우
		{
			if (m_ObjectList[iObj] != nullptr)
				return m_ObjectList[iObj]->GetFirePos();
		}
		else						//캐릭터의 메쉬가 무기와 합쳐져 있을 경우
		{
			D3DXMATRIX matpos;
			D3DXMatrixIdentity(&matpos);
			matpos._41 = m_pMatrixList[m_dwFireSocket]._41;
			matpos._42 = m_pMatrixList[m_dwFireSocket]._42;
			matpos._43 = m_pMatrixList[m_dwFireSocket]._43;
			matpos *= m_matWorld;
			
			D3DXVECTOR3 pos;
			pos.x = matpos._41;
			pos.y = matpos._42;
			pos.z = matpos._43;
			return pos;
		}
	}

	D3DXVECTOR3 CBY_Character::GetColPos(int i)
	{
		return m_BoxList[i].GetPos();
	}

	CBY_Character::CBY_Character()
	{
		D3DXMatrixIdentity(&m_matRootPos);
		m_iRoot = -1;
		m_vRaySize = D3DXVECTOR4(0, 0, 0, 0);
	}


	CBY_Character::~CBY_Character()
	{
	}



}

