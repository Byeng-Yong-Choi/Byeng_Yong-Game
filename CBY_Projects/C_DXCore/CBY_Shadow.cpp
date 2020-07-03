#include "CBY_Shadow.h"



HRESULT CBY_Shadow::Create(ID3D11Device* pd3dDevice, float fWidth, float fHeight, const TCHAR* ShaderFileName, const CHAR* VSName, const CHAR* PSName)
{
	HRESULT hr = S_OK;

	if (m_Rt)
	{
		m_Rt->Release();
		m_Rt = NULL;
	}
	m_Rt = std::make_shared<CBY_DxRT>();
	m_Rt->m_DSFormat = m_DSFormat;
	hr = m_Rt->Create(pd3dDevice, fWidth, fHeight);

	//hr = LoadShader(pd3dDevice, ShaderFileName, VSName, PSName);

	return hr;
}
bool CBY_Shadow::Begin(ID3D11DeviceContext* pContext, D3DXVECTOR4 vColor)
{
	CDX::ApplyDSS(pContext, CDX::CDxState::g_pDSS);
	CDX::ApplyBS(pContext, CDX::CDxState::g_pAlpahBlend);
	CDX::ApplyRS(pContext, CDX::CDxState::g_pRSDepth);

	m_Rt->Begin(pContext, vColor);
	return true;
}

void CBY_Shadow::ShaderSet(ID3D11DeviceContext* pContext)
{
	pContext->VSSetShader(m_pVS, NULL, 0);
	pContext->PSSetShader(m_pPS, NULL, 0);


}

bool CBY_Shadow::End(ID3D11DeviceContext* pContext)
{
	m_Rt->End(pContext);
	
	CDX::ApplyBS(pContext, CDX::CDxState::g_pAlpahBlend);
	CDX::ApplySS(pContext, CDX::CDxState::g_pSampler);
	CDX::ApplyRS(pContext, CDX::CDxState::g_pRSSold);
	return true;
}

bool CBY_Shadow::Release()
{
	m_Rt->Release();
	return true;
}

HRESULT CBY_Shadow::LoadShader(ID3D11Device* pd3dDevice, const TCHAR* ShaderFileName, const CHAR* VSName, const CHAR* PSName)			//���̴� �ε�
{
	HRESULT hr = S_OK;
	ID3DBlob* pErrorBlob;

	//if (m_pVS)m_pVS->Release();
	//if (m_pPS)m_pPS->Release();
	//if (m_pVertexCode)m_pVertexCode->Release();
	//if (m_pPixelCode)m_pPixelCode->Release();

	hr = D3DX11CompileFromFile(ShaderFileName, NULL, NULL, VSName, "vs_5_0", 0, 0, NULL, &m_pVertexCode, &pErrorBlob, NULL);		//���� ���̴� ������
	if (FAILED(hr))
	{
		if (pErrorBlob == nullptr)
			return hr;
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		return hr;
	}
	hr = pd3dDevice->CreateVertexShader(m_pVertexCode->GetBufferPointer(), m_pVertexCode->GetBufferSize(), NULL, &m_pVS);	//���� ���̴� ����
	if (FAILED(hr))
	{
		return hr;
	}


	hr = D3DX11CompileFromFile(ShaderFileName, NULL, NULL, PSName, "ps_5_0", 0, 0, NULL, &m_pPixelCode, &pErrorBlob, NULL);			//�ȼ� ���̴� ������
	if (FAILED(hr))
	{
		if (pErrorBlob == nullptr)
			return hr;
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		return hr;
	}
	hr = pd3dDevice->CreatePixelShader(m_pPixelCode->GetBufferPointer(), m_pPixelCode->GetBufferSize(), NULL, &m_pPS);		//�ȼ� ���̴� ����
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

void CBY_Shadow::SetLTM(D3DXMATRIX* world, D3DXVECTOR3 vPos, D3DXVECTOR3 vAt, float fDistance, float fFar)
{
	m_matWorld = *world;

	D3DXVECTOR3 vEye = D3DXVECTOR3(vPos.x, vPos.y, vPos.z);
	D3DXVECTOR3 vLookat = vAt;
	D3DXVECTOR3 vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_matView, &vEye, &vLookat, &vUp);

	D3DXMatrixOrthoOffCenterLH(&m_matProj, -fDistance / 2, fDistance / 2, -fDistance / 2, fDistance / 2, 0.0f, fFar);

	m_matLTM = m_matWorld * m_matView * m_matProj * m_matTexture;
	D3DXMatrixTranspose(&m_matLTM, &m_matLTM);
}

void CBY_Shadow::GetLTM(D3DXMATRIX& world, D3DXMATRIX& View, D3DXMATRIX& Proj, D3DXMATRIX& LTM)
{
	world = m_matWorld;
	View = m_matView;
	Proj = m_matProj;
	LTM = m_matLTM;
}

//ID3D11ShaderResourceView CBY_Shadow::GetDSV()
//{
//	return m_Rt->m_pDsvSRV.GetAddressOf();
//}


CBY_Shadow::CBY_Shadow()
{
	m_matTexture = D3DXMATRIX(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
}


CBY_Shadow::~CBY_Shadow()
{
}
