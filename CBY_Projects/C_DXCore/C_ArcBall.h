#pragma once
#include "Cstd.h"

class C_ArcBall
{
public:
	bool			m_bDrag;
	D3DXQUATERNION	m_qDown;
	D3DXQUATERNION	m_qNow;
	D3DXVECTOR3		m_vDownPt;
	D3DXVECTOR3		m_vCurrentPt;

	D3DXMATRIX      m_matRotation;
	D3DXMATRIX      m_matXRotation;

public:
	void  OnBegin(int nX, int nY);
	void  OnMove(int nX, int nY);
	void  OnEnd();
	D3DXVECTOR3 ScreenToVector(float nX, float nY);
	D3DXQUATERNION QuatFromBallPoints(
		const D3DXVECTOR3& vFrom,
		const D3DXVECTOR3& vTo);
	inline D3DXMATRIX* GetRotationMatrix()
	{
		return D3DXMatrixRotationQuaternion(&m_matRotation, &m_qNow);
	}
	inline D3DXMATRIX* GetRotationXMatrix()
	{
		return D3DXMatrixRotationQuaternion(&m_matXRotation, &m_qXNow);
	}
	inline void	SetQuatNow(D3DXQUATERNION q) { m_qNow = q; }

public:
	D3DXQUATERNION	m_qXDown;
	D3DXQUATERNION	m_qXNow;
	D3DXVECTOR3		m_vXDownPt;
	D3DXVECTOR3		m_vXCurrentPt;
	D3DXVECTOR3 ScreenToVectorX(float nX);

public:
	C_ArcBall();
	virtual ~C_ArcBall();
};

