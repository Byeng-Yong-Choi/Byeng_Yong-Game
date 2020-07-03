#pragma once
#include "C_ObjectMT.h"

class C_Make :public C_MonsterStateProcess
{
public:
	float fStart;

public:
	void Process(std::vector<std::shared_ptr<C_Tower>>& Tower);

public:
	C_Make(C_Monster* host);
	~C_Make();
};

