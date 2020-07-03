#pragma once
#include "C_Object.h"

class C_Thread : public C_Object
{
public:
	unsigned int m_hThread;
	unsigned int m_iThreadID;
	bool		m_bStarted;
public:
	void		CreateThread();
	virtual bool Run();
	static unsigned int WINAPI HandleRunner(LPVOID param);
public:
	C_Thread();
	virtual ~C_Thread();
};