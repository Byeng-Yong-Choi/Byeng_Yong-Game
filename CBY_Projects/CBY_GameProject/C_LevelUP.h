#pragma once
#include "C_ObjectMT.h"

class C_LevelUP :public C_TowerStateProcess
{
public:
	void Process(list<C_Monster*>& obj);

public:
	C_LevelUP(C_Tower* host);
	~C_LevelUP();
};

