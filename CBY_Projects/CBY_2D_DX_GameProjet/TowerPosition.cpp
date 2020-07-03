#include "TowerPosition.h"
#include "C_Intersection.h"
#include "CBY_Input.h"
#include "C_MoneyMgr.h"
#include "C_GameMap.h"

HRESULT TowerPosition::CreateInit(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr=S_OK;

	
	hr= Create(pd3dDevice, Context, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/TowerPosition.bmp", "VSmat", "PSBlend2");
	if (FAILED(hr))
	{
		return hr;
	}

	m_TowerGoku = std::make_shared<C_TowerGoku>();
	*m_TowerGoku = *TOWERSRC.GetGoku();

	m_TowerVegeta = std::make_shared<C_TowerVegeta>();
	*m_TowerVegeta = *TOWERSRC.GetVegeta();

	m_TowerCold = std::make_shared<C_TowerCold>();
	*m_TowerCold = *TOWERSRC.GetCold();

	m_TowerHidden = std::make_shared<C_HiddenTower>();
	*m_TowerHidden = *TOWERSRC.GetHidden();


	m_TowerChoice.resize(TOWERCOUNT);
	for (int i = 0; i < TOWERCOUNT; i++)
	{
		m_TowerChoice[i] = std::make_shared<TowerCoice>();
		*m_TowerChoice[i] = *TOWERSRC.GetTowerChoice(i);
	}

	m_Tower.resize(TOWERCOUNT + 1);
	m_Tower[GOKU] = m_TowerGoku.get();
	m_Tower[VEGETA] = m_TowerVegeta.get();
	m_Tower[COLD] = m_TowerCold.get();
	m_Tower[HIDDEN] = m_TowerHidden.get();

	return hr;
}


void TowerPosition::TextArray(int a)					//선택 초상화 rect설정
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

void TowerPosition::SetWorldandMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	if (world != nullptr)
	{
		m_matWorld = *world;
	}
	SetMatrix(&m_matWorld, view, proj);
	D3DXVECTOR3 pos = D3DXVECTOR3(m_matWorld._41, m_matWorld._42, m_matWorld._43);
	RectSet(pos);
	if (!MakeTower)
	{
		
		if (Click)
		{
			for (int i = 0; i < TOWERCOUNT; i++)
			{
				D3DXMATRIX mat = m_matWorld;
				mat._41 += m_TowerChoice[i]->m_pos.x;
				mat._42 += m_TowerChoice[i]->m_pos.y;
				mat._43 += m_TowerChoice[i]->m_pos.z;
				m_TowerChoice[i]->SetWorldandMatrix(&mat, view, proj);
			}
		}
	}
	else
	{
		switch (MakeTowerNum)
		{
		case GOKU:
			m_TowerGoku->SetWorldandMatrix(&m_matWorld, view, proj);
			break;

		case VEGETA:
			m_TowerVegeta->SetWorldandMatrix(&m_matWorld, view, proj);
			break;

		case COLD:
			m_TowerCold->SetWorldandMatrix(&m_matWorld, view, proj);
			break;

		case HIDDEN:
			m_TowerHidden->SetWorldandMatrix(&m_matWorld, view, proj);
			break;

		}
		if (Click && !MakeLvTower)
		{
			D3DXMATRIX mat;

			if (world != nullptr)
			{
				mat = *world;
			}
			else
			{
				mat = m_matWorld;
			}

			mat._41 += m_TowerChoice[MakeTowerNum]->m_pos.x;
			mat._42 += m_TowerChoice[MakeTowerNum]->m_pos.y;
			mat._43 += m_TowerChoice[MakeTowerNum]->m_pos.z;
			m_TowerChoice[MakeTowerNum]->SetWorldandMatrix(&mat, view, proj);
		}
		
	}
}

bool TowerPosition::Init()
{
	return true;
}


bool TowerPosition::Frame()
{
	ClickTime += g_SecondTime;
	
	if (m_fRect.left < (I_Input.m_Mouse.x - (Winrt.right/2) )&& (I_Input.m_Mouse.x - (Winrt.right / 2)) < m_fRect.right)					//타워 지정 위치를 클릭하는지 확인하는 조건문
	{
		if (m_fRect.top > ((Winrt.bottom / 2) - I_Input.m_Mouse.y) && ((Winrt.bottom / 2) - I_Input.m_Mouse.y) > m_fRect.bottom)
		{
			SetCursor(LoadCursor(NULL, IDC_HAND));
			if (I_Input.KeyCheck(VK_LBUTTON))
			{
					if (Click&&ClickTime >= 0.3f)
					{
						Click = false;
						ClickTime = 0.0f;
						Ren = 0.0f;
						LvRen = 0.0f;
					}
					
					if (ClickTime >= 0.3f)
					{
						Click = true;
						ClickTime = 0.0f;
					}
			}
		}
	}
	
	if (Click&&!MakeTower)																//지정 위치 클릭시 아이콘을 띄울 조건문
	{
		Ren += g_SecondTime * 80;
		if (Ren >= 50.0f)
		{
			Ren = 50.0f;
		}

		for (int i = 0; i < TOWERCOUNT; i++)
		{
			switch (i)
			{
			case GOKU:
				m_TowerChoice[GOKU]->m_pos = D3DXVECTOR3(-Ren, Ren, 0);
				if (Ren == 50.0f)
				{
					m_TowerChoice[GOKU]->Frame();
					if (m_TowerChoice[GOKU]->TowerClick && ( m_Tower[GOKU]->m_iPrice[0] <= I_MONEY.GetMoney() ))
					{
						MakeTower = true;
						MakeTowerNum = GOKU;
						m_TowerChoice[GOKU]->TowerClick = false;
						m_TowerChoice[GOKU]->m_pos = D3DXVECTOR3(0, 0, 0);
						m_TowerChoice[GOKU]->StateNUM = CHOICE_LEVELUP_STATE;
						int money = I_MONEY.GetMoney() - m_Tower[GOKU]->m_iPrice[0];
						I_MONEY.SetMoney(money);
						
					}
				}
				break;

			case VEGETA:
				m_TowerChoice[VEGETA]->m_pos = D3DXVECTOR3(Ren, Ren, 0);
				if (Ren == 50.0f)
				{
					m_TowerChoice[VEGETA]->Frame();
					if (m_TowerChoice[VEGETA]->TowerClick && (m_Tower[VEGETA]->m_iPrice[0] <= I_MONEY.GetMoney()))
					{
						MakeTower = true;
						MakeTowerNum = VEGETA;
						m_TowerChoice[VEGETA]->TowerClick = false;
						m_TowerChoice[VEGETA]->m_pos = D3DXVECTOR3(0, 0, 0);
						m_TowerChoice[VEGETA]->StateNUM = CHOICE_LEVELUP_STATE;
						int money = I_MONEY.GetMoney() - m_Tower[VEGETA]->m_iPrice[0];
						I_MONEY.SetMoney(money);
						
					}
				}
				break;

			case COLD:
				m_TowerChoice[COLD]->m_pos = D3DXVECTOR3(-Ren, -Ren, 0);
				if (Ren == 50.0f)
				{
					m_TowerChoice[COLD]->Frame();
					if (m_TowerChoice[COLD]->TowerClick && (m_Tower[COLD]->m_iPrice[0] <= I_MONEY.GetMoney()))
					{
						MakeTower = true;
						MakeTowerNum = COLD;
						m_TowerChoice[COLD]->TowerClick = false;
						m_TowerChoice[COLD]->m_pos = D3DXVECTOR3(0, 0, 0);
						m_TowerChoice[COLD]->StateNUM = CHOICE_LEVELUP_STATE;
						int money = I_MONEY.GetMoney() - m_Tower[COLD]->m_iPrice[0];
						I_MONEY.SetMoney(money);
						
					}
				}
				break;
			}
		}
	
	}

	if (Click && MakeTower && !MakeLvTower && Ren==0)																		//타워 설치후 타워 레벨업과 UI
	{
		LvRen += g_SecondTime * 80;
		if (LvRen >= 50.0f)
		{
			LvRen = 50.0f;
		}
		m_TowerChoice[MakeTowerNum]->m_pos.x = LvRen;
		if (LvRen == 50.0f)
		{
			m_TowerChoice[MakeTowerNum]->Frame();
			if (m_TowerChoice[MakeTowerNum]->TowerClick && (m_Tower[MakeTowerNum]->m_iPrice[1] <= I_MONEY.GetMoney()))
			{
				MakeLvTower = true;
				m_TowerChoice[MakeTowerNum]->TowerClick = false;
				m_TowerChoice[MakeTowerNum]->m_pos = D3DXVECTOR3(0, 0, 0);
				m_Tower[MakeTowerNum]->SetState(TOWER_LEVELUP_STATE);
				m_Tower[MakeTowerNum]->SetLV(1);
				int money = I_MONEY.GetMoney() - m_Tower[MakeTowerNum]->m_iPrice[1];
				I_MONEY.SetMoney(money);
			}
		}
	}

	if (m_fRect.left > (I_Input.m_Mouse.x - (Winrt.right / 2)) ||
		(I_Input.m_Mouse.x - (Winrt.right / 2)) > m_fRect.right ||
		m_fRect.top < ((Winrt.bottom / 2) - I_Input.m_Mouse.y) ||
		((Winrt.bottom / 2) - I_Input.m_Mouse.y) < m_fRect.bottom)	//지정 위치가 아닌 다른곳을 클릭하면 아이콘 출력을 지운다
	{
		
			if (I_Input.KeyCheck(VK_LBUTTON))
			{
				Click = false;
				ClickTime = 0.5f;
				Ren = 0.0f;
				LvRen = 0.0f;
			}
		
	}

	/*if (Click)
	{
		I_MAP.SetCbData(m_Center, m_fScope);
	}*/
	if (MakeTower)
	{
		m_fChangTime += g_SecondTime;
		m_Tower[MakeTowerNum]->Update(m_fChangTime);
	}
	return true;
}

bool TowerPosition::Render()
{
	if (!MakeTower)
	{
		CShapeObject::Render();

		if (Click)
		{
			for (int i = 0; i < TOWERCOUNT; i++)
			{
				m_TowerChoice[i]->Render();
			}
		}
	}
	else
	{
		switch (MakeTowerNum)
		{
		case GOKU:
			m_TowerGoku->Render();
			break;

		case VEGETA:
			m_TowerVegeta->Render();
			break;

		case COLD:
			m_TowerCold->Render();
			break;

		case HIDDEN:
			m_TowerHidden->Render();
			return true;
			break;

		}
		if (Click&&!MakeLvTower)
		{
			m_TowerChoice[MakeTowerNum]->Render();
		}
	}
	
	
	return true;
}

bool TowerPosition::Release()
{
	m_TowerGoku->Release();
	m_TowerVegeta->Release();
	m_TowerCold->Release();
	m_TowerHidden->Release();

	for (int i = 0; i < TOWERCOUNT; i++)
	{
		m_TowerChoice[i]->Release();
	}


	return true;
}

HRESULT TowerPosition::CreateVertexData()
{
	HRESULT hr = S_OK;

	
	m_obj.m_VertexSize = sizeof(PNCT_VERTEX);

	m_VerTex.resize(4);

	m_VerTex[0].p = D3DXVECTOR3(-29.0f, 29.0f, 0.5f);
	m_VerTex[1].p = D3DXVECTOR3(29.0f, 29.0f, 0.5f);
	m_VerTex[2].p = D3DXVECTOR3(-29.0f, -29.0f, 0.5f);
	m_VerTex[3].p = D3DXVECTOR3(29.0f, -29.0f, 0.5f);				//정점 좌표 입력(직교 투영)	
	
	m_VerTex[0].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VerTex[1].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VerTex[2].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VerTex[3].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	
	m_VerTex[0].c = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);
	m_VerTex[1].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VerTex[2].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VerTex[3].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);			//정점 컬러값 입력
	
	RECT rect = { 145,147,58,58 };
	UVConvert(m_VerTex, rect, 1280, 1024);

	//RECT rect = { 0,0,144,144 };
	//UVConvert(m_VerTex, rect, 144, 144);

	m_Center.x = 0;
	m_Center.y = 0;
	m_Center.z = 0;

	m_Size.x = abs((m_VerTex[0].p.x - m_VerTex[1].p.x) / 2);
	m_Size.y = abs((m_VerTex[2].p.y - m_VerTex[0].p.y) / 2);
	m_Size.z = abs((m_VerTex[0].p.z - m_VerTex[1].p.z) / 2);

	m_fRect.left = m_Center.x - m_Size.x;
	m_fRect.top = m_Center.y + m_Size.y;
	m_fRect.right = m_Center.x + m_Size.x;
	m_fRect.bottom = m_Center.y - m_Size.y;

	return hr;
}
void TowerPosition::RectSet(D3DXVECTOR3 pos)
{
	m_Center.x = 0 + pos.x;
	m_Center.y = 0 + pos.y;
	m_Center.z = 0 + pos.z;			//센터값에서 이동값 더함

	switch (MakeTowerNum)
	{
	case GOKU:
		m_fScope = GOKUSCOPE;
		break;

	case VEGETA:
		m_fScope = VEGETASCOPE;
		break;

	case COLD:
		m_fScope = COLDSCOPE;
		break;

	case HIDDEN:
		m_fScope = HIDDENSCOPE;
		break;
	}

	m_fRect.left = m_Center.x - m_Size.x;
	m_fRect.top = m_Center.y + m_Size.y;
	m_fRect.right = m_Center.x + m_Size.x;
	m_fRect.bottom = m_Center.y - m_Size.y;

	m_frTowerScope.left = m_Center.x  - m_fScope;
	m_frTowerScope.top = m_Center.y + m_fScope;
	m_frTowerScope.right = m_Center.x + m_fScope;
	m_frTowerScope.bottom = m_Center.y - m_fScope;
}

int TowerPosition::MakeTowerNumber()
{
	return MakeTowerNum;
}

float TowerPosition::GetColdSlowValue()
{
	return m_TowerCold->fSpeedSlow[m_Tower[MakeTowerNum]->GetLV()];
}

void TowerPosition::GameSet()
{
	for (int i = 0; i < m_TowerChoice.size(); i++)
	{
		m_TowerChoice[i]->StateNUM = CHOICE_STANDBY;
	}

	for (int i = 0; i < m_Tower.size(); i++)
	{
		m_Tower[i]->StateNUM = TOWER_STANDBY;
		m_Tower[i]->SetLV(0);
	}

	Ren = 0.0f;
	LvRen = 0.0f;
	MakeTowerNum = -1;
	MakeTower = false;
	Click = false;
	ClickTime = 0.3f;
	MakeLvTower = false;
	bHiddenb = false;
	m_fScope = 0;
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_Rot);
}

void TowerPosition::HiddenTowerSet()
{
	GameSet();
	MakeTower = true;
	MakeLvTower = true;
	bHiddenb = true;
	MakeTowerNum = HIDDEN;
}

bool TowerPosition::bHidden()
{
	return bHiddenb;
}

TowerPosition::TowerPosition()
{
	Ren = 0.0f;
	LvRen = 0.0f;
	MakeTowerNum = -1;
	MakeTower = false;
	Click = false;
	ClickTime = 0.3f;
	MakeLvTower = false;
	bHiddenb = false;
	m_fScope = 0;
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_Rot);
	m_fChangTime = 0.0f;
}


TowerPosition::~TowerPosition()
{
}

