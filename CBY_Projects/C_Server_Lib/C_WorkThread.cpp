#include "C_WorkThread.h"
#include "C_IOCP.h"
#include "C_User.h"
#include "C_SessionMgr.h"

bool C_WorkThread::Run()
{
	HANDLE hIOCP = I_IOCP.m_hIOCP;
	DWORD byteTransfer;
	ULONG_PTR  keyValue;
	LPOVERLAPPED ov;

	while (m_bStarted)
	{
		BOOL ret = ::GetQueuedCompletionStatus(hIOCP, &byteTransfer, &keyValue, &ov, INFINITE);

		C_User* pUser = (C_User*)keyValue;
		COV* pOV = (COV*)ov;

		if (ret == TRUE && pUser != nullptr && pOV != nullptr)
		{
			if (pUser->Dispatch(byteTransfer, pOV) == false)
			{
				I_SessionMgr.DelUser(pUser);
			}
		}
		else
		{
			DWORD dwError = WSAGetLastError();
			// 받기 도중 전송할 때.
			// ERROR_NETNAME_DELETED 64L
			// WAIT_TIMEOUT
			if (dwError != ERROR_OPERATION_ABORTED)
			{
				if (pOV != nullptr)
				{
					delete pOV;
				}
				if (pUser != nullptr)
				{
					I_SessionMgr.DelUser(pUser);
				}
			}
		}
		Sleep(1);
	}
	m_bStarted = false;
	return true;
}

C_WorkThread::C_WorkThread()
{

}

C_WorkThread::~C_WorkThread()
{

}