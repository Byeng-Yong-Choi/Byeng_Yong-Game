#pragma once
#include "C_Lock.h"

class C_Queue:public C_Object
{
public:
	std::queue<C_Object*> m_List;
	void Push(C_Object* pobj);
	C_Object* Pop();

public:
	C_Queue();
	virtual ~C_Queue();
};

