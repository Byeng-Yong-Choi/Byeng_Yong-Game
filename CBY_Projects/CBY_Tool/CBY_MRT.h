#pragma once
#include "CBYCore.h"
#include "CBY_QuadTree.h"

class CBY_MRT : public CBYCore
{
public:
	ID3D11Texture2D* m_pOffScreenTex;
	ID3D11Texture2D* m_pOffColorScreenTex;
	ID3D11RenderTargetView* m_pOffRTV;
	ID3D11RenderTargetView* m_pOffColorRTV;
	ID3D11ShaderResourceView* m_pOffSRV;
	ID3D11ShaderResourceView* m_pOffColorSRV;

public:
	HRESULT CreateResource();
	void RenderIntoBuffer(CBY_QuadTree* quad);
	void RenderMRT(CBY_QuadTree* quad);

public:
	bool Release();

public:
	CBY_MRT();
	virtual ~CBY_MRT();
};

