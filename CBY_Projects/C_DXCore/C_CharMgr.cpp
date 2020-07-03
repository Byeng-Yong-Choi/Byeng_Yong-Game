#include "C_CharMgr.h"
int C_CharMgr::SkinLoad(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{

	std::wstring nameExt = pszLoad;

	std::map<int, C_SkinObj*>::iterator iter;
	for (iter = m_SkinLoadList.begin(); iter != m_SkinLoadList.end(); iter++)
	{
		if (iter->second->m_szname == nameExt)
		{
			return iter->first;
		}
	}

	std::wstring loadfile;
	loadfile = nameExt; //m_szPath + ;

	C_SkinObj* pData = new C_SkinObj;
	pData->m_szname = nameExt;

	if (pData->Load(loadfile.c_str(), pd3dDevice, Context) >= 0)
	{
		m_SkinLoadList.insert(std::make_pair(m_iSkinIndex, pData));
		return m_iSkinIndex++;
	}
	delete pData;
	return m_iSkinIndex++;
}

int C_CharMgr::BoneLoad(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	std::wstring nameExt = pszLoad;

	std::map<int, C_BoneObj*>::iterator iter;
	for (iter = m_BoneLoadList.begin(); iter != m_BoneLoadList.end(); iter++)
	{
		if (iter->second->m_szname == nameExt)
		{
			return iter->first;
		}
	}

	std::wstring loadfile;
	loadfile = m_szPath + nameExt;

	C_BoneObj* pData = new C_BoneObj;
	pData->m_szname = nameExt;

	if (pData->Load(loadfile.c_str(), pd3dDevice, Context) >= 0)
	{
		m_BoneLoadList.insert(std::make_pair(m_iBoneIndex, pData));
		return m_iBoneIndex++;
	}
	delete pData;
	return m_iBoneIndex++;
}

void C_CharMgr::SkinDelete(T_STR pszLoad)
{
	std::wstring nameExt = pszLoad;
	std::map<int, C_SkinObj*>::iterator iter;
	for (iter = m_SkinLoadList.begin(); iter != m_SkinLoadList.end(); iter++)
	{
		if (iter->second->m_szname == nameExt)
		{
			m_SkinLoadList.erase(iter);
			return;
		}
	}
}

void C_CharMgr::BoneDelete(T_STR pszLoad)
{
	std::wstring nameExt = pszLoad;
	std::map<int, C_BoneObj*>::iterator iter;
	for (iter = m_BoneLoadList.begin(); iter != m_BoneLoadList.end(); iter++)
	{
		if (iter->second->m_szname == nameExt)
		{
			m_BoneLoadList.erase(iter);
			return;
		}
	}
}

bool C_CharMgr::Release()
{
	for(auto iter : m_SkinLoadList)
	{
		C_SkinObj* temp = iter.second;
		temp->Release();
		delete temp;
	}

	for (auto iter : m_BoneLoadList)
	{
		C_BoneObj* temp = iter.second;
		temp->Release();
		delete temp;
	}


	m_BoneLoadList.clear();
	m_SkinLoadList.clear();
	return true;
}

C_CharMgr::C_CharMgr()
{
	//m_szPath = L"../../data/Char/";
	m_iSkinIndex = 0;
	m_iBoneIndex = 0;
}


C_CharMgr::~C_CharMgr()
{
	Release();
}
