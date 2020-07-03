#include "stdafx.h"
#include "CBY_MRT.h"


HRESULT CBY_MRT::CreateResource()
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC desc;		//백버퍼 대신 입력될 텍스쳐를 생성한다.
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = m_SwapChainDesc.BufferDesc.Width;
	desc.Height = m_SwapChainDesc.BufferDesc.Height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	m_pd3dDevice->CreateTexture2D(&desc, NULL, &m_pOffScreenTex);
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_pd3dDevice->CreateTexture2D(&desc, NULL, &m_pOffColorScreenTex);

	D3D11_RENDER_TARGET_VIEW_DESC rtdesc;			//랜더타켓뷰도 생성
	rtdesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	rtdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtdesc.Texture2D.MipSlice = 0;

	m_pd3dDevice->CreateRenderTargetView(m_pOffScreenTex, &rtdesc, &m_pOffRTV);
	rtdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_pd3dDevice->CreateRenderTargetView(m_pOffColorScreenTex, &rtdesc, &m_pOffColorRTV);

	D3D11_SHADER_RESOURCE_VIEW_DESC shdesc;			//쉐이더 리소스 뷰 생성
	shdesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	shdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shdesc.Texture2D.MostDetailedMip = 0;
	shdesc.Texture2D.MipLevels = desc.MipLevels;

	m_pd3dDevice->CreateShaderResourceView(m_pOffScreenTex, &shdesc, &m_pOffSRV);

	shdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_pd3dDevice->CreateShaderResourceView(m_pOffColorScreenTex, &shdesc, &m_pOffColorSRV);

	return hr;
}

void CBY_MRT::RenderIntoBuffer(CBY_QuadTree* quad)
{
	float color[4] = { 0,0,0,0 };
	m_pContext->ClearRenderTargetView(m_pOffRTV, color);
	m_pContext->ClearRenderTargetView(m_pOffColorRTV, color);		//랜더타켓뷰 청소

	ID3D11RenderTargetView* pOldRTV;
	ID3D11DepthStencilView* pOldDSV;
	m_pContext->OMGetRenderTargets(1, &pOldRTV, &pOldDSV);
	m_pContext->ClearDepthStencilView(pOldDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);


	ID3D11RenderTargetView* pViews[2];
	pViews[0] = m_pOffRTV;
	pViews[1] = m_pOffColorRTV;
	m_pContext->OMSetRenderTargets(2, pViews, pOldDSV);
	
	RenderMRT(quad);		//랜더링

	pViews[0] = pOldRTV;									//해제 작업
	pViews[1] = NULL;
	m_pContext->OMSetRenderTargets(2, pViews, pOldDSV);
	SAFE_RELEASE(pOldRTV);
	SAFE_RELEASE(pOldDSV);

}

void CBY_MRT::RenderMRT(CBY_QuadTree* quad)
{
	quad->Render();
}

bool CBY_MRT::Release()
{
	if (m_pOffScreenTex)
	{
		m_pOffScreenTex->Release();
		m_pOffScreenTex = 0;
	}

	if (m_pOffColorScreenTex)
	{
		m_pOffColorScreenTex->Release();
		m_pOffColorScreenTex = 0;
	}

	if (m_pOffRTV)
	{
		m_pOffRTV->Release();
		m_pOffRTV = 0;
	}

	if (m_pOffColorRTV)
	{
		m_pOffColorRTV->Release();
		m_pOffColorRTV = 0;
	}

	if (m_pOffSRV)
	{
		m_pOffSRV->Release();
		m_pOffSRV = 0;
	}

	if (m_pOffColorSRV)
	{
		m_pOffColorSRV->Release();
		m_pOffColorSRV = 0;
	}
	return true;
}

CBY_MRT::CBY_MRT()
{
}


CBY_MRT::~CBY_MRT()
{
}
