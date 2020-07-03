#pragma once
#include "CCamera.h"

class CModelViewCamera :public CCamera
{
public:
	
public:
	virtual bool Frame() override;

public:
	void Update(D3DXVECTOR4 rotvalue);

public:
	CModelViewCamera();
	virtual ~CModelViewCamera();
};

