#pragma once
#include "C_Model.h"

class CShapeBox : public C_Model
{
public:
	C_Model m_Model;
	HRESULT CreateVertexData();
	HRESULT CreateIndexData();

public:
	CShapeBox();
	~CShapeBox();
};

