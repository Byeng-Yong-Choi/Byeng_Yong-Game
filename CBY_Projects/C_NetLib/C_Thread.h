#pragma once
#include "C_Lock.h"
#include <process.h>

class C_Thread:public C_NetBase
{
public:
	unsigned int m_hThread;
	unsigned int m_uiID;
	bool	  m_bStart;

public:
	void CreateThread();
	void CreateThread(LPVOID parameter);
	virtual bool RunThread();

public:
	static unsigned int WINAPI ThreadRun(LPVOID prameter);

public:
	C_Thread();
	~C_Thread();
};

