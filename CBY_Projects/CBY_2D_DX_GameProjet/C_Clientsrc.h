#pragma once
#include "C_Client.h"

class C_Clientsrc: public C_Client
{
public:
	FD_SET rSet;
	FD_SET wSet;
	char m_szID[10];
	char m_szPS[10];

public:
	bool Init();
	void SetFunction();
	bool RunThread() override;
	bool Run() override;
	bool SelectRun();

public:
	static void RecvMsg(USERPAKET& pk);
	static void CreateIDSuccess(USERPAKET& pk);
	static void CreateIDFail(USERPAKET& pk);
	static bool Login;
	static bool CreateID;

public:
	C_Clientsrc();
	~C_Clientsrc();
};

