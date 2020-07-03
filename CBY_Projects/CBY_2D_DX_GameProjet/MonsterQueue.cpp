#include "MonsterQueue.h"
void MonsterQueue::Push(C_Monster* pobj)
{
	m_List.push(pobj);
}

C_Monster* MonsterQueue::Pop()
{
	C_Monster* pobj = nullptr;
	if (m_List.empty() == false)
	{
		pobj = m_List.front();
		m_List.pop();
	}

	return pobj;
}

MonsterQueue::MonsterQueue()
{
}


MonsterQueue::~MonsterQueue()
{
}
