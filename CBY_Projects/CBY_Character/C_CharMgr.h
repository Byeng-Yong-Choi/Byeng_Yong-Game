#pragma once
#include "C_SkinObj.h"
#include "C_BoneObj.h"

class C_CharMgr :public Singleton<C_CharMgr>
{
public:
	std::map<int, C_SkinObj*> m_SkinLoadList;
	std::map<int, C_BoneObj*> m_BoneLoadList;
	std::wstring	m_szPath;
	int				m_iSkinIndex;
	int				m_iBoneIndex;
	HFONT   m_hDefaultFont;

public:

	bool Release();
	int SkinLoad(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	int BoneLoad(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);

public:
	C_CharMgr();
public:
	~C_CharMgr();
};

#define I_CHARACTER C_CharMgr::GetInstance()


