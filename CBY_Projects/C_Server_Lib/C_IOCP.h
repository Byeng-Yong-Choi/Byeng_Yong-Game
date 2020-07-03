#pragma once
#include "C_WorkThread.h"
#define WORKER_THREAD_COUNT 3

class C_IOCP: public Singleton<C_IOCP>
{
private:
	friend class Singleton<C_IOCP>;

public:
	HANDLE m_hIOCP;
	C_WorkThread m_WorkThread[WORKER_THREAD_COUNT];

public:
	bool Init();
	void AddToIocp(HANDLE hSock, ULONG_PTR returnkey);

private:
	C_IOCP();

public:
	~C_IOCP();
};

#define I_IOCP C_IOCP::GetInstance()
