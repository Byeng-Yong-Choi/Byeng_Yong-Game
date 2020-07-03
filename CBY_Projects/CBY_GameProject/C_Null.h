#pragma once
#include "C_ObjectMT.h"

class C_Null :public C_TowerStateProcess
{
public:
	void Process(list<C_Monster*>& obj);

public:
	C_Null(C_Tower* host);
	~C_Null();
};

