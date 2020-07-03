#pragma once
#include "CCamera.h"
class C_DebugCamera:public CCamera
{
public:
	bool  m_bDrag;
	POINT m_LastPos;

public:
	virtual bool Frame() override;
	virtual void MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

public:
	void Update(D3DXVECTOR4 rotvalue);

public:
	C_DebugCamera();
	~C_DebugCamera();
};

