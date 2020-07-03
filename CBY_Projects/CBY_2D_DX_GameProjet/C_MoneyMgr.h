#pragma once
#include "Cstd.h"

class C_MoneyMgr : public Singleton<C_MoneyMgr>
{
private:
	int m_iMoney;

public:
	void SetMoney(int i);
	int GetMoney();

public:
	C_MoneyMgr();
	~C_MoneyMgr();
};

#define I_MONEY C_MoneyMgr::GetInstance()



