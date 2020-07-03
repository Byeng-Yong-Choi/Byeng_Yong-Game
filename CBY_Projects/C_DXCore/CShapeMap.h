#pragma once
#include "C_Model.h"


class CShapeMap : public C_Model
{

public:
	C_Model m_Model;

public:
	virtual void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj);
	HRESULT CreateVertexData();
	HRESULT CreateIndexData();
	virtual void UpdateVertexData(std::vector<PNCT_VERTEX>& list);
public:
	CShapeMap();
	~CShapeMap();
};

