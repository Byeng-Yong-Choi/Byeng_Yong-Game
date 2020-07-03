#include "C_EffectQueue.h"

void C_EffectQueue::Push(C_EffectSrc* pobj)
{
	m_List.push(pobj);
}

C_EffectSrc* C_EffectQueue::Pop()
{
	C_EffectSrc* pobj = nullptr;
	if (m_List.empty() == false)
	{
		pobj = m_List.front();
		m_List.pop();
	}

	return pobj;
}

C_EffectQueue::C_EffectQueue()
{
}


C_EffectQueue::~C_EffectQueue()
{
}
