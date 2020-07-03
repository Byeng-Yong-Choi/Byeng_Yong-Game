#include "C_Lose.h"

HRESULT C_Lose::CreateVertexData()					//정점 버퍼 데이터 생성
{
	HRESULT hr = S_OK;

	m_obj.m_VertexSize = sizeof(PNCT_VERTEX);

	m_VerTex.resize(4);
	m_VerTex[0].p = D3DXVECTOR3(-(Winrt.right/2.0f), (Winrt.bottom / 2.0f), 0.5f);
	m_VerTex[1].p = D3DXVECTOR3((Winrt.right / 2.0f), (Winrt.bottom / 2.0f), 0.5f);
	m_VerTex[2].p = D3DXVECTOR3(-(Winrt.right / 2.0f), -(Winrt.bottom / 2.0f), 0.5f);
	m_VerTex[3].p = D3DXVECTOR3((Winrt.right / 2.0f), -(Winrt.bottom / 2.0f), 0.5f);				//정점 좌표 입력(직교 투영)	

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

	RECT rt = {0,0,639,1143};
	UVConvert(m_VerTex, rt, 1281, 1762);

	return hr;
}
bool C_Lose::Frame()
{
	bContinue = false;
	if (I_Input.KeyCheck(VK_UP))
	{
		bContinue = true;
	}

	return true;
}

C_Lose::C_Lose()
{
	bContinue = false;
}


C_Lose::~C_Lose()
{
}
