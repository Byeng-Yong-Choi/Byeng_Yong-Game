#pragma once
#include "C_Device.h"
#include "CCamera.h"
#include "C_Model.h"
#include "CBY_Input.h"
#include "C_DXWrite.h"
#include "CShapeLine.h"
#include "C_DebugCamera.h"
#include "CBY_Select.h"


class CBYCore:public C_Device
{
protected:
	bool GetGameRun;
	CBYTime	m_Timer;
	std::shared_ptr<C_DXWrite> m_Font;
	std::shared_ptr<CShapeLine> m_DebugLine;
	CCamera m_DebugCamera;
	CCamera* m_pMainCamera;
	bool bDebug;
	//CBY_Input m_Input;

public:
	CBY_Select m_Select;

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	void Debug();
	bool CoreInit();
	bool CoreFrame();
	bool PreRender();
	bool CoreRender();
	bool PostRender();
	bool CoreRelease();
	bool GameRun();
	virtual HRESULT CreateDXrc();		//리사이즈시 DX소스의 변경후 생성
	virtual HRESULT	DeleteDXrc();		//리사이즈시 DX소스 변경을 위해 삭제
	virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void ClearContext();

public:
	CBYCore();
	virtual ~CBYCore();
};
