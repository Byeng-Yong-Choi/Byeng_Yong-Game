#pragma once
#include "CBY_Object.h"
class CBY_ObjectTest:public CBY_Object
{
public:
	bool SkinLoad(T_STR pszLoad) override;
	bool BoneLoad(T_STR pszLoad) override;

public:
	CBY_ObjectTest();
	virtual ~CBY_ObjectTest();
};

