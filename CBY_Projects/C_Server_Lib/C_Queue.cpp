#include "C_Queue.h"

void C_Queue::Push(C_Object* pobj)
{
	{
		C_Lock Lock(this);
		m_List.push(pobj);
	}
}

C_Object* C_Queue::Pop()
{
	C_Object* pobj = nullptr;
	if (m_List.empty() == false)
	{
		C_Lock Lock(this);
		pobj = m_List.front();
		m_List.pop();
	}

	return pobj;
}

C_Queue::C_Queue()
{
}


C_Queue::~C_Queue()
{
}
