#pragma once
#include "C_EffectSrc.h"
#include "QueueStd.h"

class C_EffectQueue :public C_EffectSrc
{
public:
	std::queue<C_EffectSrc*> m_List;
	void Push(C_EffectSrc* pobj);
	C_EffectSrc* Pop();

public:
	C_EffectQueue();
	virtual ~C_EffectQueue();
};


