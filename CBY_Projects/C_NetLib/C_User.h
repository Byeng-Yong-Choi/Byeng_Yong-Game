#pragma once
#include "C_NetStd.h"
class C_User
{
public:
	SOCKET sock;
	SOCKADDR_IN address;
	char m_userbuf[PACKET_MAX_SIZE];
	char* m_pWirtepos;
	int m_iWirtenum;
	int m_iReadnum;
	USERPAKET* m_pRecvPacket;

public:
	int AddPaket(SOCKET sock);

public:
	C_User();
	C_User(const C_User& user);
	~C_User();
};

