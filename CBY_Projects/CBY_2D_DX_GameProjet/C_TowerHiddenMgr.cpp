#include "C_TowerHiddenMgr.h"
#include "C_MoneyMgr.h"

void C_TowerHiddenMgr::HiddenMgrFrame(std::vector<std::shared_ptr<C_Tower>>& Tower)
{
	CheckHidden(Tower);
	Hidden();
	if (bHidden)
	{
		MakeHiddenTower(Tower);
	}
}

void C_TowerHiddenMgr::CheckHidden(std::vector<std::shared_ptr<C_Tower>>& Tower)
{
	bGoku = false;
	bVegeta = false;
	for (int i = 0; i < Tower.size(); i++)
	{
		if (Tower[i]->MakeTowerNumber() == GOKU)
		{
			if (Tower[i]->m_Tower[GOKU]->GetLV() == 1)
			{
				bGoku = true;
				bool bDouble=false;
				for (int j = 0; j < m_goku.size(); j++)
				{
					if (m_goku[j] == i)
					{
						bDouble = false;
						break;
					}
					bDouble = true;
				}
				if (bDouble || m_goku.size()==0)
				{
					m_goku.push_back(i);
					continue;
				}
			}
		}

		if (Tower[i]->MakeTowerNumber() == VEGETA)
		{
			if (Tower[i]->m_Tower[VEGETA]->GetLV() == 1)
			{
				bVegeta = true;
				bool bDouble = false;
				for (int j = 0; j < m_vegeta.size(); j++)
				{
					if (m_vegeta[j] == i)
					{
						bDouble = false;
						break;
					}
					bDouble = true;
				}
				if (bDouble || m_vegeta.size() == 0)
				{
					m_vegeta.push_back(i);
					break;
				}
				
			}
		}
	}
}

bool C_TowerHiddenMgr::Hidden()
{
	if (bGoku && bVegeta && (I_MONEY.GetMoney()> m_HiddenPrice))
	{
		bHidden = true;
	}
	else
	{
		bHidden = false;
	}


	return bHidden;
}

void C_TowerHiddenMgr::MakeHiddenTower(std::vector<std::shared_ptr<C_Tower>>& Tower)
{
	for (int i = 0; i < m_goku.size(); i++)
	{
		if (Tower[m_goku[i]]->Click)
		{
			Tower[m_goku[i]]->HiddenTowerSet();
			Tower[m_vegeta[0]]->GameSet();
			int money = I_MONEY.GetMoney()- m_HiddenPrice;
			I_MONEY.SetMoney(money);

			std::vector<int>::iterator iter;
			iter = m_goku.begin() + i;
			m_goku.erase(iter);
			iter = m_vegeta.begin();
			m_vegeta.erase(iter);
			i--;
		}
	}

	for (int i = 0; i < m_vegeta.size(); i++)
	{
		if (Tower[m_vegeta[i]]->Click)
		{
			Tower[m_vegeta[i]]->HiddenTowerSet();
			Tower[m_goku[0]]->GameSet();
			int money = I_MONEY.GetMoney() - m_HiddenPrice;
			I_MONEY.SetMoney(money);
			std::vector<int>::iterator iter;
			iter = m_goku.begin();
			m_goku.erase(iter);
			iter = m_vegeta.begin() + i;
			m_vegeta.erase(iter);
			i--;
		}
	}
}

bool C_TowerHiddenMgr::Release()
{
	m_goku.clear();
	m_vegeta.clear();
	return true;
}

C_TowerHiddenMgr::C_TowerHiddenMgr()
{
	bGoku = false;
	bVegeta = false;
	bHidden = false;
	m_HiddenPrice = 300;
}


C_TowerHiddenMgr::~C_TowerHiddenMgr()
{
}
