#pragma once
#include "C_ObjectMT.h"

class C_ATTACK:public C_TowerStateProcess
{
public:
	void Process(list<C_Monster*>& obj);
public:
	

public:
	C_ATTACK(C_Tower* host);
	~C_ATTACK();
};

