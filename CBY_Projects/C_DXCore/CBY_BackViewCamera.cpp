#include "CBY_BackViewCamera.h"
#include "CBY_Input.h"

void CBY_BackViewCamera::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_LBUTTONDOWN)
	{
		int iMouseX = LOWORD(lParam);
		int iMouseY = HIWORD(lParam);
		m_ViewArcBall.OnBegin(iMouseX, iMouseY);
	}
	if (msg == WM_RBUTTONDOWN)
	{
		int iMouseX = LOWORD(lParam);
		int iMouseY = HIWORD(lParam);
		m_WorldArcBall.OnBegin(iMouseX, iMouseY);
	}
	if (msg == WM_LBUTTONUP)
	{
		m_ViewArcBall.OnEnd();
	}
	if (msg == WM_RBUTTONUP)
	{
		m_WorldArcBall.OnEnd();
	}
	if (msg == WM_MOUSEMOVE)
	{
		int iMouseX = LOWORD(lParam);
		int iMouseY = HIWORD(lParam);
		/*if (m_fMoveTime < 0.1f)
		{
			m_ViewArcBall.OnBegin(iMouseX, iMouseY);
		}*/

		m_WorldArcBall.OnMove(iMouseX, iMouseY);
		m_ViewArcBall.OnMove(iMouseX, iMouseY);

		/*m_fMoveTime += g_SecondTime;
		if (m_fMoveTime > 3.0f)
		{
			m_ViewArcBall.OnEnd();
			m_fMoveTime = 0.0f;
		}*/
	}
	if (msg == WM_MOUSEWHEEL)
	{
		m_nMouseWheelDelta = -(short)HIWORD(wParam);
	}
}

bool CBY_BackViewCamera::Frame()
{
	if (I_Input.GetKeyCheck('W'))
	{
		Front(-1.0f);
	}
	if (I_Input.GetKeyCheck('S'))
	{
		Front(1.0f);
	}
	if (I_Input.GetKeyCheck('A'))
	{
		Side(1.0f);
	}
	if (I_Input.GetKeyCheck('D'))
	{
		Side(-1.0f);
	}

	m_fRadius += m_nMouseWheelDelta * m_fRadius * 0.1f / 120.0f;
	m_fRadius = __min(m_fMaxRadius, m_fRadius);
	m_fRadius = __max(m_fMinRadius, m_fRadius);
	m_nMouseWheelDelta = 0;

	float x = -(I_Input.m_Mouse.x - (Winrt.right / 2)) / (Winrt.right / 2);
	float y = ((Winrt.bottom / 2) - I_Input.m_Mouse.y) / (Winrt.bottom / 2);
	float z = 0;
	float mag = x * x + y * y;
	if (mag > 1.0f)
	{
		float scale = 1.0f / sqrtf(mag);
		x *= scale*10;
		y *= scale*10;
	}
	else
	{
		z = sqrtf(1.0f - mag);
	}

	m_At.x += x;
	m_At.y += y;
	m_At.z += -z;

	//m_At = D3DXVECTOR3(0, 0, -300);
	CreateViewMatrix(m_Pos, m_At);
	D3DXVECTOR3 vPos = m_At + m_BackPos *m_fRadius;
	D3DXMatrixLookAtLH(&m_View, &vPos, &m_At, &m_DefaultUp);
	
	//D3DXMATRIX mCameraRot;
	//D3DXMatrixInverse(&mCameraRot, NULL, m_ViewArcBall.GetRotationMatrix());

	////// Transform vectors based on camera's rotation matrix
	//D3DXVECTOR3 vWorldUp, vWorldAhead;
	//D3DXVECTOR3 vLocalUp = D3DXVECTOR3(0, 1, 0);
	//D3DXVECTOR3 vLocalAhead = D3DXVECTOR3(0, 0, 1);
	//D3DXVec3TransformCoord(&vWorldUp, &vLocalUp, &mCameraRot);
	//D3DXVec3TransformCoord(&vWorldAhead, &vLocalAhead, &mCameraRot);

	//D3DXMatrixLookAtLH(&m_View, &m_Pos, &m_At, &vWorldUp);

	//D3DXMATRIX mInvView;
	//D3DXMatrixInverse(&mInvView, NULL, &m_View);
	//mInvView._41 = mInvView._42 = mInvView._43 = 0;

	//D3DXMATRIX mModelLastRotInv;
	//D3DXMatrixInverse(&mModelLastRotInv, NULL, &m_mModelLastRot);

	//m_vCameraDestination = m_Pos = m_At - vWorldAhead;

	//D3DXMATRIX mModelRot;
	//mModelRot = *m_WorldArcBall.GetRotationMatrix();

	//m_mModelRot = m_mModelRot * m_View * mModelLastRotInv * mModelRot * mInvView;

	//m_mModelLastRot = mModelRot;

	//// Translate the rotation matrix to the same position as the lookAt position
	//m_mModelRot._41 = m_At.x;
	//m_mModelRot._42 = m_At.y;
	//m_mModelRot._43 = m_At.z;

	//// Translate world matrix so its at the center of the model
	//D3DXMATRIX mTrans;
	//D3DXMatrixTranslation(&mTrans, -m_vModelCenter.x, -m_vModelCenter.y, -m_vModelCenter.z);
	//m_World = mTrans * m_mModelRot;

	UpdateVector();
	return true;
}


void CBY_BackViewCamera::Side(float fValue)
{
	m_At.x += fValue * g_SecondTime * m_fSpeed;
}
void CBY_BackViewCamera::Front(float fValue)
{
	m_At.z += fValue * g_SecondTime* m_fSpeed;
}


CBY_BackViewCamera::CBY_BackViewCamera()
{

}


CBY_BackViewCamera::~CBY_BackViewCamera()
{
}
