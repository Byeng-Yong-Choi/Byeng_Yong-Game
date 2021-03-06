#pragma once
#include "C_Model.h"
class CShapeLine :public C_Model
{
	std::vector<PC_VERTEX>	m_VertexLineData;
public:
	HRESULT CreateVertexData();
	HRESULT CreateIndexData();
	HRESULT CreateInputLayout();
	HRESULT CreateVertexBuffer();
	bool    PostRender();
	bool	Draw(	D3DXVECTOR3 v0, 
					D3DXVECTOR3 v1,
					D3DXVECTOR4 color );
public:
	CShapeLine();
	virtual ~CShapeLine();
};

