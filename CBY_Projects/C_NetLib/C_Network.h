#pragma once
#include "C_User.h"
#include "C_Thread.h"

typedef void(*Fuction)(USERPAKET& pk);

class C_Network:public C_Thread
{
public:
	C_NetBase m_Lock;
	C_User m_Packet;
	SOCKET m_Socket;
	static std::stringstream SStr;
	static std::list<USERPAKET> m_RecvPool;
	static std::list<USERPAKET> m_SendPool;
	static std::list<std::string> m_ssSendPool;
	static std::map<int, Fuction> m_fPacketFun;
	typedef std::map<int, Fuction>::iterator f_iter;

public:
	bool Init();
	bool Release();
	bool CreateSock(int iSockType, unsigned long lMode);
	virtual bool Run();
	void PacketProcess();
	virtual bool RunThread() override;

public:
	static int SendMSG(SOCKET sock, const char* buf, int iLength);
	static int SendMSG(SOCKET sock, USERPAKET& data);
	static int SendMSG(SOCKET sock, std::string& data);
	static int PushSendPool(const char* buf, int iLength);
	static bool Error();

public:
	C_Network();
	~C_Network();
};

