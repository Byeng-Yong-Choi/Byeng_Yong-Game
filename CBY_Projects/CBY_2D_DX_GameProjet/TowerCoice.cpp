#include "TowerCoice.h"
#include "CBY_Input.h"

HRESULT TowerCoice::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;
	m_pd3dDevice = pd3dDevice;
	m_pContext = Context;
	m_SrcList.resize(CHOICE_COUNT);
	VertexData();
	TextSrc();

	StateNUM = CHOICE_STANDBY;

	return hr;
}

void TowerCoice::TextArray(std::vector<RECT>& rt)
{
	m_RtList.resize(CHOICE_COUNT);
	for (int i = 0; i < m_RtList.size(); i++)
	{
		m_RtList[i] = rt[i];
	}
	
}

bool TowerCoice::Frame()
{
	TowerClick = false;
	if (m_fRect.left < (I_Input.m_Mouse.x - (Winrt.right / 2)) && (I_Input.m_Mouse.x - (Winrt.right / 2)) < m_fRect.right)					//타워 지정 위치를 클릭하는지 확인하는 조건문
	{
		if (m_fRect.top > ((Winrt.bottom / 2) - I_Input.m_Mouse.y) && ((Winrt.bottom / 2) - I_Input.m_Mouse.y) > m_fRect.bottom)
		{
			SetCursor(LoadCursor(NULL, IDC_HAND));
			if (I_Input.KeyCheck(VK_LBUTTON))
			{
				TowerClick = true;
			}
		}
	}
	return true;
}

void TowerCoice::TextSrc()
{
	//STATE_START:
	m_SrcList[CHOICE_STANDBY] = std::make_shared<CShapeObject>();
	m_SrcList[CHOICE_STANDBY]->Create(m_pd3dDevice, m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/Tower.bmp", "VSmat", "PSBlend2");
	m_SrcList[CHOICE_STANDBY]->UVConvert(m_vertexdata,m_RtList[CHOICE_STANDBY], 3840, 1080);
	m_SrcList[CHOICE_STANDBY]->UpdateVertexData(m_vertexdata);


	//TOWER_LEVELUP_STATE:
	m_SrcList[CHOICE_LEVELUP_STATE] = std::make_shared<CShapeObject>();
	m_SrcList[CHOICE_LEVELUP_STATE]->Create(m_pd3dDevice, m_pContext, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/TowerSrc/Tower.bmp", "VSmat", "PSBlend2");
	m_SrcList[CHOICE_LEVELUP_STATE]->UVConvert(m_vertexdata,m_RtList[CHOICE_LEVELUP_STATE],3840, 1080);
	m_SrcList[CHOICE_LEVELUP_STATE]->UpdateVertexData(m_vertexdata);
}

TowerCoice::TowerCoice()
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	TowerClick = false;
}


TowerCoice::~TowerCoice()
{
}
