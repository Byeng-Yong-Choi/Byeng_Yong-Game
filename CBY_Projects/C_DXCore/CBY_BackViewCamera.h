#pragma once
#include "CCamera.h"
class CBY_BackViewCamera:public CCamera
{
public:
	D3DXMATRIX m_mModelLastRot;
	D3DXMATRIX m_mModelRot;

public:
	D3DXVECTOR3			m_vOffsetDistance;		// 오브젝트의 위치에서 각 방향으로 떨어져 있는 거리 
	D3DXVECTOR3			m_vLerpUpVector;
	D3DXVECTOR3			m_vCameraDestination;
	// 모델의 월드 변환 행렬의 방향벡터
	D3DXVECTOR3			m_vObjectVector[3];
	float				m_fPrevRadius;
	float m_fMoveTime;

public:
	virtual bool Frame() override;
	void Side(float fValue);
	void Front(float fValue);
	void Update(D3DXVECTOR4 rotvalue);
	virtual void MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	CBY_BackViewCamera();
	virtual ~CBY_BackViewCamera();
};

