#include "C_Acceptor.h"
#include "C_SessionMgr.h"

bool C_Acceptor::Set(int iPort, const char* Address)
{
	m_ListenSock = socket(AF_INET, SOCK_STREAM, 0);

	int optval = 1;
	int iRet = setsockopt(m_ListenSock, SOL_SOCKET,
		SO_REUSEADDR,
		(char*)&optval, sizeof(optval));

	int option = 1;
	iRet = setsockopt(m_ListenSock, IPPROTO_TCP,
		TCP_NODELAY, (char*)&option, sizeof(option));

	LINGER ling = { 0, };
	ling.l_onoff = 1;
	ling.l_linger = 1000;
	iRet = setsockopt(m_ListenSock, SOL_SOCKET,
		SO_LINGER, (char*)&ling, sizeof(ling));

	int iSize;
	int typelen = sizeof(iSize);
	iRet = getsockopt(m_ListenSock, SOL_SOCKET,
		SO_SNDBUF, (char*)&iSize, &typelen);
	iRet = getsockopt(m_ListenSock, SOL_SOCKET,
		SO_RCVBUF, (char*)&iSize, &typelen);

	iSize = 0;
	iRet = setsockopt(m_ListenSock, SOL_SOCKET,
		SO_RCVBUF, (char*)&iSize, typelen);

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(SOCKADDR_IN));
	serveraddr.sin_family = AF_INET;
	if (Address == 0)
	{
		serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		serveraddr.sin_addr.s_addr = inet_addr(Address);
	}
	serveraddr.sin_port = htons(iPort);
	bind(m_ListenSock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	listen(m_ListenSock, SOMAXCONN);
	CreateThread();
	return true;
}

bool C_Acceptor::Run()
{
	SOCKET clientSock;
	SOCKADDR_IN address;
	int addlen = sizeof(address);
	while (m_bStarted)
	{
		clientSock = accept(m_ListenSock, (SOCKADDR*)&address, &addlen);
		C_User* user=I_SessionMgr.CreateNewSession(clientSock, address);
		Sleep(1);
	}
	m_bStarted = false;
	return true;
}

C_Acceptor::C_Acceptor()
{
}


C_Acceptor::~C_Acceptor()
{
}
