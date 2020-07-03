#pragma once
#include "C_ObjectMT.h"

class C_Damage:public C_MonsterStateProcess
{
public:
	void Process(vector<C_Tower>& Tower);
	float motiontime;
	float damagetime;

public:
	C_Damage(C_Monster* host);
	~C_Damage();
};

