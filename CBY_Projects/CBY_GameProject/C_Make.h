#pragma once
#include "C_ObjectMT.h"

class C_Make :public C_MonsterStateProcess
{
public:
	void Process(vector<C_Tower>& Tower);

public:
	C_Make(C_Monster* host);
	~C_Make();
};

