#pragma once
#include "C_NetBase.h"

class C_Lock
{
public:
	C_NetBase* m_pBase;

public:
	C_Lock(C_NetBase* pBase);
	~C_Lock();
};

