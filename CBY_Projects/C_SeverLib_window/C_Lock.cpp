#include "C_Lock.h"



C_Lock::C_Lock(C_NetBase* pBase) : m_pBase(pBase)
{
	if (m_pBase != nullptr)
	{
		EnterCriticalSection(&m_pBase->m_CS);
	}
}


C_Lock::~C_Lock()
{
	if (m_pBase != nullptr)
	{
		LeaveCriticalSection(&m_pBase->m_CS);
	}
}
