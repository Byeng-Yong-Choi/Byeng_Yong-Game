#include "C_Clientsrc.h"
#include "Cstd.h"

bool C_Clientsrc::Login = false;
bool C_Clientsrc::CreateID = false;

bool C_Clientsrc::Init()
{
	if (!Connect(10005, "192.168.0.49"))	//내서버
	{
		return false;
	}
	SetFunction();
	return true;
}

void C_Clientsrc::RecvMsg(USERPAKET& pk)
{
	if (pk.ph.type == LOGIN_SUCCESS)
	{
		Login = false;
		int i = MessageBox(g_hwnd, L"게임 입장합니다.", L"로그인 성공", MB_OK);
		if (i == IDOK)
		{
		
			Login = true;
		}
	}
	else if (pk.ph.type == LOGIN_IDFAIL)
	{
		MessageBox(g_hwnd, L"아이디가 없습니다.", L"로그인 실패", MB_OK);
		Login = false;
	}
	else if (pk.ph.type == LOGIN_PSFAIL)
	{
		MessageBox(g_hwnd, L"비밀 번호가 틀렸습니다.", L"로그인 실패", MB_OK);
		Login = false;
	}
}

bool C_Clientsrc::SelectRun()
{
	FD_ZERO(&rSet);
	FD_ZERO(&wSet);

	FD_SET(m_Socket, &rSet);
	if (C_Network::m_SendPool.size() != 0)		//스트링 이용
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
			for (auto& packet : C_Network::m_SendPool)	//스트링을 이용한 패킷 주고받기 사용시
			{
				C_Network::SendMSG(m_Socket, packet);
			}
			C_Network::m_SendPool.clear();			//스트링 이용
		}
	}

	return true;
}

bool C_Clientsrc::Run()
{
	SelectRun();
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
	m_fPacketFun[LOGIN_SUCCESS] = RecvMsg;
	m_fPacketFun[LOGIN_IDFAIL] = RecvMsg;
	m_fPacketFun[LOGIN_PSFAIL] = RecvMsg;
	m_fPacketFun[CREATID_SUCCESS] = CreateIDSuccess;
	m_fPacketFun[CREATID_FAIL] = CreateIDFail;
}

C_Clientsrc::C_Clientsrc()
{
	ZeroMemory(m_szID, sizeof(m_szID));
	ZeroMemory(m_szPS, sizeof(m_szPS));
}


C_Clientsrc::~C_Clientsrc()
{
}

void C_Clientsrc::CreateIDSuccess(USERPAKET& pk)
{
	MessageBox(g_hwnd, L"축하드립니다. 회원 가입이 되었습니다.", L"회원 가입 성공!!", MB_OK);
	CreateID = true;
}

void C_Clientsrc::CreateIDFail(USERPAKET& pk)
{
	MessageBox(g_hwnd, L"이미 있는 아이디입니다.", L"회원 가입 실패", MB_OK);
}