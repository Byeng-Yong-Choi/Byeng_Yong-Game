#pragma once
#include "C_SkinObj.h"
#include "CBY_Parser.h"

class CBY_TestSkinObj:public C_SkinObj
{
public:
	CBY_Parser m_BLoader;

public:
	virtual bool Load(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context);
	virtual void ConvertSkin(std::vector<PNCT2IW_VERTEX>& list);
public:
	CBY_TestSkinObj();
	virtual ~CBY_TestSkinObj();
};

