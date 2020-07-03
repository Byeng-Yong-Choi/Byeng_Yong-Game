#pragma once
#include "C_ObjectMT.h"

class C_Null :public C_TowerStateProcess
{
public:
	void Process();

public:
	C_Null(C_Tower* host);
	~C_Null();
};

