#pragma once
#include "C_ObjectMT.h"
#include <queue>

class MonsterQueue :public C_Monster
{
public:
	std::queue<C_Monster*> m_List;
	void Push(C_Monster* pobj);
	C_Monster* Pop();

public:
	MonsterQueue();
	virtual ~MonsterQueue();
};



