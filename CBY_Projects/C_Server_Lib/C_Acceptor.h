#pragma once
#include "C_Thread.h"

class C_Acceptor:public C_Thread
{
public:
	SOCKET m_ListenSock;

public:
	virtual bool Set(int iPort, const char* Address = 0);
	virtual bool Run() override;

public:
	C_Acceptor();
	~C_Acceptor();
};

