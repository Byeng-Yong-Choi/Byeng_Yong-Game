#pragma once
#include "C_ObjectMT.h"

class C_Move :public C_MonsterStateProcess
{
public:

public:
	void Process(std::vector<std::shared_ptr<C_Tower>>& Tower);

public:
	C_Move(C_Monster* host);
	~C_Move();
};

