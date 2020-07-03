#pragma once
#include "C_BoneObj.h"
#include "CBY_Parser.h"

class CBY_TestBoneObj:public C_BoneObj
{
public:
	CBY_Parser m_BLoader;

public:
	virtual bool Load(T_STR pszLoad, ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context) override;
	virtual void Convert(std::vector<PNCT2IW_VERTEX>& list);

public:
	CBY_TestBoneObj();
	virtual ~CBY_TestBoneObj();
};

