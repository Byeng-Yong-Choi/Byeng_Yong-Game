#pragma once
#include "C_ObjectMT.h"

class C_LevelUP :public C_TowerStateProcess
{
public:
	void Process();

public:
	C_LevelUP(C_Tower* host);
	~C_LevelUP();
};

