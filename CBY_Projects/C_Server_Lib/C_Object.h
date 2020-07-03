#pragma once
#include "C_NetStd.h"
#define RUNTIME_CLASS(s) &s::class##s;
#define DECLARE_DYNCREATE static C_Object* CreateObject();
#define IMPLEMENT_DYNCREATE(s) C_Object* s::CreateObject(){return new s;};

#define DECLARE_DYNAMIC(s) static CRuntime class##s;
#define IMPLEMENT_DYNAMIC(s) CRuntime s::classCGameUser = { #s, sizeof(s), s::CreateObject };

class C_Object;

struct CRuntime
{
	char m_Classname[25];
	int m_iObjectSize;
	C_Object* (*pCreateObject)();		//함수포인터 이용
	C_Object* CreateObject()
	{
		return(*pCreateObject)();
	}
};

class C_Object
{
public:
	CRITICAL_SECTION m_CS;

public:
	C_Object();
	virtual ~C_Object();
};

