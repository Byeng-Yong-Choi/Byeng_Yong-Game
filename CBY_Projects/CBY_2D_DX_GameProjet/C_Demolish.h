#pragma once
#include "C_ObjectMT.h"

class C_Demolish :public C_TowerStateProcess
{
public:
	void Process();

public:
	C_Demolish(C_Tower* host);
	~C_Demolish();
};

