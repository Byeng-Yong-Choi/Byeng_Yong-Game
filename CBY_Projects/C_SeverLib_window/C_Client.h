#pragma once
#include "C_Network.h"

class C_Client :public C_Network
{
public:

public:
	bool Connect(int iPort, const char* ip, int iSockType = SOCK_STREAM, int iMode = TRUE);
	virtual bool RunThread() override;
	virtual bool Run() override;

public:
	C_Client();
	~C_Client();
};

