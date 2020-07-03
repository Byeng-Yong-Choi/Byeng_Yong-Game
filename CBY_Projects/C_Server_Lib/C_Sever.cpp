#include "C_Sever.h"
#include "C_IOCP.h"
#include "C_SessionMgr.h"

bool C_Sever::Init()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	I_IOCP.Init();
	m_Acceptor.Set(10005);

	m_ODBC.Init();
	m_ODBC.ConnectSQL(L"GAMEUSER", L"sa", L"kgca1234");
	m_ODBC.ProcedurePrepare();

	CreateThread();
	return true;
}

bool C_Sever::Run()
{
	while (1)
	{
		Sleep(1);
	}
	return true;
}


bool C_Sever::Release()
{
	m_ODBC.Release();
	WSACleanup();
	return true;
}

void C_Sever::SendUserList(C_User* pUser)
{
	std::stringstream userdata;
	userdata << I_SessionMgr;
	std::stringstream Data = MakePacket(0, &userdata);
	SendPacket(pUser, Data);
}

void C_Sever::SendPacket(C_User* pUser, std::stringstream& sendStream)
{
	C_PACKET cPacket;				//패킷 생성
	cPacket.pUser=pUser;			//패킷에 유저정보 입력
	CopyMemory(&cPacket.packet, sendStream.str().c_str(), sendStream.str().length());		//패킷에 패킷 내용 복사
	m_SendPackPool.push_back(cPacket);		//패킷풀에 입력된 패킷정보 삽입

}

C_Sever::C_Sever()
{
}


C_Sever::~C_Sever()
{
}
