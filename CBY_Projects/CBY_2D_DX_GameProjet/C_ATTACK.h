#pragma once
#include "C_ObjectMT.h"

class C_ATTACK:public C_TowerStateProcess
{
public:
	void Process();
public:
	

public:
	C_ATTACK(C_Tower* host);
	~C_ATTACK();
};

