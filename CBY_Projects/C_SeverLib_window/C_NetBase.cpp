#include "C_NetBase.h"



C_NetBase::C_NetBase()
{
	InitializeCriticalSection(&m_CS);
}


C_NetBase::~C_NetBase()
{
	DeleteCriticalSection(&m_CS);
}
