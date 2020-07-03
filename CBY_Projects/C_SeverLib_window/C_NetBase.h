#pragma once
#include "C_NetStd.h"

class C_NetBase
{
public:
	CRITICAL_SECTION m_CS;

public:
	C_NetBase();
	~C_NetBase();
};

