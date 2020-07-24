#include "C_TowerSrcMgr.h"

HRESULT C_TowerSrcMgr::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;

	m_TowerGoku = std::make_shared<C_TowerGoku>();
	hr = m_TowerGoku->Create(pd3dDevice, Context);
	if (FAILED(hr))
	{
		return hr;
	}
	m_TowerGoku->SetLV(0);

	m_TowerVegeta = std::make_shared<C_TowerVegeta>();
	hr = m_TowerVegeta->Create(pd3dDevice, Context);
	if (FAILED(hr))
	{
		return hr;
	}
	m_TowerVegeta->SetLV(0);

	m_TowerCold = std::make_shared<C_TowerCold>();
	hr = m_TowerCold->Create(pd3dDevice, Context);
	if (FAILED(hr))
	{
		return hr;
	}
	m_TowerCold->SetLV(0);

	m_TowerHidden = std::make_shared<C_HiddenTower>();
	hr = m_TowerHidden->Create(pd3dDevice, Context);
	if (FAILED(hr))
	{
		return hr;
	}
	m_TowerHidden->SetLV(0);

	m_TowerChoice.resize(TOWERCOUNT);
	for (int i = 0; i < TOWERCOUNT; i++)
	{
		m_TowerChoice[i] = std::make_shared<TowerCoice>();
		TextArray(i);
		hr = m_TowerChoice[i]->Create(pd3dDevice, Context);
		if (FAILED(hr))
		{
			return hr;
		}

	}
}

bool C_TowerSrcMgr::Release()
{
	m_TowerGoku->Release();
	m_TowerVegeta->Release();
	m_TowerCold->Release();
	m_TowerHidden->Release();

	for (int i = 0; i < m_TowerChoice.size(); i++)
	{
		m_TowerChoice[i]->Release();
	}
	return true;
}

C_TowerGoku* C_TowerSrcMgr::GetGoku()
{
	return m_TowerGoku.get();
}

C_TowerVegeta* C_TowerSrcMgr::GetVegeta()
{
	return m_TowerVegeta.get();
}

C_TowerCold* C_TowerSrcMgr::GetCold()
{
	return m_TowerCold.get();
}

C_HiddenTower* C_TowerSrcMgr::GetHidden()
{
	return m_TowerHidden.get();
}

TowerCoice* C_TowerSrcMgr::GetTowerChoice(int i)
{
	return m_TowerChoice[i].get();
}


void C_TowerSrcMgr::TextArray(int a)					//선택 초상화 rect설정
{
	std::vector<RECT> rtlist;
	switch (a)
	{
	case GOKU:
		rtlist.resize(2);
		rtlist[0] = { 48,35,40,70 };
		rtlist[1] = { 135, 31, 42, 69 };
		m_TowerChoice[GOKU]->TextArray(rtlist);
		break;

	case VEGETA:
		rtlist.resize(2);
		rtlist[0] = { 46,151,40,70 };
		rtlist[1] = { 136,148,42,69 };
		m_TowerChoice[VEGETA]->TextArray(rtlist);
		break;

	case COLD:
		rtlist.resize(2);
		rtlist[0] = { 272,39,40,70 };
		rtlist[1] = { 272,39,40,70 };
		m_TowerChoice[COLD]->TextArray(rtlist);
		break;


	}
}

C_TowerSrcMgr::C_TowerSrcMgr()
{
}


C_TowerSrcMgr::~C_TowerSrcMgr()
{
}
