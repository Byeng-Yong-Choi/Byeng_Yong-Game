#include "C_Client.h"

bool C_Client::Connect(int iPort, const char* ip, int iSockType, int iMode)
{
	if (!CreateSock(iSockType, iMode))
	{
		return false;
	}

	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(SOCKADDR_IN));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(iPort);
	inet_pton(AF_INET, ip, &sa.sin_addr);

	int iRet = connect(m_Socket, (SOCKADDR*)&sa, sizeof(sa));

	return true;
}

bool C_Client::Run()
{
	if (m_Packet.AddPaket(m_Socket) > 0)
	{
		PacketProcess();
	}
	return true;
}

bool C_Client::RunThread()
{
	return true;
}


C_Client::C_Client()
{
	Init();
}


C_Client::~C_Client()
{
}
