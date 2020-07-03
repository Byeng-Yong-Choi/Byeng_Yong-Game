#pragma once
#include "CCamera.h"

class CBY_CharacterCamera : public CCamera
{
public:
	D3DXMATRIX m_mModelLastRot;
	D3DXMATRIX m_mModelRot;
	bool m_bClip;
	POINT m_Center;

public:
	virtual bool Frame();

public:
	virtual void Update(D3DXVECTOR4 rotvalue);
	virtual void SetClientCursor(fPOINT pot);
	virtual bool SetViewProj(float fFar = 1000.0f, float fNear = 1.0f)override;


public:
	CBY_CharacterCamera();
	virtual ~CBY_CharacterCamera();
};

