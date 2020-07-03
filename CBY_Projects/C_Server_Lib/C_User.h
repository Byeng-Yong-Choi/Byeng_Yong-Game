#pragma once
#include "C_Character.h"
#include "C_ObjPool.h"
//#define MAX_USER_RECV_SIZE 2048

struct COV : C_ObjPool<COV>
{
	enum
	{
		CONNECT=0,
		ZERO_RECV,
		RECV,
		ZERO_SEND,
		SEND,
		CLOSE
	};
	OVERLAPPED ov;
	int Type;

	COV(int type)
	{
		ZeroMemory(&ov, sizeof(OVERLAPPED));
		Type = type;
	}

	COV()
	{
		ZeroMemory(&ov, sizeof(OVERLAPPED));
		Type = ZERO_RECV;
	}
};

class C_User:public C_Object
{
public:
	COV m_ov;

	SOCKET m_Sock;
	SOCKADDR_IN m_SockAddr;
	char m_userbuf[PACKET_MAX_SIZE];
	char m_sendbuf[PACKET_MAX_SIZE];
	char* m_pWirtepos;
	char* m_pRecvpos;
	int m_iWirtenum;
	int m_iReadnum;
	WSABUF m_wsaRecvBuf;
	WSABUF m_wsaSendBuf;

public:
	std::vector<USERPAKET> m_PackPool;

public:
	DWORD m_ID;
	std::string m_Name;
	C_Character* m_pCharacter;
	std::map<DWORD, C_Character> m_pCharList;

public:
	int AddPaket(DWORD dwSize);
	void    SetSocket(SOCKET sock, SOCKADDR_IN addr);
	void    Disconnect();
	bool	RecvProcess();
	bool	RecvProcessZERO();
	bool	Send(C_User* pUser, std::stringstream& data);
	bool    Dispatch(DWORD dwTrans, COV* ov);
	virtual bool SendProcess();
	COV* OverlappedRecv(int Type);
	COV* OverlappedSend(std::stringstream& data);
	COV* OverlappedZERO(int Type);

public:
	friend std::ostream& operator << (std::ostream& stream, C_User& userInfo);
	friend std::istream& operator >> (std::istream& stream, C_User& userInfo);

public:
	C_User();
	C_User(const C_User& user);
	~C_User();
};

