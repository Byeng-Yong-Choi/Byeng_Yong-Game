#pragma once
#include "C_ObjectMT.h"

class C_MonClear :public C_MonsterStateProcess
{
public:
	void Process(vector<C_Tower>& Tower);

public:
	C_MonClear(C_Monster* host);
	~C_MonClear();
};


