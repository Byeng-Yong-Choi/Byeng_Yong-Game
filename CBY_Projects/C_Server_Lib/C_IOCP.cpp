#include "C_IOCP.h"

bool C_IOCP::Init()
{
	m_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, WORKER_THREAD_COUNT);

	for (int i = 0; i < WORKER_THREAD_COUNT; i++)
	{
		m_WorkThread[i].CreateThread();
	}
	return true;
}

void C_IOCP::AddToIocp(HANDLE hSock, ULONG_PTR returnkey)
{
	::CreateIoCompletionPort(hSock, m_hIOCP, returnkey, 0);
}


C_IOCP::C_IOCP()
{
}


C_IOCP::~C_IOCP()
{
	CloseHandle(m_hIOCP);
}
