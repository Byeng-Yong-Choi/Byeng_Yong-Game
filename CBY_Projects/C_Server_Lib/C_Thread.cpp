#include "C_Thread.h"

void C_Thread::CreateThread()
{
	if (m_bStarted)
	{
		return;
	}
	m_hThread = _beginthreadex(NULL, 0, HandleRunner, (LPVOID)this, 0, &m_iThreadID);
	m_bStarted = true;
}

bool C_Thread::Run()
{
	return true;
}

unsigned int WINAPI C_Thread::HandleRunner(LPVOID param)
{
	C_Thread* pThread = (C_Thread*)param;
	assert(pThread);
	pThread->Run();
	return 0;
}

C_Thread::C_Thread():m_bStarted(false), m_hThread(0)
{
	m_iThreadID = 0;
}

C_Thread::~C_Thread()
{

}