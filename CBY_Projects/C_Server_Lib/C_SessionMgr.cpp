#include "C_SessionMgr.h"
#include "C_IOCP.h"

//���� ��ó�� �ѻ����� �ѻ���, ���� ���Խ� �޸� �ο��� ȸ��
void C_SessionMgr::Create(CRuntime* obj)			//���̺귯���� �̿��ϱ⿡ ���� ���������� �ٲ���ϸ� ���̺귯���� ��� ���� �Ҵ� �Ҽ��� ��� �Լ������ͷ� ������ ���� �޾� ����.
{
	for (int i = 0; i < MAX_SESSION_COUNT; i++)
	{
		C_Object* pobj=obj->CreateObject();
		m_QueueSession.Push(pobj);
	}

//	pObjPool->Init();
}

C_User* C_SessionMgr::CreateNewSession(SOCKET sock, SOCKADDR_IN addr)
{
	C_User* user = (C_User*)m_QueueSession.Pop();
	assert(user);
	user->SetSocket(sock, addr);
	Add(user);
	std::cout << "���� IP = " << inet_ntoa(addr.sin_addr) << ", " << "Port = " << ntohs(addr.sin_port) << std::endl;
	return user;
}

void C_SessionMgr::RemoveSession(C_User* pSession)
{
	m_QueueSession.Push(pSession);
}

void C_SessionMgr::Release()
{
	for (CUserIterator iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
	{
		C_User* delUser = iter->second;
		if (delUser != nullptr)
		{
			delUser->Disconnect();
			RemoveSession(delUser);
		}
	}
	m_UserList.clear();
	//pObjPool->Release();
}

//���� ����, ���� ����
bool C_SessionMgr::Add(C_User* pUser)		//������ ���Խ� �޸� �ο�, IOCP���� �۾�
{
	Lock();
		m_UserList.insert(std::make_pair(pUser->m_Sock, pUser));
		C_Character Char;
		pUser->m_pCharList[8] = Char;
		I_IOCP.AddToIocp((HANDLE)pUser->m_Sock, (ULONG_PTR)pUser);
		pUser->RecvProcessZERO();
	Unlock();
	return true;
}

bool C_SessionMgr::DelUser(SOCKET sock)
{
	CUserIterator DeleteUser = m_UserList.find(sock);
	if (DeleteUser != m_UserList.end())
	{
		Lock();
			C_User* pUser = (C_User*)DeleteUser->second;
			pUser->Disconnect();
			RemoveSession(pUser);
			m_UserList.erase(DeleteUser++);
		Unlock();
	}
	else
	{
		return false;
	}

	return true;
}

bool C_SessionMgr::DelUser(C_User* pUser)
{
	return DelUser(pUser->m_Sock);
}

C_User* C_SessionMgr::Find(SOCKET sock)
{
	CUserIterator FindUser = m_UserList.find(sock);
	if (FindUser != m_UserList.end())
	{
		return FindUser->second;		//ã���� ��� �� ������ �ѱ��.
	}
	return nullptr;						//ã�� ���Ұ�� nullptr�� �ѱ��.
}

//Mutex ����
void C_SessionMgr::Lock()
{
	WaitForSingleObject(m_hMutex, INFINITE);
}
void C_SessionMgr::Unlock()
{
	ReleaseMutex(m_hMutex);
}

std::ostream& operator << (std::ostream& stream, C_SessionMgr& info)
{
	DWORD dCount = info.m_UserList.size();
	tBlockWrite<DWORD>(stream, dCount);
	
	for (auto& user : info.m_UserList)
	{
		DWORD dwFirst = user.first;
		tBlockWrite<DWORD>(stream, dwFirst);
		stream << *user.second;
	}
	return stream;
}

C_SessionMgr::C_SessionMgr()
{
	m_hMutex = CreateMutex(NULL, FALSE, _T("C_SessionMgr"));
}


C_SessionMgr::~C_SessionMgr()
{
}
