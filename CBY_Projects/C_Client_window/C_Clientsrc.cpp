#include "C_Clientsrc.h"

bool C_Clientsrc::Init()
{
	if (!Connect(10005, "192.168.0.49"))	//내서버
	//if (!Connect(10000, "192.168.0.111"))		//선생님 서버
	{
		return false;
	}
	return true;
}

void C_Clientsrc::RecvMsg(USERPAKET& pk)
{
	std::cout << "상대방 : " << pk.data << std::endl;
}

bool C_Clientsrc::SelectRun()
{
	FD_ZERO(&rSet);
	FD_ZERO(&wSet);

	FD_SET(m_Socket, &rSet);
	//FD_SET(m_Socket, &wSet);
	//if (C_Network::m_SendPool.size() != 0)		//패킷 직접 주고받을시
	if (C_Network::m_ssSendPool.size() != 0)		//스트링 이용
	{
		FD_SET(m_Socket, &wSet);
	}

	timeval time;
	time.tv_sec = 0;
	time.tv_usec = 1;
  	int iRet = select(0, &rSet, &wSet, NULL, &time);
	if (iRet == 0)		//타임아웃일 경우
	{
		return true;
	}

	if (FD_ISSET(m_Socket, &rSet))
	{
		if (m_Packet.AddPaket(m_Socket) > 0)
		{
			PacketProcess();
		}
	}

	if (FD_ISSET(m_Socket, &wSet))
	{
		{
			C_Lock Lock(&m_Lock);
		//for (auto& packet : C_Network::m_SendPool)	//패킷을 직접 넣을때
			for (auto& sspacket : C_Network::m_ssSendPool)	//스트링을 이용한 패킷 주고받기 사용시
			{
				C_Network::SendMSG(m_Socket, sspacket);
			}

		
			//C_Network::m_SendPool.clear();		//패킷 직접이용시
			C_Network::m_ssSendPool.clear();			//스트링 이용
		}
	}

	return true;
}

bool C_Clientsrc::Run()
{
	while (1)
	{
		SelectRun();
	}
	return true;
}

bool C_Clientsrc::RunThread()
{
	char buf[256] = { 0, };
	while (1)
	{
		ZeroMemory(buf, 256);
		fgets(buf, 256, stdin);
		if (buf[0] == '\n')
		{
			break;
		}
		{
			C_Lock Lock(&m_Lock);
			PushSendPool(buf, strlen(buf) - 1);
		}
	}
	closesocket(m_Socket);
	return 0;
}

void C_Clientsrc::SetFunction()
{
	m_fPacketFun[PACKET_CHAT_MSG] = RecvMsg;
}

C_Clientsrc::C_Clientsrc()
{
}


C_Clientsrc::~C_Clientsrc()
{
}
