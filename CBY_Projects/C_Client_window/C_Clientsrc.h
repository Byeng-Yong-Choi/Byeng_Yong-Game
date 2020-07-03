#pragma once
#include "C_Client.h"

class C_Clientsrc: public C_Client
{
public:
	FD_SET rSet;
	FD_SET wSet;

public:
	bool Init();
	void SetFunction();
	bool RunThread() override;
	bool Run() override;
	bool SelectRun();

public:
	static void RecvMsg(USERPAKET& pk);

public:
	C_Clientsrc();
	~C_Clientsrc();
};

