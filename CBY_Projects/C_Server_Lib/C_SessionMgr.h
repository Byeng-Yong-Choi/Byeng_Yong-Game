#pragma once
#include "C_User.h"
#include "C_Queue.h"
#include "C_ObjPool.h"

#define MAX_SESSION_COUNT 1000

typedef std::map<DWORD, C_User*> CUserList;
typedef std::map<DWORD, C_User*>::iterator CUserIterator;

class C_SessionMgr:public Singleton<C_SessionMgr>
{
private:
	friend class Singleton<C_SessionMgr>;
	friend std::ostream& operator << (std::ostream& stream, C_SessionMgr& info);
	C_Queue m_QueueSession;
	C_ObjPool<COV>* pObjPool;

public:
	CUserList m_UserList;					//积己等 霸烙 蜡历 府胶飘

public:
	void Create(CRuntime* obj);
	C_User* CreateNewSession(SOCKET sock, SOCKADDR_IN addr);
	void RemoveSession(C_User* pSession);
	void Release();

public:
	bool Add(C_User* pUser);
	bool DelUser(SOCKET sock);
	bool DelUser(C_User* pUser);
	C_User* Find(SOCKET sock);

private:
	HANDLE m_hMutex;

public:
	void Lock();
	void Unlock();

private:
	C_SessionMgr();
public:
	~C_SessionMgr();
};

#define I_SessionMgr C_SessionMgr::GetInstance()