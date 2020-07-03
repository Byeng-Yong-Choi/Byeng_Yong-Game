#pragma once
#include "C_ObjectMT.h"

class C_Move :public C_MonsterStateProcess
{
public:
	int root;

public:
	void Process(vector<C_Tower>& Tower);

public:
	C_Move(C_Monster* host);
	~C_Move();
};

