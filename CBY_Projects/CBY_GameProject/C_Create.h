#pragma once
#include "C_ObjectMT.h"

class C_Create :public C_TowerStateProcess
{
public:
	void Process(list<C_Monster*>& obj);

public:
	C_Create(C_Tower* host);
	~C_Create();
};

