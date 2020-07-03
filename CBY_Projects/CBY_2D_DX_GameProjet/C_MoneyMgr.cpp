#include "C_MoneyMgr.h"

void C_MoneyMgr::SetMoney(int i)
{
	m_iMoney = i;
	if (m_iMoney >= 9999)
	{
		m_iMoney = 9999;
	}
}

int C_MoneyMgr::GetMoney()
{
	return m_iMoney;
}

C_MoneyMgr::C_MoneyMgr()
{
}


C_MoneyMgr::~C_MoneyMgr()
{
}
