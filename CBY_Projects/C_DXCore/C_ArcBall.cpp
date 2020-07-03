#include "C_ArcBall.h"

// 화면좌표계를 구좌표계로 변환
D3DXVECTOR3 C_ArcBall::ScreenToVector(float nX, float nY)
{
	float x = (nX - (Winrt.right / 2)) / (Winrt.right / 2);
	float y = ( (Winrt.bottom / 2) - nY) / (Winrt.bottom / 2);
	float z = 0.0f;

	float mag = x * x + y * y;
	if (mag > 1.0f)
	{
		float scale = 1.0f / sqrtf(mag);
		x *= scale;
		y *= scale;
	}
	else
	{
		z = sqrtf(1.0f - mag);
	}
	return D3DXVECTOR3(x, y, z);
}

D3DXVECTOR3 C_ArcBall::ScreenToVectorX(float nX)
{
	float x = (nX - (Winrt.right / 2)) / (Winrt.right / 2);
	float y = 0;
	float z = 0.0f;

	float mag = x * x + y * y;
	if (mag > 1.0f)
	{
		float scale = 1.0f / sqrtf(mag);
		x *= scale;
		y *= scale;
	}
	else
	{
		z = sqrtf(1.0f - mag);
	}
	return D3DXVECTOR3(x, y, z);
}
D3DXQUATERNION C_ArcBall::QuatFromBallPoints(const D3DXVECTOR3& vFrom, const D3DXVECTOR3& vTo)
{
	float fDot = D3DXVec3Dot(&vFrom, &vTo);
	D3DXVECTOR3 vAxis;
	D3DXVec3Cross(&vAxis, &vFrom, &vTo);
	return D3DXQUATERNION(vAxis.x, vAxis.y, vAxis.z, fDot);

}

void  C_ArcBall::OnBegin(int nX, int nY)
{
	m_bDrag = true;
	m_qDown = m_qNow;
	m_qXDown = m_qXNow;
	// 2D -> 3D
	m_vDownPt = ScreenToVector(nX, nY);
	m_vXDownPt = ScreenToVectorX(nX);
}
void  C_ArcBall::OnMove(int nX, int nY)
{
	if (m_bDrag)
	{
		m_vCurrentPt = ScreenToVector(nX, nY);
		m_vXCurrentPt = ScreenToVectorX(nX);
		m_qNow = m_qDown * QuatFromBallPoints(m_vDownPt, m_vCurrentPt);
		m_qXNow = m_qXDown * QuatFromBallPoints(m_vXDownPt, m_vXCurrentPt);
	}
}
void  C_ArcBall::OnEnd()
{
	m_bDrag = false;
}

C_ArcBall::C_ArcBall()
{
	m_bDrag = false;
	D3DXQuaternionIdentity(&m_qDown);
	D3DXQuaternionIdentity(&m_qNow);
	D3DXQuaternionIdentity(&m_qXDown);
	D3DXQuaternionIdentity(&m_qXNow);
	m_vDownPt = D3DXVECTOR3(0, 0, 0);
	m_vCurrentPt = D3DXVECTOR3(0, 0, 0);
}


C_ArcBall::~C_ArcBall()
{
}
