#include "CCamera.h"

bool CCamera::SetViewProj()
{
	CreateViewMatrix(D3DXVECTOR3(0, 0.0f, -1.0f), D3DXVECTOR3(0, 0.0f, 0));
	float fAspect = (float)Winrt.right/(float)Winrt.bottom;
	CreateProjMatrix(1.0f, 1000.0f, D3DX_PI * 0.5f, fAspect);
	return true;
}

void CCamera::CreateViewMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 target, D3DXVECTOR3 up)
{
	m_Pos = pos;
	m_At = target;
	m_Up = up;
	D3DXMatrixLookAtLH(&m_View, &pos, &target, &up);
}

void CCamera::CreateProjMatrix(float fNear, float fFar, float fFovY, float fAspect)
{
	m_fNear = fNear;
	m_fFar = fFar;
	m_fFovY = fFovY;
	m_fAspect = fAspect;
	D3DXMatrixPerspectiveFovLH(&m_Proj, fFovY, fAspect, fNear, fFar);

	D3DXMatrixOrthoLH(&m_OrthoProj,
		Winrt.right,
		Winrt.bottom,
		fNear, fFar);
}

bool CCamera::Frame()
{
	D3DXMatrixLookAtLH(&m_View,&m_Pos,&m_At,&m_Up);
	return true;
}


CCamera::CCamera()
{
}


CCamera::~CCamera()
{
}
