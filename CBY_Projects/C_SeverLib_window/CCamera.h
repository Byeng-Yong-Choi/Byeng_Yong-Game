#pragma once
#include "C_NetStd.h"

class CCamera
{
public:
	D3DXMATRIX m_View;
	D3DXMATRIX m_Proj;
	D3DXMATRIX m_OrthoProj;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_At;
	D3DXVECTOR3 m_Up;

	float m_fNear;
	float m_fFar;
	float m_fFovY;
	float m_fAspect;
public:
	bool	SetViewProj();
	void	CreateViewMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 target, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));
	void	CreateProjMatrix(float fNear,float fFar,float fFovY,float fAspect);
	bool	Frame();
public:
	CCamera();
	~CCamera();
};

