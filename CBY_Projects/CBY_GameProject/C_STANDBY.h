#pragma once
#include "C_ObjectMT.h"

class C_STANDBY :public C_MonsterStateProcess
{
public:
	void Process(vector<C_Tower>& Tower);

public:
	C_STANDBY(C_Monster* host);
	~C_STANDBY();
};

