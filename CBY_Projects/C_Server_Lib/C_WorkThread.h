#pragma once
#include "C_Thread.h"
class C_WorkThread : public C_Thread
{
public:
	virtual bool Run() override;
public:
	C_WorkThread();
	virtual ~C_WorkThread();
};
