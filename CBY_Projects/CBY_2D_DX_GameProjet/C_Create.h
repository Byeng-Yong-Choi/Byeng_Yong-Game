#pragma once
#include "C_ObjectMT.h"

class C_Create :public C_TowerStateProcess
{
public:
	void Process();

public:
	C_Create(C_Tower* host);
	~C_Create();
};

