#include "C_User.h"
#include "C_Network.h"

int C_User::AddPaket(SOCKET sock)
{
	if (m_iWirtenum < PACKET_HEADER_SIZE)
	{
		int RecvData;
		RecvData = recv(sock, m_pWirtepos, PACKET_HEADER_SIZE - m_iWirtenum, 0);
		if (RecvData == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
			else
			{
				return 0;
			}
		}
		m_iWirtenum += RecvData;
		m_pWirtepos = &m_userbuf[m_iWirtenum];
		if (m_iWirtenum == PACKET_HEADER_SIZE)
		{
			m_pRecvPacket = (USERPAKET*)m_userbuf;

			if (m_iWirtenum == m_pRecvPacket->ph.len)
			{
				USERPAKET packet;
				memcpy(&packet, m_pRecvPacket, sizeof(USERPAKET));
				C_Network::m_RecvPool.push_back(packet);
				std::cout << "User : " << m_userbuf + 4 << std::endl;

				ZeroMemory(&m_userbuf, PACKET_MAX_SIZE);
				m_iWirtenum = 0;
				m_iReadnum = 0;
				m_pWirtepos = &m_userbuf[m_iWirtenum];
			}
		}
	}
	else
	{
		m_iWirtenum += recv(sock, m_pWirtepos, m_pRecvPacket->ph.len - m_iWirtenum, 0);
		m_pWirtepos = &m_userbuf[m_iWirtenum];

		if (m_iWirtenum == m_pRecvPacket->ph.len)
		{
			USERPAKET packet;
			memcpy(&packet, m_pRecvPacket, sizeof(USERPAKET));
			C_Network::m_RecvPool.push_back(packet);
			std::cout << "User : " << m_userbuf + 4 << std::endl;

			ZeroMemory(&m_userbuf, PACKET_MAX_SIZE);
			m_iWirtenum = 0;
			m_iReadnum = 0;
			m_pWirtepos = &m_userbuf[m_iWirtenum];
		}
	}
	return 1;
	
}


C_User::C_User()
{
	ZeroMemory(&m_userbuf, PACKET_MAX_SIZE);
	m_iWirtenum = 0;
	m_iReadnum = 0;
	m_pWirtepos = m_userbuf;
}

C_User::C_User(const C_User& user)
{
	sock = user.sock;
	address = user.address;
	ZeroMemory(&m_userbuf, PACKET_MAX_SIZE);
	m_iWirtenum = 0;
	m_iReadnum = 0;
	m_pWirtepos = m_userbuf;
}


C_User::~C_User()
{
}
