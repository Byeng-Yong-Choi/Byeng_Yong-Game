#pragma once
#include "C_Model.h"

class CShapeObject: public C_Model
{
public:
	C_Model m_Model;

public:
	virtual HRESULT CreateVertexData();
	virtual HRESULT CreateIndexData();
	virtual void UpdateVertexData(std::vector<PNCT_VERTEX>& list);					//정점 버퍼 데이터 업데이트

public:
	CShapeObject();
	~CShapeObject();
};

