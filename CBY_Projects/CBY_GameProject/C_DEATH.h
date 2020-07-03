#pragma once
#include "C_ObjectMT.h"

class C_DEATH :public C_MonsterStateProcess
{
public:
	void Process(vector<C_Tower>& Tower);

public:
	C_DEATH(C_Monster* host);
	~C_DEATH();
};

