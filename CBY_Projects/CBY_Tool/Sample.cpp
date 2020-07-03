#include "stdafx.h"
#include "Sample.h"


bool Sample::Create(int width, int height, int size, int quality, TCHAR* szTextur)
{
	SetRelease();

	m_iRow = width;
	m_iCol = height;

	m_Map = std::make_shared<C_Map_shape>();

	std::wstring szHeightMap;
	//szHeightMap = L"../../data/obj/maptool/height.bmp";

	if (szHeightMap[0] == NULL || szHeightMap.size() < 0)
	{
		m_Map->m_iNumCols = height + 1;
		m_Map->m_iNumRows = width + 1;
	}
	else
	{
		//m_Map->HeightMapLoad(szHeightMap.c_str(), m_pd3dDevice, m_pContext);
		m_Map->CBYHeightMapLoad(szHeightMap.c_str(), m_pd3dDevice, m_pContext);
	}
	//C_MapDesc mapdesc = { m_Map->m_iNumCols,m_Map->m_iNumRows, size, 1.0f,L"../../data/shader/LightBlend.txt", szTextur, "VSTile","PSTile" };		//싱글타일
	//C_MapDesc mapdesc = { m_Map->m_iNumCols,m_Map->m_iNumRows, size, 1.0f,L"../../data/shader/MapToolShader.txt", szTextur, "VSmat","PSAlpha" };	//응용
	//C_MapDesc mapdesc = { m_Map->m_iNumCols,m_Map->m_iNumRows, size, 1.0f,L"../../data/shader/MapToolShader.txt", szTextur, "VSmat","PSCSAlpha" };	//CS
	//C_MapDesc mapdesc = { m_Map->m_iNumCols,m_Map->m_iNumRows, size, 1.0f,L"../../data/shader/MapToolShader.txt", szTextur, "VSmat","PSCSHeight" };	//높이에 따른 텍스쳐
	//C_MapDesc mapdesc = { m_Map->m_iNumCols,m_Map->m_iNumRows, size, 1.0f,L"../../data/shader/MapToolShader.txt", szTextur, "VSmat","PSShadow" };	//그림자
	C_MapDesc mapdesc = { m_Map->m_iNumCols,m_Map->m_iNumRows, size, 1.0f,L"../../data/shader/MapToolShader_1.txt", szTextur, "VSmat","PSDSShadow" };	//깊이그림자
	if (FAILED(m_Map->Load(mapdesc, m_pd3dDevice, m_pContext)))
	{
		return false;
	}
	m_Map->m_ConData.ddata.x = m_Map->m_iNumRows;
	m_Map->m_ConData.ddata.z = m_Map->m_iNumCols;

	m_QuadTree = std::make_shared<CBY_QuadTree>();
	m_QuadTree->Build(m_Map.get(), m_pMainCamera);
	m_QuadTree->GetSelect(&m_Select);

	
	m_MiniMap->Create(m_pd3dDevice, m_pContext);
	m_MiniMap->m_RT->Create(m_pd3dDevice);

	
	m_Shadow->Release();
	
	//m_Shadow->Create(m_pd3dDevice, m_Map->m_iNumRows, m_Map->m_iNumCols);
	m_Shadow->m_DSFormat = DXGI_FORMAT_R32_TYPELESS;
	m_Shadow->Create(m_pd3dDevice, 2048, 2048);

	//SetTexture();
	SetCS();
	return true;
}

bool Sample::SetCS()
{

	ZeroMemory(&m_buf, sizeof(BufSplatting));

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = m_Map->m_iNumRows;
	desc.Height = m_Map->m_iNumCols;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS| D3D11_BIND_RENDER_TARGET;
	//desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	m_pd3dDevice->CreateTexture2D(&desc, NULL, &m_pAlphaTexture);								//알파 텍스쳐 생성
	m_pd3dDevice->CreateTexture2D(&desc, NULL, &m_pCopyMap);								//알파 텍스쳐 생성
	CDXH::CreateStructuredBuffer(m_pd3dDevice, sizeof(BufSplatting), 1, &m_buf, &m_pBuf0);		//구조화 버퍼 생성

	CDXH::CreateBufferSRV(m_pd3dDevice, m_pBuf0, &m_pBufSRV);									//구조화 버퍼 리소스
	CDXH::CreateTextureUAV(m_pd3dDevice, m_pAlphaTexture, &m_pBufResultUAV);					//RW할 텍스쳐 UAV생성

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;													//PS에 적용할 RW생성
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	//srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//D3D11_BIND_UNORDERED_ACCESS
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	//m_pd3dDevice->CreateShaderResourceView(m_pAlphaTexture, &srvDesc, &m_pBufResultSRV);
	m_pd3dDevice->CreateShaderResourceView(m_pCopyMap, &srvDesc, &m_pBufResultSRV);

	return true;
}

void Sample::SetTexture()
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = m_Map->m_iNumRows;
	desc.Height = m_Map->m_iNumCols;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	

	HRESULT hr=S_OK;

	hr = m_pd3dDevice->CreateTexture2D(&desc, NULL, &m_pAlphaTexture);
	hr = m_pd3dDevice->CreateTexture2D(&desc, NULL, &m_pHeightMap);
	
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	desc.CPUAccessFlags = 0;
	hr=m_pd3dDevice->CreateTexture2D(&desc, NULL, &m_pCopyMap);

	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	hr = m_pContext->Map((ID3D11Resource*)m_pAlphaTexture, 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest);
	if (SUCCEEDED(hr))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;

		for (UINT y = 0; y < desc.Height; y++)
		{
			for (UINT x = 0; x < desc.Width; x++)
			{
				
					int pixel = y * MappedFaceDest.RowPitch + (x * 4);
					BYTE* R = &pDestBytes[pixel];
					BYTE* G = &pDestBytes[pixel] + 1;
					BYTE* B = &pDestBytes[pixel] + 2;
					BYTE* A = &pDestBytes[pixel] + 3;

					*R = 0;
					*G = 0;
					*B = 0;
					*A = 0;
				
			}

		}

		m_pContext->Unmap(m_pAlphaTexture, 0);
	}

	CopyTexture(m_pAlphaTexture);
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = desc.MipLevels;
	hr = m_pd3dDevice->CreateShaderResourceView(m_pCopyMap, &SRVDesc, &m_pAlphaSRV);
	//LoadShader(L"../../data/shader/MapToolShader.txt", "VSmat", "PSAlpha");
}

void Sample::CopyTexture(ID3D11Texture2D* ptext)
{
	if (ptext)
	{
		m_pContext->CopyResource(m_pCopyMap, ptext);
		
	}
}

bool Sample::Tile(TCHAR* szTextur, int quality)
{
	bTile = true;
	bSplatting = false;
	if (m_dwSRVnum == 3)
	{
		MessageBox(g_hwnd, L"Tile이 가득 찼습니다.", L"실패", MB_OK);
		return false;
	}

	m_dwSRV = m_dwSRVnum+1;
	D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, szTextur, NULL, NULL, &m_pSRV[m_dwSRVnum], NULL);
	m_dwSRVnum++;
	MessageBox(g_hwnd, L"Tile지정 성공", L"성공", MB_OK);
	//C_MapDesc mapdesc = { m_Map->m_iNumCols,m_Map->m_iNumRows, size, 1.0f,L"../../data/shader/LightBlend.txt", szTextur,"VSmat","PSLight" };
	return true;
}

bool Sample::Splatting(TCHAR* szTextur, int quality)
{
	bTile = false;
	bSplatting = true;
	if (m_dwSptSRVnum == 4)
	{
		MessageBox(g_hwnd, L"Texture가 가득 찼습니다.", L"실패", MB_OK);
		return false;
	}

	m_dwSptSRV = m_dwSptSRVnum + 1;
	D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, szTextur, NULL, NULL, &m_pSptSRV[m_dwSptSRVnum], NULL);
	m_dwSptSRVnum++;
	MessageBox(g_hwnd, L"Texture지정 성공", L"성공", MB_OK);
	return true;
}

bool Sample::Init()
{
	m_MiniMap = std::make_shared<CBY_Minimap>();
	m_Shadow = std::make_shared<CBY_Shadow>();
	//LoadShader(L"../../data/shader/ShadowShader.txt", "VS", "PS");		//투영
	LoadShader(L"../../data/shader/ShadowShader.txt", "VSDS", "PSDS");		//깊이
	CDXH::CreateComputeShader(L"../../data/shader/ComputeAlpha.hlsl", "main", m_pd3dDevice, &m_pCS);
	m_vp.Width = 128;
	m_vp.Height = 128;
	m_vp.TopLeftX = 0;
	m_vp.TopLeftY = 0;
	m_vp.MinDepth = 0;
	m_vp.MaxDepth = 1.0f;

	CDX::ApplyBS(m_pContext, CDX::CDxState::g_pAlpahBlend);
	CDX::ApplySS(m_pContext, CDX::CDxState::g_pSampler);
	CDX::ApplyRS(m_pContext, CDX::CDxState::g_pRSSold);

	m_Camera = std::make_shared<C_DebugCamera>();
	m_Camera->SetViewProj();
	m_pMainCamera = m_Camera.get();
	m_pMainCamera->m_Pos += D3DXVECTOR3(0, 5.0f, -10.0f);

	D3DXVECTOR3 pos = D3DXVECTOR3(0, 20, 0);
	D3DXVECTOR3 at = D3DXVECTOR3(0, 0, 1);
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&m_matTopView,&pos,&at,&up);

	D3DXMatrixPerspectiveFovLH(&m_matTopProj, D3DX_PI * 0.4f, 1, 1, 20000);

	m_Light.g_cAmbientLightColor = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1);
	m_Light.g_cDifuseLightColor = D3DXVECTOR4(1, 1, 1, 1);

	m_Lightbuffer.Attach(CDXH::CreateConstantBuffer(m_pd3dDevice, &m_Light, 1, sizeof(CB_LIGHT)));

	/*m_pMainCamera->CreateViewMatrix(
		D3DXVECTOR3(0, 300, 0),
		D3DXVECTOR3(0, 0, 0));*/
	/*float fAspect = (float)Winrt.right / (float)Winrt.bottom;
	m_pMainCamera->CreateProjMatrix(0.01f, 50.0f,
		D3DX_PI * 0.25f,
		fAspect);*/

	CreateResource();

	box.Create(m_pd3dDevice, m_pContext, L"../../data/shader/MapToolShader_1.txt", L"../../data/map/rocks.jpg", "VSmat","PS");
	box.m_ConData.ddata = D3DXVECTOR4(1, 1, 1, 1);
	boxlight.Create(m_pd3dDevice, m_pContext, L"../../data/shader/MapToolShader_1.txt", L"../../data/map/baseColor.jpg", "VSmat", "PS");
	t = 0;
	return true;
}

bool Sample::Frame()
{
	if (m_Map == nullptr) return true;
	if (bButton)
	{
		m_Select.SetMarix(&m_pMainCamera->m_World, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
		if (m_iMode != HE_FALSE && m_iMode != HE_SPLATTING)
		{
			m_QuadTree->SetHeightMap(m_iMode);
			bSplatting = false;
		}

		if (bSplatting)
		{
			if (m_dwSptSRV != -1)
			{
				
				m_QuadTree->SplattingBufSet(&m_buf, m_dwSptSRV-1);
				m_pContext->UpdateSubresource(m_pBuf0, 0,nullptr, &m_buf, 0, 0);
				CDXH::RunComputeShader(m_pContext, m_pCS, 1, &m_pBufSRV, &m_pBufResultSRV,m_pBufResultUAV, m_iRow / 32+1, m_iCol / 32+1, 1);
				//m_QuadTree->Splatting(m_pAlphaTexture, m_dwSptSRV - 1, &m_buf);
			}
			CopyTexture(m_pAlphaTexture);
		}
	}

	if (I_Input.GetKeyCheck('D'))
	{
		m_pMainCamera->m_Pos += m_pMainCamera->m_Side *g_SecondTime * 100;
	}

	if (I_Input.GetKeyCheck('A'))
	{
		m_pMainCamera->m_Pos -= m_pMainCamera->m_Side *g_SecondTime * 100;
	}
	m_Light.g_vEyeDir.x = m_pMainCamera->m_Look.x;
	m_Light.g_vEyeDir.y = m_pMainCamera->m_Look.y;
	m_Light.g_vEyeDir.z = m_pMainCamera->m_Look.z;
	m_Light.g_vEyeDir.w = 1;

	t += D3DX_PI * g_SecondTime/2;
	float fcos = cosf(t);
	float fsin = sinf(t);

	float a = I_Col.Lerp(-10, 10, fcos);
	float b = I_Col.Lerp(-10, 10, fsin);
	m_Light.g_vEyePos.x = a;//m_pMainCamera->m_Pos.x;
	m_Light.g_vEyePos.y = 20;//m_pMainCamera->m_Pos.y;
	m_Light.g_vEyePos.z = b;//m_pMainCamera->m_Pos.z;
	m_Light.g_vEyePos.w = 1;

	m_Light.g_vLightDir.x = 0;
	m_Light.g_vLightDir.y = -1;
	m_Light.g_vLightDir.z = 0;
	m_Light.g_vLightDir.w = 1;

	m_Light.g_vLightPos.x = 0;
	m_Light.g_vLightPos.y = 50.0f;
	m_Light.g_vLightPos.z = 0;
	m_Light.g_vLightPos.w = 1;

	/*m_Light.g_vLightPos.x = a;
	m_Light.g_vLightPos.y = 20;
	m_Light.g_vLightPos.z = b;
	m_Light.g_vLightPos.w = 1;

	D3DXVECTOR3 at= D3DXVECTOR3(0,0,0);
	D3DXVECTOR3 dir = D3DXVECTOR3(at.x- m_Light.g_vLightPos.x, at.y - m_Light.g_vLightPos.y, at.z - m_Light.g_vLightPos.z);
	
	D3DXVec3Normalize(&dir, &dir);

	m_Light.g_vLightDir.x = dir.x;
	m_Light.g_vLightDir.y = dir.y;
	m_Light.g_vLightDir.z = dir.z;
	m_Light.g_vLightDir.w = 1;*/
	
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	//D3DXMatrixTranslation(&world, 0, 10, 0);

	SetLTM(&world);

	m_pContext->UpdateSubresource(m_Lightbuffer.Get(), 0, NULL, &m_Light, 0, 0);
	//m_Map->SetMatrix(nullptr, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
	//m_Map->SetMatrix(nullptr,&m_matTopView,&m_matTopProj);
	m_MiniMap->SetMatrix(nullptr,
		&m_matTopView,
		&m_matTopProj);

	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_QuadTree->Frame();

	MiniMapCopy();

	D3DXMatrixIdentity(&world);
	D3DXMatrixTranslation(&world, a, 20, b);
	boxlight.SetMatrix(&world,&m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
	return true;
}

bool Sample::Render()
{
	//return true;
	if (m_Map == nullptr) return true;

	CDX::ApplyBS(m_pContext, CDX::CDxState::g_pAlpahBlend);
	CDX::ApplySS(m_pContext, CDX::CDxState::g_pSampler,1);
	CDX::ApplySS(m_pContext, CDX::CDxState::g_pSamplShadow,2);
	CDX::ApplyRS(m_pContext, CDX::CDxState::g_pRSSold);

	m_Map->SetMatrix(nullptr, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
	m_pContext->VSSetConstantBuffers(1, 1, m_Lightbuffer.GetAddressOf());
	m_pContext->PSSetConstantBuffers(1, 1, m_Lightbuffer.GetAddressOf());
	//m_pContext->PSSetShaderResources(1, 1, m_pSRV);			//타일링 텍스쳐 리소스를 넘겨주는 작업
	m_pContext->PSSetShaderResources(1, 1, &m_pAlphaSRV);
	m_pContext->PSSetShaderResources(3, 1, &m_pBufResultSRV);
	m_pContext->PSSetShaderResources(4, 3, m_pSRV);			//타일링 텍스쳐 리소스를 넘겨주는 작업
	m_pContext->PSSetShaderResources(7, 4, m_pSptSRV);		
	m_pContext->PSSetShaderResources(11, 1, m_Shadow->m_Rt->m_pDsvSRV.GetAddressOf());

	boxlight.Render();
	m_QuadTree->Render();
	//RenderIntoBuffer(m_QuadTree.get());

	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//box.SetMatrix(nullptr, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
	
	//PreRender();

	//m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	//m_pContext->RSSetViewports(1, &m_ViewPort);					//뷰포트 세팅
	//CDX::ApplyBS(m_pContext, CDX::CDxState::g_pAlpahBlend);
	//CDX::ApplySS(m_pContext, CDX::CDxState::g_pSampler);
	//CDX::ApplyRS(m_pContext, CDX::CDxState::g_pRSSold);
	//box.PreRender();
	//m_pContext->PSSetShaderResources(0, 1, &m_pOffColorSRV);
	//box.PostRender();

	//SplattingRender();
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	//float t = g_gametime * D3DX_PI;
	D3DXMatrixTranslation(&world, 0, 10, 0);
	box.SetMatrix(&world, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
	box.Render();

	m_pContext->RSSetViewports(1, &m_vp);
	if (m_MiniMap->m_RT->m_pTexture != nullptr)
	{
		m_MiniMap->Render();
	}
	m_pContext->RSSetViewports(1, &m_ViewPort);

	//m_QuadTree->DrawNodeLine();
	//m_Map->Render();
	ID3D11ShaderResourceView* ppSRVNULL = NULL;
	m_pContext->PSSetShaderResources(3, 1, &ppSRVNULL);
	return true;
}

void Sample::MiniMapCopy()
{
	m_pContext->VSSetConstantBuffers(1, 1, m_Lightbuffer.GetAddressOf());
	m_pContext->PSSetConstantBuffers(1, 1, m_Lightbuffer.GetAddressOf());
	m_Map->SetMatrix(nullptr,&m_matTopView,&m_matTopProj);
	m_MiniMap->m_RT->Begin(m_pContext);

	m_QuadTree->Render();


	/*D3DXVECTOR3 vS = m_pMainCamera->m_Frustum.m_vFrustum[2];
	D3DXVECTOR3 vE = m_pMainCamera->m_Frustum.m_vFrustum[3];
	m_DebugLine->SetMatrix(nullptr, &m_matTopView,
		&m_matTopProj);
	m_DebugLine->Draw(vS, vE, D3DXVECTOR4(1, 0, 0, 1));
	vS = m_pMainCamera->m_Frustum.m_vFrustum[5];
	vE = m_pMainCamera->m_Frustum.m_vFrustum[6];
	m_DebugLine->Draw(vS, vE, D3DXVECTOR4(1, 0, 0, 1));
	vS = m_pMainCamera->m_Frustum.m_vFrustum[1];
	vE = m_pMainCamera->m_Frustum.m_vFrustum[5];
	m_DebugLine->Draw(vS, vE, D3DXVECTOR4(1, 0, 0, 1));
	vS = m_pMainCamera->m_Frustum.m_vFrustum[2];
	vE = m_pMainCamera->m_Frustum.m_vFrustum[6];
	m_DebugLine->Draw(vS, vE, D3DXVECTOR4(1, 0, 0, 1));
*/
	m_MiniMap->m_RT->End(m_pContext);

	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Sample::SplattingRender()
{
	m_pContext->VSSetShader(m_pVS, NULL, 0);
	m_pContext->PSSetShader(m_pPS, NULL, 0);
	m_QuadTree->NonPreRender();
}

bool Sample::Release()
{
	CBY_MRT::Release();
	SetRelease();
	if (m_pVS)m_pVS->Release();
	if (m_pPS)m_pPS->Release();
	if (m_pVertexCode)m_pVertexCode->Release();
	if (m_pPixelCode)m_pPixelCode->Release();

	if (m_Shadow)
	{
		m_Shadow->Release();
	}

	if (m_QuadTree)
	{
		m_QuadTree->Release();
	}

	if (m_Map)
	{
		m_Map->Release();
	}

	if (m_Camera)
	{
		m_Camera->Release();
	}

	if (m_MiniMap)
	{
		m_MiniMap->Release();
	}

	return true;
}

void Sample::SetRelease()
{
	bTile = false;
	bSplatting = false;
	m_iMode = 3;
	bButton = false;

	if (m_Map != nullptr)
	{
		m_Map->Release();
	}
	m_Map = 0;
	if (m_QuadTree != nullptr)
	{
		m_QuadTree->Release();
	}
	m_QuadTree = 0;
	if (m_MiniMap != nullptr)
	{
		m_MiniMap->Release();
	}

	for (DWORD i = 0; i < 3; i++)
	{
		if (m_pSRV[i])m_pSRV[i]->Release();
	}

	m_dwSRVnum = 0;
	m_dwSRV = -1;


	for (DWORD i = 0; i < 4; i++)
	{
		if (m_pSptSRV[i])
		{
			m_pSptSRV[i]->Release();
			m_pSptSRV[i] = 0;
		}

	}
	m_dwSptSRVnum = 0;
	m_dwSptSRV = -1;

	if (m_pAlphaSRV)
	{
		m_pAlphaSRV->Release();
		m_pAlphaSRV = 0;
	}

	if (m_pAlphaTexture)
	{
		m_pAlphaTexture->Release();
		m_pAlphaTexture = 0;
	}

	if (m_pHeightMap)
	{
		m_pHeightMap->Release();
		m_pHeightMap = 0;
	}

	if (m_pCopyMap)
	{
		m_pCopyMap->Release();
		m_pCopyMap = 0;
	}

	if (m_pBuf0)
	{
		m_pBuf0->Release();
		m_pBuf0 = 0;
	}

	if (m_pBufSRV)
	{
		m_pBufSRV->Release();
		m_pBufSRV = 0;
	}

	if (m_pBufResultUAV)
	{
		m_pBufResultUAV->Release();
		m_pBufResultUAV = 0;
	}

	if (m_pBufResultSRV)
	{
		m_pBufResultSRV->Release();
		m_pBufResultSRV = 0;
	}
}

void Sample::SetHeightMode(int mode)
{
	m_iMode = mode;
	if (m_iMode == HE_SPLATTING)
	{
		bSplatting = true;
	}
}

LRESULT Sample::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDBLCLK:
	{
		m_Select.SetMarix(&m_pMainCamera->m_World, &m_pMainCamera->m_View, &m_pMainCamera->m_Proj);
		//m_QuadTree->PickingSelect();
		if (bTile)
		{
			if (m_dwSRV != -1)
			{
				m_QuadTree->TileSet(m_dwSRV);
			}
		}
		break;
	}

	case WM_LBUTTONDOWN:
	{
		bButton = true;
	}
	break;

	case WM_LBUTTONUP:
	{
		bButton = false;
	}
	break;
	
	}

	return CBYCore::WndProc(hWnd, message, wParam, lParam);
}

HRESULT Sample::SaveFile(T_STR name, DWORD type)
{
	HRESULT hr = S_OK;
	switch (type)
	{
	case 0:
	{
		m_QuadTree->SaveheightMap(m_pHeightMap);

		if (m_pHeightMap != nullptr)
		{
			// 랜더타켓 텍스처를 텍스처 파일로 저장
			if (FAILED(hr = D3DX11SaveTextureToFile(m_pContext, m_pHeightMap, D3DX11_IFF_BMP, name.c_str())))
			{
				return true;
			}
		}
	}
	break;

	case 1:
	{
		if (m_pCopyMap != nullptr)
		{
			// 랜더타켓 텍스처를 텍스처 파일로 저장
			if (FAILED(hr = D3DX11SaveTextureToFile(m_pContext, m_pCopyMap, D3DX11_IFF_BMP, name.c_str())))
			{
				return true;
			}
		}
		else if (m_pAlphaSRV != nullptr)
		{
			ID3D11Resource* resource;
			m_pAlphaSRV->GetResource(&resource);

			ID3D11Texture2D* texture;
			if (FAILED(hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&texture))))
			{
				return hr;
			}
			if (FAILED(hr = D3DX11SaveTextureToFile(m_pContext, texture, D3DX11_IFF_BMP, name.c_str())))
			{
				return hr;
			}
			texture->Release();
		}
	}
	break;
	}

	return hr;
}

Sample::Sample()
{
	m_pAlphaTexture = NULL;
	m_pCopyMap = NULL;
	m_matTexture = D3DXMATRIX(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
}


Sample::~Sample()
{
}

void Sample::SetLTM(D3DXMATRIX* world)
{
	D3DXMATRIX matworld, matview, matproj;
	matworld = *world;
	
	D3DXVECTOR3 vEye = D3DXVECTOR3(m_Light.g_vEyePos.x, m_Light.g_vEyePos.y, m_Light.g_vEyePos.z);
	D3DXVECTOR3 vLookat = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//D3DXMatrixLookAtLH(&matview, &vEye, &vLookat, &vUp);

	float MaxDistance = sqrt((m_Map->m_iNumRows*m_Map->fCellDistance*m_Map->m_iNumRows*m_Map->fCellDistance) +
		(m_Map->m_iNumCols*m_Map->fCellDistance*m_Map->m_iNumCols*m_Map->fCellDistance));
	//D3DXMatrixOrthoOffCenterLH(&matproj, -MaxDistance / 2, MaxDistance / 2, -MaxDistance / 2, MaxDistance / 2, 0.0f, 100.0f);
	
	m_Shadow->SetLTM(&matworld, vEye, vLookat, MaxDistance);
	m_Shadow->GetLTM(matworld, matview, matproj, m_LTM[0]);
	//m_LTM[0] = matworld * matview * matproj * m_matTexture;
	//D3DXMatrixTranspose(&m_LTM[0], &m_LTM[0]);
	m_Light.g_LTM = m_LTM[0];

	D3DXMATRIX boxworld;
	D3DXMatrixIdentity(&boxworld);
	D3DXMatrixTranslation(&boxworld, 0, 10, 0);
	Shadow(&boxworld, &matview, &matproj);
}

void Sample::Shadow(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	m_Shadow->Begin(m_pContext);
	box.SetMatrix(world, view, proj);
	box.PreRender();
	//m_Shadow->ShaderSet(m_pContext);
	m_pContext->VSSetConstantBuffers(1, 1, m_Lightbuffer.GetAddressOf());
	m_pContext->PSSetConstantBuffers(1, 1, m_Lightbuffer.GetAddressOf());
	m_pContext->VSSetConstantBuffers(0, 1, box.m_obj.m_pConstantBuffer.GetAddressOf());
	m_pContext->PSSetConstantBuffers(0, 1, box.m_obj.m_pConstantBuffer.GetAddressOf());
	box.PostRender();
	
	m_Map->SetMatrix(nullptr, view, proj);
	m_QuadTree->m_pMap->PreRender();
	//m_Shadow->ShaderSet(m_pContext);
	m_pContext->VSSetConstantBuffers(1, 1, m_Lightbuffer.GetAddressOf());
	m_pContext->PSSetConstantBuffers(1, 1, m_Lightbuffer.GetAddressOf());
	m_QuadTree->NonPreRender();

	m_Shadow->End(m_pContext);
}

HRESULT Sample::LoadShader(const TCHAR* ShaderFileName, const CHAR* VSName, const CHAR* PSName)			//쉐이더 로드
{
	HRESULT hr = S_OK;
	ID3DBlob* pErrorBlob;

	if (m_pVS)m_pVS->Release();
	if (m_pPS)m_pPS->Release();
	if (m_pVertexCode)m_pVertexCode->Release();
	if (m_pPixelCode)m_pPixelCode->Release();

	hr = D3DX11CompileFromFile(ShaderFileName, NULL, NULL, VSName, "vs_5_0", 0, 0, NULL, &m_pVertexCode, &pErrorBlob, NULL);		//정점 쉐이더 컴파일
	if (FAILED(hr))
	{
		if (pErrorBlob == nullptr)
			return hr;
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		return hr;
	}
	hr = m_pd3dDevice->CreateVertexShader(m_pVertexCode->GetBufferPointer(), m_pVertexCode->GetBufferSize(), NULL, &m_pVS);	//정점 쉐이더 생성
	if (FAILED(hr))
	{
		return hr;
	}


	hr = D3DX11CompileFromFile(ShaderFileName, NULL, NULL, PSName, "ps_5_0", 0, 0, NULL, &m_pPixelCode, &pErrorBlob, NULL);			//픽셀 쉐이더 컴파일
	if (FAILED(hr))
	{
		if (pErrorBlob == nullptr)
			return hr;
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		return hr;
	}
	hr = m_pd3dDevice->CreatePixelShader(m_pPixelCode->GetBufferPointer(), m_pPixelCode->GetBufferSize(), NULL, &m_pPS);		//픽셀 쉐이더 생성
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

void Sample::SetRadian(UINT i)
{
	m_QuadTree->SetRadian(i);
}