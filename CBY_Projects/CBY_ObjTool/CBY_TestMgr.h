#pragma once
#include "CBY_TestBoneObj.h"
#include "CBY_TestSkinObj.h"

class CBY_TestMgr : public Singleton<CBY_TestMgr>
{
public:
	std::map<int, CBY_TestSkinObj*> m_SkinLoadList;
	std::map<int, CBY_TestBoneObj*> m_BoneLoadList;
	std::wstring	m_szPath;
	int				m_iSkinIndex;
	int				m_iBoneIndex;
	HFONT   m_hDefaultFont;

public:
	bool Release();
	int SkinLoad(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	int BoneLoad(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
public:
	void SkinDelete(T_STR pszLoad);
	void BoneDelete(T_STR pszLoad);

public:
	CBY_TestMgr();
	virtual ~CBY_TestMgr();
};

#define I_TEST CBY_TestMgr::GetInstance()
