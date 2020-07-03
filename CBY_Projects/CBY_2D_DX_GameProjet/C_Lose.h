#pragma once
#include "CShapeObject.h"

class C_Lose :public CShapeObject
{
public:
	bool bContinue;

public:
	HRESULT CreateVertexData() override;
	bool Frame();

public:
	C_Lose();
	~C_Lose();
};


