#pragma once
#include "C_ObjectMT.h"

class C_Damage:public C_MonsterStateProcess
{
public:
	void Process(std::vector<std::shared_ptr<C_Tower>>& Tower);
	float fDamagetime;

public:
	C_Damage(C_Monster* host);
	~C_Damage();
};
