#pragma once
#include "Cstd.h"
#include "C_ArcBall.h"
#include "CBY_Frusutm.h"

#define MOUSE_LEFT_BUTTON   0x01
#define MOUSE_MIDDLE_BUTTON 0x02
#define MOUSE_RIGHT_BUTTON  0x04
#define MOUSE_WHEEL         0x08

class CCamera
{
public:
	CBY_Frusutm m_Frustum;

public:
	D3DXMATRIX m_View;
	D3DXMATRIX m_Proj;
	D3DXMATRIX m_OrthoProj;
	D3DXMATRIX m_World;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_At;
	D3DXVECTOR3 m_DefaultUp;
	D3DXVECTOR3 m_Look;
	D3DXVECTOR3 m_Side;
	D3DXVECTOR3 m_Up;
	D3DXVECTOR3 m_BackPos;
	D3DXVECTOR3	m_vModelCenter;
	D3DXMATRIX m_mModelLastRot;
	D3DXMATRIX m_mModelRot;

public:
	float m_fNear;
	float m_fFar;
	float m_fFovY;
	float m_fAspect;
	float m_fYaw;
	float m_fPitch;
	float m_fRoll;
	float m_fDefaultRadius;
	float m_fRadius;
	float m_fMinRadius;
	float m_fMaxRadius;
	float m_fSpeed;

public:
	C_ArcBall  m_WorldArcBall;
	C_ArcBall  m_ViewArcBall;
	int   m_nMouseWheelDelta;

public:
	virtual bool	SetViewProj(float fFar=1000.0f, float fNear=1.0f);
	virtual void	CreateViewMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 target, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));
	virtual void	CreateProjMatrix(float fNear,float fFar,float fFovY,float fAspect);
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	virtual void MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void SetRadius(float fDefaultRadius = 5.0f, float fMinRadius = 1.0f, float fMaxRadius = FLT_MAX);
	void	UpdateVector();
	int	CheckOBBInPlane(CBY_Box& box);
	void SetModelCenter(D3DXVECTOR3 vModelCenter);

public:
	CCamera();
	~CCamera();
};

