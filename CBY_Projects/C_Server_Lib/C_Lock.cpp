#include "C_Lock.h"



C_Lock::C_Lock(C_Object* pobj):m_pSeverobj(pobj)
{
	if (m_pSeverobj == nullptr)
	{
		return;
	}

	EnterCriticalSection(&m_pSeverobj->m_CS);
}


C_Lock::~C_Lock()
{
	if (m_pSeverobj == nullptr)
	{
		return;
	}
	LeaveCriticalSection(&m_pSeverobj->m_CS);
}
