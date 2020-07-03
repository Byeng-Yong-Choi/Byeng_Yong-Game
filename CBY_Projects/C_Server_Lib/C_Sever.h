#pragma once
#include "C_Acceptor.h"
#include "C_User.h"
#include "C_ODBC.h"

struct C_PACKET
{
	C_User* pUser;
	USERPAKET packet;
};

class C_Sever:public Singleton<C_Sever>, public C_Thread
{
private:
	friend class Singleton<C_Sever>;

public:
	C_Acceptor m_Acceptor;
	std::vector<C_PACKET> m_SendPackPool;
	C_ODBC m_ODBC;

public:
	bool Init();
	bool Release();
	void SendUserList(C_User* pUser);
	void SendPacket(C_User* pUser, std::stringstream& sendStream);
	virtual bool Run() override;

private:
	C_Sever();
public:
	~C_Sever();
};

#define I_Server C_Sever::GetInstance()

