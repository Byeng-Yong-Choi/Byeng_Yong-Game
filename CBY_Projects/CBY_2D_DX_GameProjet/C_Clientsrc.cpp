#include "C_Clientsrc.h"
#include "Cstd.h"

bool C_Clientsrc::Login = false;
bool C_Clientsrc::CreateID = false;

bool C_Clientsrc::Init()
{
	if (!Connect(10005, "192.168.0.49"))	//������
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
		int i = MessageBox(g_hwnd, L"���� �����մϴ�.", L"�α��� ����", MB_OK);
		if (i == IDOK)
		{
		
			Login = true;
		}
	}
	else if (pk.ph.type == LOGIN_IDFAIL)
	{
		MessageBox(g_hwnd, L"���̵� �����ϴ�.", L"�α��� ����", MB_OK);
		Login = false;
	}
	else if (pk.ph.type == LOGIN_PSFAIL)
	{
		MessageBox(g_hwnd, L"��� ��ȣ�� Ʋ�Ƚ��ϴ�.", L"�α��� ����", MB_OK);
		Login = false;
	}
}

bool C_Clientsrc::SelectRun()
{
	FD_ZERO(&rSet);
	FD_ZERO(&wSet);

	FD_SET(m_Socket, &rSet);
	if (C_Network::m_SendPool.size() != 0)		//��Ʈ�� �̿�
	{
		FD_SET(m_Socket, &wSet);
	}

	timeval time;
	time.tv_sec = 0;
	time.tv_usec = 1;
  	int iRet = select(0, &rSet, &wSet, NULL, &time);
	if (iRet == 0)		//Ÿ�Ӿƿ��� ���
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
			for (auto& packet : C_Network::m_SendPool)	//��Ʈ���� �̿��� ��Ŷ �ְ�ޱ� ����
			{
				C_Network::SendMSG(m_Socket, packet);
			}
			C_Network::m_SendPool.clear();			//��Ʈ�� �̿�
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
	MessageBox(g_hwnd, L"���ϵ帳�ϴ�. ȸ�� ������ �Ǿ����ϴ�.", L"ȸ�� ���� ����!!", MB_OK);
	CreateID = true;
}

void C_Clientsrc::CreateIDFail(USERPAKET& pk)
{
	MessageBox(g_hwnd, L"�̹� �ִ� ���̵��Դϴ�.", L"ȸ�� ���� ����", MB_OK);
}