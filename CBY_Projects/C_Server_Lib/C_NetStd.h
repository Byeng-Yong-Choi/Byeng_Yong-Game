#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "C_Protocol.h"
#include <tchar.h>
#include <assert.h>
#include <process.h>


#pragma comment(lib, "ws2_32.lib")

template <class T> class Singleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
};

