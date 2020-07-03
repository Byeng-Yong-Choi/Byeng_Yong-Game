#include "C_User.h"
#include "C_Lock.h"
#include "C_Sever.h"
#include "C_SessionMgr.h"

std::ostream& operator << (std::ostream& stream, C_User& userInfo)
{
	C_Lock lock(&userInfo);
	tBlockWrite<DWORD>(stream, userInfo.m_ID);
	
	stream << userInfo.m_Name << std::endl;
	DWORD dwCnt = userInfo.m_pCharList.size();
	tBlockWrite<DWORD>(stream, dwCnt);

	for (auto& Char : userInfo.m_pCharList)
	{
		DWORD dwFirst = Char.first;
		tBlockWrite<DWORD>(stream, dwFirst);
		stream << Char.second;
	}
	return stream;
}

std::istream& operator >> (std::istream& stream, C_User& userInfo)
{
	C_Lock lock(&userInfo);
	tBlockRead<DWORD>(stream, userInfo.m_ID);
	stream >> userInfo.m_Name;
	BYTE tmp;
	tBlockRead<BYTE>(stream, tmp);
	DWORD iNumCharacter;
	tBlockRead<DWORD>(stream, iNumCharacter);

	DWORD dwID;
	C_Character Char;
	for (int i = 0; i < iNumCharacter; i++)
	{
		tBlockRead<DWORD>(stream, dwID);
		stream >> Char;
		userInfo.m_pCharList[dwID] = Char;
	}
	return stream;
}

void C_User::SetSocket(SOCKET sock, SOCKADDR_IN addr)
{
	m_Sock = sock;
	m_SockAddr = addr;
}

void C_User::Disconnect()
{
	closesocket(m_Sock);
}

bool C_User::RecvProcess()
{
	DWORD dwTrans;
	DWORD dwFlags = 0;
	COV* pOV = OverlappedRecv(COV::RECV);
	int iRet = WSARecv(m_Sock,&m_wsaRecvBuf,1,&dwTrans,&dwFlags,(LPOVERLAPPED)pOV,NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
	
}

bool C_User::RecvProcessZERO()
{
	DWORD dwTrans;
	DWORD dwFlags = 0;
	COV* pOV = OverlappedZERO(COV::ZERO_RECV);
	int iRet = WSARecv(m_Sock,	&m_wsaRecvBuf,1,&dwTrans,&dwFlags,(LPOVERLAPPED)pOV,NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}

bool C_User::SendProcess()
{
	C_Lock Lock(this);
	for (auto& iter : m_PackPool)
	{
		std::stringstream aa;
		aa << iter;

		for (auto& user : I_SessionMgr.m_UserList)
		{
			Send(user.second, aa);
		}
	}
	
	m_PackPool.clear();
	RecvProcessZERO();
	return true;

}

bool C_User::Send(C_User* pUser,std::stringstream& data)
{
	DWORD dwTrans;
	DWORD dwFlags = 0;

	COV* pOV = OverlappedSend(data);
	int iRet = WSASend(pUser->m_Sock, &m_wsaSendBuf, 1, &dwTrans, dwFlags, (LPOVERLAPPED)pOV, NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}

	return true;
}

bool C_User::Dispatch(DWORD dwTrans, COV* ov)
{
	if (ov->Type == COV::RECV && dwTrans == 0)
	{
		return false;
	}

	switch (ov->Type)
	{
	
	case COV::ZERO_RECV:
		{
		delete ov;
		RecvProcess();
		
		break;
		}

	case COV::RECV:
		{	
		delete ov;
		DWORD dwSize = dwTrans;
		AddPaket(dwTrans);
		break;
		}

	case COV::SEND:
		{	
		delete ov;
		//RecvProcessZERO();
		break;
		}
	
	}

	return true;
}

COV* C_User::OverlappedRecv(int Type)
{
	COV* ov = new COV(Type);
	m_wsaRecvBuf.buf = m_pWirtepos;
	m_wsaRecvBuf.len = PACKET_MAX_SIZE;
	return ov;
}

COV* C_User::OverlappedSend(std::stringstream& data)
{
	COV* ov = new COV(COV::SEND);
	ZeroMemory(&m_sendbuf, sizeof(char)*PACKET_MAX_SIZE);
	memcpy(&m_sendbuf, data.str().c_str(), data.str().length());
	m_wsaSendBuf.buf = m_sendbuf;
	m_wsaSendBuf.len = data.str().length();
	return ov;
}

COV* C_User::OverlappedZERO(int Type)
{
	COV* ov = new COV(Type);
	m_wsaRecvBuf.buf = nullptr;
	m_wsaRecvBuf.len = 0;
	return ov;
}

int C_User::AddPaket(DWORD dwSize)
{
	//m_pWirtepos 현재까지 읽은 주소
	//m_pRecvpos 패킷으로 빼고 나서 첫번째 주소

	m_iWirtenum += dwSize;
	m_pWirtepos = &m_userbuf[m_iWirtenum];

	if (PACKET_MAX_SIZE - m_iWirtenum < PACKET_HEADER_SIZE)
	{
		DWORD dwtemp = m_pWirtepos - m_pRecvpos;				//잔여 데이터 크기
		std::string copystr;
		memcpy(&copystr, m_pRecvpos, dwtemp);					//잔여 메모리가 없을때 패킷이 되지못한 남은 데이터를 앞으로 당기고 메모리를 초기화 시켜준다.
		ZeroMemory(&m_userbuf, PACKET_MAX_SIZE);
		memcpy(&m_userbuf, copystr.c_str(), dwtemp);
		m_iWirtenum = dwtemp;
		m_pWirtepos = &m_userbuf[m_iWirtenum];
		m_pRecvpos = m_userbuf;
	}

	if (m_pWirtepos - m_pRecvpos < PACKET_HEADER_SIZE)
	{
		return 0;
	}

	if (m_pWirtepos - m_pRecvpos >= PACKET_HEADER_SIZE)
	{
		USERPAKET* pRecvPacket;
		pRecvPacket = (USERPAKET*)m_pRecvpos;

		while (pRecvPacket->ph.len <= m_pWirtepos - m_pRecvpos)
		{
			USERPAKET packet;
			ZeroMemory(&packet, sizeof(USERPAKET));
			memcpy(&packet, pRecvPacket, pRecvPacket->ph.len);
			{
				C_Lock Lock(this);
				m_PackPool.push_back(packet);
			}
			m_pRecvpos += pRecvPacket->ph.len;
		}
	}
	return 1;
}

C_User::C_User()
{
	m_Name = "gameuser";
	m_iWirtenum=0;
	m_iReadnum=0;
	m_ID = 0;
	ZeroMemory(m_userbuf, sizeof(char)*PACKET_MAX_SIZE);
	m_pRecvpos = m_userbuf;
	m_pWirtepos = m_userbuf;
	
}


C_User::~C_User()
{
}
