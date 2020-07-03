#pragma once
#include "C_CBYParser_1.h"

class C_BoneParser:public C_CBYParser_1
{
public:
	CBY_Box m_CharBox;

public:
	bool Load(const TCHAR* szFileName);

public:
	C_BoneParser();
	virtual ~C_BoneParser();
};

