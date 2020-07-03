#include "stdafx.h"
#include "CBY_SkyBox.h"
#define D3DX_FROM_FILE          ((UINT) -3)

HRESULT CBY_SkyBox::LoadTextureArray(ID3D11Device* pd3dDevice, ID3D11DeviceContext*    pContext, TCHAR** szTextureNames, int iNumTextures,
	ID3D11Texture2D** ppTex2D, ID3D11ShaderResourceView** ppSRV)
{
	HRESULT hr = S_OK;
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));

	WCHAR str[MAX_PATH];
	for (int i = 0; i < iNumTextures; i++)
	{
		ID3D11Resource* pRes = NULL;
		D3DX11_IMAGE_LOAD_INFO loadInfo;
		ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
		loadInfo.Width = D3DX11_DEFAULT;
		loadInfo.Height = D3DX11_DEFAULT;
		loadInfo.Depth = D3DX11_DEFAULT;
		loadInfo.FirstMipLevel = 0;
		loadInfo.MipLevels = 1;
		loadInfo.Usage = D3D11_USAGE_STAGING;
		loadInfo.BindFlags = 0;
		loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		loadInfo.MiscFlags = 0;
		loadInfo.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
		loadInfo.Filter = D3DX11_FILTER_LINEAR;
		loadInfo.MipFilter = D3DX11_FILTER_LINEAR;

		D3DX11CreateTextureFromFile(pd3dDevice, szTextureNames[i], &loadInfo, NULL, &pRes, NULL);
		if (pRes)
		{
			ID3D11Texture2D* pTemp;
			pRes->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTemp);
			pTemp->GetDesc(&desc);

			if (!(*ppTex2D))
			{
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				desc.CPUAccessFlags = 0;
				desc.ArraySize = iNumTextures;
				pd3dDevice->CreateTexture2D(&desc, NULL, ppTex2D);
			}

			for (UINT iMip = 0; iMip < desc.MipLevels; iMip++)
			{
				D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
				if (SUCCEEDED(pContext->Map((ID3D11Resource*)pTemp, 0, D3D11_MAP_READ, 0, &MappedFaceDest)))
				{
					UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
					pContext->UpdateSubresource((*ppTex2D),
						D3D10CalcSubresource(iMip, i, desc.MipLevels),
						NULL,
						MappedFaceDest.pData,
						MappedFaceDest.RowPitch,
						0);
					pContext->Unmap(pTemp, 0);
				}
			}

			SAFE_RELEASE(pRes);
			SAFE_RELEASE(pTemp);
		}
		else
		{
			return false;
		}
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	// 원본 컬러를 사용( rgb )
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// SRGB 포맷으로 변환하면 색상이 강해진다.
	//SRVDesc.Format = MAKE_SRGB(DXGI_FORMAT_R8G8B8A8_UNORM);
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	SRVDesc.Texture2DArray.MipLevels = desc.MipLevels;
	SRVDesc.Texture2DArray.ArraySize = iNumTextures;
	pd3dDevice->CreateShaderResourceView(*ppTex2D, &SRVDesc, ppSRV);
	return hr;
}

bool CBY_SkyBox::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context, const TCHAR* ShaderFileName, const TCHAR* TexFileName)
{
	if (CShapeBox::Create(pd3dDevice, Context, ShaderFileName, TexFileName) == false)
	{
		MessageBox(0, _T("m_pDirectionLIne 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	CreateTextureArray(pd3dDevice, Context);
	return true;
}


HRESULT CBY_SkyBox::CreateTextureArray(ID3D11Device* pDevice, ID3D11DeviceContext*    pContext)
{
	HRESULT hr = S_OK;

	TCHAR* g_szSkyTextures[] =
	{
		L"../../data/sky/ft.bmp",
		L"../../data/sky/bk.bmp",
		L"../../data/sky/rt.bmp",
		L"../../data/sky/lt.bmp",
		L"../../data/sky/up.bmp",
		L"../../data/sky/dn.bmp"
	};
	int iNumTexture = sizeof(g_szSkyTextures) / sizeof(g_szSkyTextures[0]);
	//--------------------------------------------------------------------------------------
	// 서브리소스를 사용하는 방법
	//--------------------------------------------------------------------------------------
	if (FAILED(hr = LoadTextureArray(pDevice, pContext, g_szSkyTextures, iNumTexture,
		m_pSkyTexture.GetAddressOf(),
		m_pSkyTexRV.GetAddressOf())))
	{
		return hr;
	}
	return hr;
}
bool CBY_SkyBox::Render()
{
	//D3DXMatrixScaling(&m_matWorld, 10.0f, 10.0f, 10.0f);
	D3DXMATRIX matSkyBoxView = m_matView;
	matSkyBoxView._41 = 0;
	matSkyBoxView._42 = 0;
	matSkyBoxView._43 = 0;

	CDX::ApplySS(m_obj.m_pContext, CDX::CDxState::g_pSSClampPoint);
	CDX::ApplyRS(m_obj.m_pContext, CDX::CDxState::g_pRSSold);
	CDX::ApplyDSS(m_obj.m_pContext, CDX::CDxState::g_pDSSDisable);

	SetMatrix(&m_matWorld, &matSkyBoxView, &m_matProj);

	PreRender();
	m_obj.m_pContext->PSSetShaderResources(0, 1, m_pSkyTexRV.GetAddressOf());
	//m_obj.m_pContext->PSSetShader(m_pPSTextureArray.Get(), NULL, 0);
	PostRender();

	CDX::ApplyBS(m_obj.m_pContext, CDX::CDxState::g_pAlpahBlend);
	CDX::ApplySS(m_obj.m_pContext, CDX::CDxState::g_pSampler);
	CDX::ApplyRS(m_obj.m_pContext, CDX::CDxState::g_pRSSold);
	CDX::ApplyDSS(m_obj.m_pContext, CDX::CDxState::g_pDSS);


	return true;
}

CBY_SkyBox::CBY_SkyBox()
{
	m_iRenderType = 3;
	m_pSkyTexture = nullptr;
	m_pSkyTexRV = nullptr;
}
CBY_SkyBox::~CBY_SkyBox()
{
}
