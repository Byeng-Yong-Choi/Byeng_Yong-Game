#include "C_SessionMgr.h"
#include "C_IOCP.h"

//세션 맨처음 총생성과 총삭제, 유저 진입시 메모리 부여와 회수
void C_SessionMgr::Create(CRuntime* obj)			//라이브러리를 이용하기에 만약 유저정보를 바꿔야하면 라이브러리를 뜯어 고쳐 할당 할수는 없어서 함수포인터로 생성할 것을 받아 쓴다.
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
	std::cout << "접속 IP = " << inet_ntoa(addr.sin_addr) << ", " << "Port = " << ntohs(addr.sin_port) << std::endl;
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

//유저 생성, 삭제 관리
bool C_SessionMgr::Add(C_User* pUser)		//유저가 진입시 메모리 부여, IOCP연동 작업
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
		return FindUser->second;		//찾았을 경우 그 유저를 넘긴다.
	}
	return nullptr;						//찾지 못할경우 nullptr을 넘긴다.
}

//Mutex 관리
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
