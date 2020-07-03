#pragma once
#include "C_ObjectMT.h"

class C_TowerStandBy :public C_TowerStateProcess
{
public:
	void Process();

public:
	C_TowerStandBy(C_Tower* host);
	~C_TowerStandBy();
};

