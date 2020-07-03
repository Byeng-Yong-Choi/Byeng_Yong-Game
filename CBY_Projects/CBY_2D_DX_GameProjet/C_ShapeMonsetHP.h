#pragma once
#include "CShapeObject.h"

class C_ShapeMonsetHP:public CShapeObject
{
public:
	virtual HRESULT CreateVertexData() override;
	virtual HRESULT CreateIndexData() override;

public:
	C_ShapeMonsetHP();
	virtual ~C_ShapeMonsetHP();
};

