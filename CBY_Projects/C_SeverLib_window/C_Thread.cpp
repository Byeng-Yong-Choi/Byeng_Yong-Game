#include "C_Thread.h"

unsigned int WINAPI C_Thread::ThreadRun(LPVOID prameter)
{
	C_Thread* pThread = (C_Thread*)prameter;
	pThread->RunThread();
	return 0;
}

void C_Thread::CreateThread()
{
	m_hThread = _beginthreadex(NULL, 0, ThreadRun, this, 0, &m_uiID);
}

bool C_Thread::RunThread()
{

	return true;
}

C_Thread::C_Thread()
{
	m_hThread = 0;
	m_uiID = 0;
	m_bStart = false;
}


C_Thread::~C_Thread()
{
}
