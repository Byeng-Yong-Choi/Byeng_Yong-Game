#pragma once
#include "CDxHelper.h"

class CDxHelperEx
{
public:
	ID3D11Device*			m_pd3dDevice;		//디바이스
	ID3D11DeviceContext*	m_pContext;			//Context 랜더링에 필요

public:

	ID3D11Buffer* m_pVertexBuffer;				//버텍스 버퍼
	ID3D11Buffer* m_pIndexBuffer;				//인덱스 버퍼
	ID3D11Buffer* m_pConstantBuffer;			//상수 버퍼

	ID3D11InputLayout* m_pVertexLayout;			//버텍스 버퍼및 레이아웃
	ID3D11SamplerState*  m_pSamplerState;


	ID3D11VertexShader* m_pVS;					//정적 쉐이더
	ID3D11VertexShader* m_pVSmat;					//정적 쉐이더
	ID3D11PixelShader* m_pPS;					//픽셀 쉐이더
	ID3D11PixelShader* m_pPSBlend;
	ID3D11ShaderResourceView*  m_pSRV;

	ID3DBlob* m_pVertexCode;
	ID3DBlob* m_pPixelCode;

public:
	UINT m_VertexSize;				//정점 크기
	UINT m_Vertexnum;				//정점 개수
	UINT m_Indexnum;				//인덱스 개수
	int m_StartIndex;				//드로우 인덱스시 인덱스 시작 번호

public:
	void PrePender();
	void Pender();
	void PostPender();

public:
	CDxHelperEx();
	~CDxHelperEx();
};

