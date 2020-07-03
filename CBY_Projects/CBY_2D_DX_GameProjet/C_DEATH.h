#pragma once
#include "C_ObjectMT.h"

class C_DEATH :public C_MonsterStateProcess
{
public:
	void Process(std::vector<std::shared_ptr<C_Tower>>& Tower);
	float fDeathTime;

public:
	C_DEATH(C_Monster* host);
	~C_DEATH();
};

