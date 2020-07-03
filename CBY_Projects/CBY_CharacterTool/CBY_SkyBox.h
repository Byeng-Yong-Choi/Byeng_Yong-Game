#pragma once
#include "CShapeBox.h"

#define MAX_SKYBOX_TEXTURE	6

class CBY_SkyBox : public CShapeBox
{
public:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_pTexSRV[MAX_SKYBOX_TEXTURE];
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			m_pPSTextrueIndex;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			m_pPSTextureArray;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			m_pPSCubeTexture;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_pSkyTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_pSkyTexRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_pCubeTexSRV;
	
	int m_iRenderType;
public:
	bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* Context, const TCHAR* ShaderFileName, const TCHAR* TexFileName = 0);
	bool Render();
	HRESULT	LoadTextureArray(ID3D11Device* pd3dDevice, ID3D11DeviceContext*    pContext, TCHAR** szTextureNames, int iNumTextures, ID3D11Texture2D** ppTex2D,
		ID3D11ShaderResourceView** ppSRV);
	HRESULT	CreateTextureArray(ID3D11Device* pDevice, ID3D11DeviceContext*    pContext);
public:
	CBY_SkyBox();
	virtual ~CBY_SkyBox();
};


