#pragma once
#include "C_Network.h"
class C_Server:public C_Network
{
public:
	int			m_SockType;
	bool		m_bConnect;
	std::string	m_szAddress;
	int			m_iPort;

public:
	bool Connect(int iPort,const char* ip,int iSockType= SOCK_STREAM,int iMode=TRUE);

public:
	C_Server();
	~C_Server();
};

