#pragma once
#include "C_Object.h"

class C_Lock
{
private:
	C_Object* m_pSeverobj;

public:
	C_Lock(C_Object* pobj);
	virtual ~C_Lock();
};

