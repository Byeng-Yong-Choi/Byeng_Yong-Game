#include "C_Score.h"
HRESULT C_Score::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context)
{
	HRESULT hr = S_OK;

	m_Score.resize(5);
	Textarray();
	for (int i = 0; i < m_Score.size(); i++)
	{
		m_Score[i] = std::make_shared<C_Score>();
		m_Score[i]->CSprite::Create(pd3dDevice, Context, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/UI/score.png", "VSmat", "PS");
		m_Score[i]->SetRectArray(m_RtList, 500, 384);
	}
	m_Scorefont = std::make_shared<CShapeObject>();
	m_Scorefont->Create(pd3dDevice, Context, L"../../data/shader/ColorBlend.txt", L"../../data/2DGame/UI/score_font.png", "VSmat", "PS");
	FontVertex();
	RECT rt = { 0,0,650,488 };
	m_Scorefont->UVConvert(m_vertexdata, rt, 650, 488);
	m_Scorefont->UpdateVertexData(m_vertexdata);

	m_PosList.resize(m_Score.size()+1);

	return hr;
}

void C_Score::FontVertex()
{
	m_vertexdata.resize(4);

	m_vertexdata[0].p = D3DXVECTOR3(-50.0f, 50.0f, 0.5f);
	m_vertexdata[1].p = D3DXVECTOR3(50.0f, 50.0f, 0.5f);
	m_vertexdata[2].p = D3DXVECTOR3(-50.0f, -50.0f, 0.5f);
	m_vertexdata[3].p = D3DXVECTOR3(50.0f, -50.0f, 0.5f);				//정점 좌표 입력(직교 투영)	

	m_vertexdata[0].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vertexdata[1].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vertexdata[2].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vertexdata[3].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_vertexdata[0].c = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);
	m_vertexdata[1].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_vertexdata[2].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_vertexdata[3].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);			//정점 컬러값 입력

	m_vertexdata[0].t = D3DXVECTOR2(0.0f, 0.0f);
	m_vertexdata[1].t = D3DXVECTOR2(1.0f, 0.0f);
	m_vertexdata[2].t = D3DXVECTOR2(0.0f, 1.0f);
	m_vertexdata[3].t = D3DXVECTOR2(1.0f, 1.0f);			//정점 텍스쳐 좌표
}

void C_Score::Textarray()
{
	m_RtList.resize(10);

	m_RtList[0] = {90,26,25,29};
	m_RtList[1] = { 128,26,25,29 };
	m_RtList[2] = { 155,26,25,29 };
	m_RtList[3] = { 185,26,25,29 };
	m_RtList[4] = { 217,26,25,29 };
	m_RtList[5] = { 245,26,25,29 };
	m_RtList[6] = { 275,26,25,29 };
	m_RtList[7] = { 304,26,25,29 };
	m_RtList[8] = { 334,26,25,29 };
	m_RtList[9] = { 1,57,25,29 };

}

void C_Score::SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	int ipos = 0;
	for (int i = m_Score.size()-1; i >= 0; i--)
	{
		m_matWorld._41 = m_PosList[ipos].x;
		m_matWorld._42 = m_PosList[ipos].y;
		m_matWorld._43 = m_PosList[ipos].z;
		m_Score[i]->CSprite::SetMatrix(&m_matWorld, view, proj);
		ipos++;
	}
	m_matWorld._41 = m_PosList[ipos].x;
	m_matWorld._42 = m_PosList[ipos].y;
	m_matWorld._43 = m_PosList[ipos].z;
	m_Scorefont->SetMatrix(&m_matWorld, view, proj);
}



HRESULT C_Score::CreateVertexData()					//정점 버퍼 데이터 생성
{
	HRESULT hr = S_OK;

	m_obj.m_VertexSize = sizeof(PNCT_VERTEX);

	m_VerTex.resize(4);

	m_VerTex[0].p = D3DXVECTOR3(-15.0f, 15.0f, 0.5f);
	m_VerTex[1].p = D3DXVECTOR3(15.0f, 15.0f, 0.5f);
	m_VerTex[2].p = D3DXVECTOR3(-15.0f, -15.0f, 0.5f);
	m_VerTex[3].p = D3DXVECTOR3(15.0f, -15.0f, 0.5f);				//정점 좌표 입력(직교 투영)	

	m_VerTex[0].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VerTex[1].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VerTex[2].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VerTex[3].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_VerTex[0].c = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);
	m_VerTex[1].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VerTex[2].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VerTex[3].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);			//정점 컬러값 입력

	m_VerTex[0].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VerTex[1].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VerTex[2].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VerTex[3].t = D3DXVECTOR2(1.0f, 1.0f);			//정점 텍스쳐 좌표

	return hr;

}

bool C_Score::Frame()
{
	m_fScore += g_SecondTime;
	
	int index, a;

	index = (int) m_fScore / 10000;
	a = (int)m_fScore % 10000;
	m_Score[0]->m_Index = index;

	index = a / 1000;
	a %= 1000;
	m_Score[1]->m_Index = index;

	index = a / 100;
	a %= 100;
	m_Score[2]->m_Index = index;

	index = a / 10;
	a %= 10;
	m_Score[3]->m_Index = index;

	m_Score[4]->m_Index = a;

	for (int i = 0; i < m_PosList.size(); i++)
	{
		m_PosList[i].x = (Winrt.right / 2.2f) - ((Winrt.right / 38.6f)*i);
		m_PosList[i].y = (-Winrt.bottom / 2.0f) + (Winrt.bottom / 18.0f);
		if (i == m_PosList.size() - 1)
		{
			m_PosList[i].x = (Winrt.right / 2.2f) - ((Winrt.right / 28.6f)*i);
			m_PosList[i].y = (-Winrt.bottom / 2.0f) + (Winrt.bottom / 21.0f);
		}
		m_PosList[i].z = 1.0f;
	}
	return true;
}

bool C_Score::Render()
{
	for (int i = 0; i < m_Score.size(); i++)
	{
		m_Score[i]->CSprite::Render();
	}
	m_Scorefont->Render();
	return true;
}

bool C_Score::Release()
{
	for (int i = 0; i < m_Score.size(); i++)
	{
		m_Score[i]->CSprite::Release();
	}
	m_Scorefont->Release();
	return true;
}

void C_Score::GameSet()
{
	for (int i = 0; i < m_Score.size(); i++)
	{
		m_Score[i]->m_Index = 0;
	}
	m_fScore = 0.0f;

}

float C_Score::GetScore()
{
	return m_fScore;
}


C_Score::C_Score()
{
	D3DXMatrixIdentity(&m_matWorld);
	m_fScore = 0;
}


C_Score::~C_Score()
{
}
